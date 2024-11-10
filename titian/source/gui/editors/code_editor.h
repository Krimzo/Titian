#pragma once

#include "standard.h"


namespace titian
{
struct CodeEditor
{
    CodeEditor();

    void load( StringRef const& source );
    void edit( String& source );

    void load_lua_standard();
    void load_hlsl_standard();

private:
    TextEditor m_text_editor;
};
}
