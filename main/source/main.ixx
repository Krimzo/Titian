export module main;

export import jdk;
export import game;
export import editor;
export import sandbox;

enum class ExecutableType
{
    EDITOR = 0,
    GAME = 1,
    SANDBOX = 2,
};

export int main()
{
    // JVM paths
    const std::vector<std::string> jvm_args = {
        "-Djava.class.path=.",
        "-Djava.class.path=script_sdk/out/artifacts/script_sdk_jar/script_sdk.jar",
    };

    // Prep
    int exit_code = -1;
    if (!java::init("jdk/bin/server/jvm.dll", jvm_args)) {
        return exit_code;
    }

    // App
    switch (ExecutableType::SANDBOX)
    {
    case ExecutableType::GAME: exit_code = game_main(); break;
    case ExecutableType::EDITOR: exit_code = editor_main(); break;
    case ExecutableType::SANDBOX: exit_code = sandbox_main(); break;
    }
    return exit_code;
}
