export module handle_resize;

export import editor_update;

export void handle_resizes(EditorState* state)
{
    static kl::Int2 last_window_size = {};
    const kl::Int2 current_window_size = state->window->size();

    if (current_window_size.x > 0 && current_window_size.y > 0 && current_window_size != last_window_size) {
        state->gpu->resize_internal(current_window_size);
        state->gpu->set_viewport_size(current_window_size);
        last_window_size = current_window_size;
    }
}
