#include "main.h"

#include "editor.h"
#include "game.h"
#include "sandbox.h"
#include "jdk.h"


int main()
{
	// Prep
	int exit_code = -1;
	jdk::init();

	// App
	switch (executable::sandbox)
	{
	case  executable::editor: exit_code =  editor_main(); break;
	case    executable::game: exit_code =    game_main(); break;
	case executable::sandbox: exit_code = sandbox_main(); break;
	}

	// Final
	jdk::uninit();
    return exit_code;
}
