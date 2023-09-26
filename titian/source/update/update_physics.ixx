export module update_physics;

export import editor_update;

export void update_physics(EditorState* state)
{
    if (state->game_running) {
        const float delta_t = state->timer.delta();
        state->scene->update_physics(delta_t);
    }
}
