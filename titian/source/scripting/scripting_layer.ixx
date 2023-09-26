export module scripting_layer;

export import layer;
export import jdk;

export namespace titian {
	class ScriptingLayer : public Layer
	{
	public:
		ScriptingLayer()
		{
			// JVM paths
			const std::vector<std::string> jvm_args = {
				"-Djava.class.path=.",
				"-Djava.class.path=script_sdk/out/artifacts/script_sdk_jar/script_sdk.jar",
			};

			// Prep
			const jint error = java::init("jdk/bin/server/jvm.dll", jvm_args);
			kl::assert(error == 0, kl::format("Failed to init jvm. Error code: 0x", std::hex, error));
		}

		~ScriptingLayer() override
		{}
		
		bool update() override
		{
			return true;
		}
	};
}
