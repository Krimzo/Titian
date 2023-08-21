#pragma once

// External
#include "klib.h"
#include "jdk.h"

// Alter state
#include "setup/setup_state.h"
#include "update/update_state.h"

// Render
#include "render/render_chain.h"
#include "gui/gui_render.h"

// State
#include "state/editor_state.h"
#include "state/logger_state.h"
#include "state/render_state.h"
#include "state/gui_state.h"
#include "state/script_state.h"

// Serialization
#include "serialization/serialization.h"
#include "serialization/serializer.h"
#include "serialization/deserializer.h"

// Scripting
#include "scripting/script.h"


int editor_main();

void  editor_setup(editor_state* state);
void editor_update(editor_state* state);
