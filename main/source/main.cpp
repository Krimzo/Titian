#include "main.h"

#include "editor.h"
#include "game.h"
#include "sandbox.h"


static const executable exe_type = executable::sandbox;

int main()
{
	switch (exe_type) {
	case executable::editor:
		return editor_main();

	case executable::game:
		return game_main();

	case executable::sandbox:
		return sandbox_main();
	}
    return -1;
}
