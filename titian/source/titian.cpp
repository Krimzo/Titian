#include "titian.h"


int titian::titian_entry( int argc, str* argv, EntryType type )
{
    Ref<LayerStack> layer_stack;

    if ( type == EntryType::GAME )
    {
        String entry_scene = "entry.titian";
        if ( argc >= 3 )
        {
            entry_scene = argv[2];
            Logger::log( "Using argument entry scene: ", entry_scene );
        }
        else if ( _conf_data.contains( CONF_ENTRY_SCENE ) )
        {
            entry_scene = _conf_data.at( CONF_ENTRY_SCENE )->get_string().value_or( entry_scene );
            Logger::log( "Using .json entry scene: ", entry_scene );
        }
        else
        {
            Logger::log( "Entry scene defaulting to ", entry_scene );
        }
        layer_stack = new TitianGame( entry_scene );
    }
    else if ( type == EntryType::GAME_EDITOR )
    {
        layer_stack = new TitianEditor();
    }
    else
    {
        Logger::log( "Invalid entry type" );
    }

    if ( !layer_stack )
        return 1;

    while ( layer_stack->process_layers() );
    return 0;
}
