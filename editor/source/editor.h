#pragma once

// External
#include "klib.h"
#include "jdk.h"

// Alter state
#include "setup\editor_setup.h"
#include "update\editor_update.h"

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
#include "scripting/engine_script.h"
#include "scripting/dll/dll_script.h"
#include "scripting/jvm/jvm_script.h"
#include "scripting/jvm/jvm_script_transfer.h"
#include "scripting/jvm/jvm_class_info.h"


int editor_main();

void  editor_setup(editor_state* state);
void editor_update(editor_state* state);
