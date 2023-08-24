#pragma once

#include "state/editor_state.h"
#include "scripting/class_info.h"


namespace signatures {
	inline const std::string STRING = java::build_class_signature("java/lang/String");
	inline const std::string FLOAT3 = java::build_class_signature("sdk/math/vector/Float3");
	inline const std::string SHAPE = java::build_class_signature("sdk/render/components/shape/Shape");
	inline const std::string COLLIDER = java::build_class_signature("sdk/render/components/Collider");
	inline const std::string ENTITY = java::build_class_signature("sdk/render/scene/Entity");
}

namespace classes {
	inline class_info FLOAT3 = {};
	inline class_info SHAPE = {};
	inline class_info COLLIDER = {};
	inline class_info ENTITY = {};
}

namespace script_transfer {
	void bind_state(editor_state* state);
	void bind_native_methods();
	void load_classes();
}

namespace script_transfer {
	// System
	void logExt(JNIEnv* env, jobject self, jstring name);

	// Entity
	jobject getEntity(JNIEnv* env, jobject self, jstring name);
	void setEntity(JNIEnv* env, jobject self, jstring name, jobject entity);
}
