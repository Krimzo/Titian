#include "editor.h"


// Helper
kl::float3 helper::read_float3(jobject object)
{
	static const jfield fields[3] = {
		classes::FLOAT3.get_field("x"),
		classes::FLOAT3.get_field("y"),
		classes::FLOAT3.get_field("z"),
	};
	return {
		java::jni->GetFloatField(object, fields[0]),
		java::jni->GetFloatField(object, fields[1]),
		java::jni->GetFloatField(object, fields[2]),
	};
}

// Setup
static editor_state* bound_state = nullptr;

void script_transfer::bind_state(editor_state* state)
{
	::bound_state = state;
}

void script_transfer::bind_native_methods()
{
	jclass engine_class = java::load_eternal_class("sdk/engine/Engine");
	if (!engine_class) {
		throw std::exception("Failed to load Engine class");
	}

	std::vector<std::tuple<std::string, std::string, void*>> method_infos = {};

	// System
	method_infos.push_back({ "logString", java::build_method_signature(signatures::VOID, { signatures::STRING }), script_transfer::native::logString });

	// Entity
	method_infos.push_back({ "readEntity", java::build_method_signature(signatures::ENTITY, { signatures::STRING }), script_transfer::native::readEntity });
	method_infos.push_back({ "writeEntity", java::build_method_signature(signatures::VOID, {  signatures::STRING, signatures::ENTITY }), script_transfer::native::writeEntity });

	std::vector<JNINativeMethod> methods(method_infos.size());
	for (size_t i = 0; i < method_infos.size(); i++) {
		auto& [name, signature, pointer] = method_infos[i];
		
		JNINativeMethod method = {};
		method.name = name.data();
		method.signature = signature.data();
		method.fnPtr = pointer;

		methods[i] = method;
	}

	jint result = java::jni->RegisterNatives(engine_class, methods.data(), (jint) methods.size());
	if (result != JNI_OK) {
		kl::print("Failed to register native jni functions.");
	}
}

void script_transfer::load_classes()
{
	classes::FLOAT3.load_class("sdk/math/vector/Float3");
	classes::FLOAT3.load_constructor(0, {});
	classes::FLOAT3.load_constructor(1, { signatures::FLOAT, signatures::FLOAT, signatures::FLOAT });
	classes::FLOAT3.load_field("x", signatures::FLOAT);
	classes::FLOAT3.load_field("y", signatures::FLOAT);
	classes::FLOAT3.load_field("z", signatures::FLOAT);

	classes::SHAPE.load_class("sdk/render/components/shape/Shape");
	classes::SHAPE.load_field("type", signatures::INT);
	classes::SHAPE.load_method("getGeometry", signatures::FLOAT3, {});

	classes::COLLIDER.load_class("sdk/render/components/Collider");
	classes::COLLIDER.load_constructor(0, {});
	classes::COLLIDER.load_field("shape", signatures::SHAPE);
	classes::COLLIDER.load_field("rotation", signatures::FLOAT3);
	classes::COLLIDER.load_field("offset", signatures::FLOAT3);
	classes::COLLIDER.load_field("staticFriction", signatures::FLOAT);
	classes::COLLIDER.load_field("dynamicFriction", signatures::FLOAT);
	classes::COLLIDER.load_field("restitution", signatures::FLOAT);

	classes::ENTITY.load_class("sdk/render/scene/Entity");
	classes::ENTITY.load_constructor(0, {});
	classes::ENTITY.load_field("renderScale", signatures::FLOAT3);
	classes::ENTITY.load_field("mesh", signatures::STRING);
	classes::ENTITY.load_field("material", signatures::STRING);
	classes::ENTITY.load_field("rotation", signatures::FLOAT3);
	classes::ENTITY.load_field("position", signatures::FLOAT3);
	classes::ENTITY.load_field("dynamic", signatures::BOOL);
	classes::ENTITY.load_field("gravity", signatures::BOOL);
	classes::ENTITY.load_field("mass", signatures::FLOAT);
	classes::ENTITY.load_field("velocity", signatures::FLOAT3);
	classes::ENTITY.load_field("angular", signatures::FLOAT3);
	classes::ENTITY.load_field("collider", signatures::COLLIDER);
}

// System
void script_transfer::native::logString(JNIEnv* env, jobject self, jstring data)
{
	log_info info = {};
	info.message = java::read_string(data);
	bound_state->logger_state->log(info);
}

// Entity
jobject script_transfer::native::readEntity(JNIEnv* env, jobject self, jstring name)
{
	kl::object entity = bound_state->scene->find_entity(java::read_string(name));
	if (!entity) {
		return nullptr;
	}

	jobject jav_entity = classes::ENTITY.new_instance(0);
	if (!jav_entity) {
		return nullptr;
	}

	// Render
	jobject object_render_scale = classes::FLOAT3.new_instance(1, entity->render_scale.x, entity->render_scale.y, entity->render_scale.z);
	java::jni->SetObjectField(jav_entity, classes::ENTITY.get_field("renderScale"), object_render_scale);

	if (entity->mesh) {
		for (const auto& [mesh_name, mesh] : bound_state->scene->meshes) {
			if (entity->mesh == mesh) {
				java::jni->SetObjectField(jav_entity, classes::ENTITY.get_field("mesh"), java::new_string(mesh_name));
				break;
			}
		}
	}

	if (entity->material) {
		for (const auto& [material_name, material] : bound_state->scene->materials) {
			if (entity->material == material) {
				java::jni->SetObjectField(jav_entity, classes::ENTITY.get_field("material"), java::new_string(material_name));
				break;
			}
		}
	}

	// Geometry
	java::jni->SetBooleanField(jav_entity, classes::ENTITY.get_field("dynamic"), entity->is_dynamic());
	java::jni->SetBooleanField(jav_entity, classes::ENTITY.get_field("gravity"), entity->has_gravity());
	java::jni->SetFloatField(jav_entity, classes::ENTITY.get_field("mass"), entity->mass());

	kl::float3 rotation = entity->rotation();
	jobject object_rotation = classes::FLOAT3.new_instance(1, rotation.x, rotation.y, rotation.z);
	java::jni->SetObjectField(jav_entity, classes::ENTITY.get_field("rotation"), object_rotation);

	kl::float3 position = entity->position();
	jobject object_position = classes::FLOAT3.new_instance(1, position.x, position.y, position.z);
	java::jni->SetObjectField(jav_entity, classes::ENTITY.get_field("position"), object_position);

	// Physics
	kl::float3 velocity = entity->velocity();
	jobject object_velocity = classes::FLOAT3.new_instance(1, velocity.x, velocity.y, velocity.z);
	java::jni->SetObjectField(jav_entity, classes::ENTITY.get_field("velocity"), object_velocity);

	kl::float3 angular = entity->angular();
	jobject object_angular = classes::FLOAT3.new_instance(1, angular.x, angular.y, angular.z);
	java::jni->SetObjectField(jav_entity, classes::ENTITY.get_field("angular"), object_angular);

	// Collision
	//WIP
	kl::object collider = entity->collider();

	return jav_entity;
}

void script_transfer::native::writeEntity(JNIEnv* env, jobject self, jstring jav_name, jobject jav_entity)
{
	// Get
	// Render
	jobject renderScale = java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("renderScale"));
	jstring mesh = (jstring) java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("mesh"));
	jstring material = (jstring) java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("material"));

	// Geometry
	jobject rotation = java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("rotation"));
	jobject position = java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("position"));

	// Physics
	jbool dynamic = java::jni->GetBooleanField(jav_entity, classes::ENTITY.get_field("dynamic"));
	jbool gravity = java::jni->GetBooleanField(jav_entity, classes::ENTITY.get_field("gravity"));
	jfloat mass = java::jni->GetFloatField(jav_entity, classes::ENTITY.get_field("mass"));
	jobject velocity = java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("velocity"));
	jobject angular = java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("angular"));

	// Collision
	jobject collider = java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("collider"));

	// Set
	const std::string name_str = java::read_string(jav_name);
	kl::object entity = bound_state->scene->make_entity(dynamic);
	
	// Render
	entity->render_scale = helper::read_float3(renderScale);

	std::string mesh_str = java::read_string(mesh);
	if (bound_state->scene->meshes.contains(mesh_str)) {
		entity->mesh = bound_state->scene->meshes.at(mesh_str);
	}
	else {
		entity->mesh = nullptr;
	}

	std::string material_str = java::read_string(material);
	if (bound_state->scene->materials.contains(material_str)) {
		entity->material = bound_state->scene->materials.at(material_str);
	}
	else {
		entity->material = nullptr;
	}

	// Geometry
	entity->set_rotation(helper::read_float3(rotation));
	entity->set_position(helper::read_float3(position));

	// Physics
	entity->set_gravity(gravity);
	entity->set_mass(mass);
	entity->set_velocity(helper::read_float3(velocity));
	entity->set_angular(helper::read_float3(angular));

	// Collider
	//WIP
	
	bound_state->scene->add(name_str, entity);
}
