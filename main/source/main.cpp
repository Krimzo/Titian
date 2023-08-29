#include "main.h"

#include "editor.h"
#include "game.h"
#include "sandbox.h"
#include "java.h"


int main()
{
	// JVM paths
	std::vector<std::string> jvm_args = {
		"-Djava.class.path=.",
		"-Djava.class.path=script_sdk/out/artifacts/script_sdk_jar/script_sdk.jar",
	};

	// Prep
	int exit_code = -1;
	if (!java::init("jdk/bin/server/jvm.dll", jvm_args)) {
		return exit_code;
	}

	// App
	switch (executable::sandbox)
	{
	case  executable::editor: exit_code =  editor_main(); break;
	case    executable::game: exit_code =    game_main(); break;
	case executable::sandbox: exit_code = sandbox_main(); break;
	}
    return exit_code;
}
