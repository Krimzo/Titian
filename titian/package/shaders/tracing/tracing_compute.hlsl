struct AABB
{
	float3 position;
	float3 size;
	
	float3 min_point()
    {
		return position - size;
    }

    float3 max_point()
    {
		return position + size;
    }
	
	bool contains(float3 pnt)
	{
		float3 minp = min_point();
		float3 maxp = max_point();
        return
            (pnt.x >= minp.x && pnt.x <= maxp.x) &&
            (pnt.y >= minp.y && pnt.y <= maxp.y) &&
            (pnt.z >= minp.z && pnt.z <= maxp.z);
	}
};

struct Vertex
{
	float3 position;
	float3 normal;
	float2 uv;
};

struct Triangle
{
	Vertex a;
	Vertex b;
	Vertex c;
	
	float3 weights(float3 position)
	{
		float3 v0 = a.position - c.position;
        float3 v1 = b.position - c.position;
        float3 v2 = position - c.position;

        float d00 = dot(v0, v0);
        float d01 = dot(v0, v1);
        float d11 = dot(v1, v1);
        float d20 = dot(v2, v0);
        float d21 = dot(v2, v1);

        float inverse_denom = 1.0f / (d00 * d11 - d01 * d01);
        float w1 = (d11 * d20 - d01 * d21) * inverse_denom;
        float w2 = (d00 * d21 - d01 * d20) * inverse_denom;
		return float3( w1, w2, 1.0f - w1 - w2 );
	}
	
	float interpolate(float3 weights, float3 values)
	{
		return dot(weights, values);
	}
	
	Vertex interpolate_self(float3 weights)
	{
		Vertex result;
		result.position.x = interpolate(weights, float3(a.position.x, b.position.x, c.position.x));
		result.position.y = interpolate(weights, float3(a.position.y, b.position.y, c.position.y));
		result.position.z = interpolate(weights, float3(a.position.z, b.position.z, c.position.z));
		result.normal.x = interpolate(weights, float3(a.normal.x, b.normal.x, c.normal.x));
		result.normal.y = interpolate(weights, float3(a.normal.y, b.normal.y, c.normal.y));
		result.normal.z = interpolate(weights, float3(a.normal.z, b.normal.z, c.normal.z));
		result.normal = normalize(result.normal);
		result.uv.x = interpolate(weights, float3(a.uv.x, b.uv.x, c.uv.x));
		result.uv.y = interpolate(weights, float3(a.uv.y, b.uv.y, c.uv.y));
		return result;
	}
};

struct Ray
{
	float3 origin;
	float3 direction;
	
	bool intersect_aabb(AABB aabb, inout float3 intersection)
	{
		bool result = false;
		if (aabb.contains(origin))
		{
            intersection = origin;
			result = true;
		}
		else
		{
			float3 inv_ray = 1.0f / direction;
			float3 t1 = (aabb.min_point() - origin) * inv_ray;
			float3 t2 = (aabb.max_point() - origin) * inv_ray;
			float3 t_min = min(t1, t2);
			float3 t_max = max(t1, t2);
			float t_min_max = max(max(t_min.x, t_min.y), t_min.z);
			float t_max_min = min(min(t_max.x, t_max.y), t_max.z);
			if (t_max_min >= 0.0f && t_min_max <= t_max_min)
			{
				intersection = origin + direction * t_min_max;
				result = true;
			}
		}
		return result;
	}
	
	bool intersect_triangle(Triangle triangl, inout float3 intersection)
	{
		bool result = false;
		float3 edge1 = triangl.b.position - triangl.a.position;
		float3 edge2 = triangl.c.position - triangl.a.position;
        float3 h = cross(direction, edge2);
		float3 s = origin - triangl.a.position;
        float f = 1.0f / dot(edge1, h);
        float u = dot(s, h) * f;
        if (u >= 0.0f && u <= 1.0f)
		{
			float3 q = cross(s, edge1);
			float v = dot(direction, q) * f;
			if (v >= 0.0f && (u + v) <= 1.0f)
			{
				float t = dot(edge2, q) * f;
				if (t > 0.0f)
				{
					intersection = origin + direction * t;
					result = true;
				}
			}
		}
		return result;
	}
};

struct Mesh
{
	AABB aabb;
	int triangles_first;
	int triangles_count;
};

struct Material
{
	float3 color;
	float texture_blend;
	float reflectivity_factor;
	float refraction_index;
};

struct Entity
{
	Mesh mesh;
	Material material;
};

struct Payload
{
	int entity;
	int triangl;
	float3 intersect;
};

float4x4 CAMERA_INV_MAT;
float4 CAMERA_BACKGROUND;
float3 CAMERA_POSITION;
int DEPTH_LIMIT;
float3 DIRECTIONAL_DIRECTION;
float CAMERA_IS_WIREFRAME;
float3 DIRECTIONAL_COLOR;
float CAMERA_HAS_SKYBOX;
float CAMERA_BOUND;
float DIRECTIONAL_POINT_SIZE;
float DIRECTIONAL_BOUND;
float TIME;

RWTexture2D<float4> TARGET : register(u0);

StructuredBuffer<Entity> ENTITIES : register(t0);
StructuredBuffer<Triangle> TRIANGLES : register(t1);
TextureCube<float4> SKYBOX : register(t2);

SamplerState SKYBOX_SAMPLER : register(s0);

float rand_float(float2 ndc)
{
	return frac(sin(dot(ndc, float2(12.9898f, 78.233f))) * 43758.5453f);
}

float3 align_to_normal(float3 sample, float3 normal)
{
    float3 tangent = abs(normal.x) > 0.1f ? float3(0.0f, 1.0f, 0.0f) : float3(1.0f, 0.0f, 0.0f);
    tangent = normalize(cross(normal, tangent));
    float3 bitangent = cross(normal, tangent);
    return sample.x * tangent + sample.y * bitangent + sample.z * normal;
}

float3 rand_float3(float3 normal, float2 seed)
{
	seed += float2(TIME, TIME * 1.37f);
	
    float u = rand_float(seed);
    float v = rand_float(seed + float2(321.0f, 32.0f));
    float theta = 2.0f * radians(180.0f) * u;
    float r = sqrt(1.0f - v);
    float x = r * cos(theta);
    float y = r * sin(theta);
    float z = sqrt(v);

    float3 sample = { x, y, z };
    return align_to_normal(sample, normal);
}

float2 compute_ndc(uint2 resolution, float2 id)
{
	return float2(
		float(id.x) / (resolution.x - 1) * 2.0f - 1.0f,
		float(resolution.y - 1 - id.y) / (resolution.y - 1) * 2.0f - 1.0f
	);
}

float3 sample_background(float3 direction)
{
	float3 result = 0.0f;
	if (CAMERA_HAS_SKYBOX)
	{
		result = SKYBOX.SampleLevel(SKYBOX_SAMPLER, direction, 0).xyz;
	}
	else
	{
		result = CAMERA_BACKGROUND.xyz;
	}
	return result;
}

bool scene_trace(Ray ray, int blacklist, inout Payload pyld)
{
	bool result = false;
	
	float3 intersection = 0.0f;
	float min_distance = 1e30f;
	uint entities_count, _ignored;
	ENTITIES.GetDimensions(entities_count, _ignored);
	
	for (uint i = 0; i < entities_count; ++i)
	{
		Entity entity = ENTITIES[i];
		if (!ray.intersect_aabb(entity.mesh.aabb, intersection))
			continue;

		float distance = length(intersection - ray.origin);
		if (distance >= min_distance)
			continue;
		
		for (int j = entity.mesh.triangles_first; j < (entity.mesh.triangles_first + entity.mesh.triangles_count); ++j)
		{
			Triangle triangl = TRIANGLES[j];
			if (j == blacklist)
				continue;

			if (!ray.intersect_triangle(triangl, intersection))
				continue;

			float distance = length(intersection - ray.origin);
			if (distance >= min_distance)
				continue;

			min_distance = distance;
			pyld.entity = i;
			pyld.triangl = j;
			pyld.intersect = intersection;
			result = true;
		}
	}
	return result;
}

float3 trace_ray(Ray ray, float2 seed)
{
	Payload pyld;
	pyld.entity = -1;
	pyld.triangl = -1;
    pyld.intersect = 0.0f;
	
	float3 result = 1.0f;
    for (int i = 0; i <= DEPTH_LIMIT; ++i)
	{
		if (i == DEPTH_LIMIT)
		{
			result *= 0.0f;
			break;
		}
		if (!scene_trace(ray, pyld.triangl, pyld))
		{
			float3 light = sample_background(ray.direction);
			if (DIRECTIONAL_BOUND)
			{
                float diffuse = saturate(dot(-DIRECTIONAL_DIRECTION, ray.direction));
                light += DIRECTIONAL_COLOR * DIRECTIONAL_POINT_SIZE * diffuse;
			}
            result *= saturate(light);
			break;
		}
		else
		{
			Material material = ENTITIES[pyld.entity].material;
			Triangle triangl = TRIANGLES[pyld.triangl];
			
			float3 interp_weights = triangl.weights(pyld.intersect);
			Vertex interp_vertex = triangl.interpolate_self(interp_weights);

			float3 normal = interp_vertex.normal;
			if (dot(-ray.direction, normal) < 0.0f)
			{
				normal = -normal;
			}
			
			if (material.reflectivity_factor >= 0.0f)
            {
                float3 random_dir = normalize(rand_float3(normal, seed + float2(321.0f, 32.0f) * i));
                float random_infl = 1.0f - clamp(material.reflectivity_factor, 0.0f, 1.0f);
                float3 random_norm = normalize(normal + random_dir * random_infl);
                float3 reflect_dir = reflect(ray.direction, random_norm);
                ray.origin = pyld.intersect;
                ray.direction = reflect_dir;
            }
			else
			{
                float3 random_dir = normalize(rand_float3(normal, seed - float2(321.0f, 32.0f) * i));
				float random_infl = 1.0f - clamp(-material.reflectivity_factor, 0.0f, 1.0f);
				float3 random_norm = normalize(normal + random_dir * random_infl);
				float3 refract_dir = refract(ray.direction, random_norm, 1.0f / material.refraction_index);
				ray.origin = pyld.intersect;
				ray.direction = refract_dir;
			}
            result *= material.color;
        }
	}
	return result;
}

float3 render_pixel(float2 ndc)
{
	float4 ndc_pos = mul(float4(ndc, 1.0f, 1.0f), CAMERA_INV_MAT);
	float3 direction = normalize( ndc_pos.xyz / ndc_pos.w - CAMERA_POSITION );
	
	Ray ray;
	ray.origin = CAMERA_POSITION;
	ray.direction = direction;
	
	float3 result = 0.0f;
	if (CAMERA_BOUND)
	{
		if (CAMERA_IS_WIREFRAME)
		{
			result = sample_background(ray.direction);
		}
		else
		{
			result = trace_ray(ray, ndc);
		}
	}
	return result;
}

[numthreads(32, 32, 1)]
void c_shader(uint3 id : SV_DispatchThreadID)
{
	uint2 resolution;
	TARGET.GetDimensions(resolution.x, resolution.y);
	if (id.x >= resolution.x || id.y >= resolution.y)
		return;
	
	float2 ndc = compute_ndc(resolution, id.xy);
	TARGET[id.xy] += float4(render_pixel(ndc), 1.0f);
}
