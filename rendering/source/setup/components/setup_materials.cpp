#include "state/state_machine.h"


void setup_materials(state_machine* state)
{
    // Colors
    const kl::ref<kl::material> white_material = kl::make<kl::material>();
    white_material->color = kl::colors::white;
    state->materials["white"] = white_material;

    // Bullets
    const kl::ref<kl::material> bullet_material = kl::make<kl::material>();
    bullet_material->color = kl::color(232, 230, 227);
    bullet_material->reflection_factor = 0.15f;
    state->materials["bullet"] = bullet_material;

    const kl::ref<kl::material> bullet_casing_material = kl::make<kl::material>();
    bullet_casing_material->color = kl::color(210, 180, 130);
    bullet_casing_material->reflection_factor = 0.2f;
    state->materials["bullet_casing"] = bullet_casing_material;

    const kl::ref<kl::material> casing_primer_material = kl::make<kl::material>();
    casing_primer_material->color = kl::color(232, 230, 227);
    casing_primer_material->reflection_factor = 0.1f;
    state->materials["bullet_primer"] = casing_primer_material;
}
