export module jvm_script_transfer;

export import editor_state;

static EditorState* bound_state = nullptr;

export namespace signatures {
	inline const std::string STRING = java::build_class_signature("java/lang/String");
	inline const std::string FLOAT3 = java::build_class_signature("sdk/math/vector/Float3");
	inline const std::string SHAPE = java::build_class_signature("sdk/render/components/shape/Shape");
	inline const std::string COLLIDER = java::build_class_signature("sdk/render/components/Collider");
	inline const std::string ENTITY = java::build_class_signature("sdk/render/scene/Entity");
}

export namespace classes {
	inline ClassInfo FLOAT3 = {};
	inline ClassInfo SHAPE = {};
	inline ClassInfo COLLIDER = {};
	inline ClassInfo ENTITY = {};
}

export namespace helper {
	kl::Float3 read_float3(const jobject object)
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
}

export namespace script_transfer::native {
	// System
	void logString(JNIEnv* env, jobject self, jstring data)
	{
		LogInfo info = {};
		info.message = java::read_string(data);
		bound_state->logger_state->log(info);
	}

	// Entity
	jobject readEntity(JNIEnv* env, jobject self, jstring jav_name)
	{
		kl::Object entity = bound_state->scene->find_entity(java::read_string(jav_name));
		if (!entity) {
			return nullptr;
		}

		const jobject jav_entity = classes::ENTITY.new_instance(0);
		if (!jav_entity) {
			return nullptr;
		}

		// Render
		const jobject object_render_scale = classes::FLOAT3.new_instance(1, entity->render_scale.x, entity->render_scale.y, entity->render_scale.z);
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

		const kl::Float3 rotation = entity->rotation();
		const jobject object_rotation = classes::FLOAT3.new_instance(1, rotation.x, rotation.y, rotation.z);
		java::jni->SetObjectField(jav_entity, classes::ENTITY.get_field("rotation"), object_rotation);

		const kl::Float3 position = entity->position();
		const jobject object_position = classes::FLOAT3.new_instance(1, position.x, position.y, position.z);
		java::jni->SetObjectField(jav_entity, classes::ENTITY.get_field("position"), object_position);

		// Physics
		const kl::Float3 velocity = entity->velocity();
		const jobject object_velocity = classes::FLOAT3.new_instance(1, velocity.x, velocity.y, velocity.z);
		java::jni->SetObjectField(jav_entity, classes::ENTITY.get_field("velocity"), object_velocity);

		const kl::Float3 angular = entity->angular();
		const jobject object_angular = classes::FLOAT3.new_instance(1, angular.x, angular.y, angular.z);
		java::jni->SetObjectField(jav_entity, classes::ENTITY.get_field("angular"), object_angular);

		// Collision
		//WIP
		kl::Object collider = entity->collider();

		return jav_entity;
	}

	void writeEntity(JNIEnv* env, jobject self, jstring jav_name, jobject jav_entity)
	{
		// Get
		// Render
		const jobject render_scale = java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("renderScale"));
		const jstring mesh = (jstring) java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("mesh"));
		const jstring material = (jstring) java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("material"));

		// Geometry
		const jobject rotation = java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("rotation"));
		const jobject position = java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("position"));

		// Physics
		const jbool dynamic = java::jni->GetBooleanField(jav_entity, classes::ENTITY.get_field("dynamic"));
		const jbool gravity = java::jni->GetBooleanField(jav_entity, classes::ENTITY.get_field("gravity"));
		const jfloat mass = java::jni->GetFloatField(jav_entity, classes::ENTITY.get_field("mass"));
		const jobject velocity = java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("velocity"));
		const jobject angular = java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("angular"));

		// Collision
		jobject collider = java::jni->GetObjectField(jav_entity, classes::ENTITY.get_field("collider"));

		// Set
		const std::string name_str = java::read_string(jav_name);
		kl::Object entity = bound_state->scene->make_entity(dynamic);

		// Render
		entity->render_scale = helper::read_float3(render_scale);

		const std::string mesh_str = java::read_string(mesh);
		if (bound_state->scene->meshes.contains(mesh_str)) {
			entity->mesh = bound_state->scene->meshes.at(mesh_str);
		}
		else {
			entity->mesh = nullptr;
		}

		const std::string material_str = java::read_string(material);
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
}

export namespace script_transfer {
	void bind_state(EditorState* state)
	{
		::bound_state = state;
	}

	void bind_native_methods()
	{
		const jclass engine_class = java::load_eternal_class("sdk/engine/Engine");
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

		if (java::jni->RegisterNatives(engine_class, methods.data(), (jint) methods.size())) {
			kl::print("Failed to register native jni functions.");
		}
	}

	void load_classes()
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
}
