#include "main.h"


titian::DefaultMeshes::DefaultMeshes(Scene* scene)
{
    auto create_mesh = [&](kl::Object<Mesh>& mesh, const char* filename)
    {
        if (std::optional data = scene->get_assimp_data(filename)) {
            const aiScene* ai_scene = data.value().importer->GetScene();
            for (uint32_t i = 0; i < ai_scene->mNumMeshes; i++) {
                mesh = scene->load_assimp_mesh(ai_scene, ai_scene->mMeshes[i]);
                break;
            }
        }
        kl::assert(mesh->graphics_buffer, "Failed to init mesh: ", filename);
    };

    WorkQueue queue;
    queue.add_task([&] { create_mesh(cube, "builtin/meshes/cube.obj"); });
    queue.add_task([&] { create_mesh(sphere, "builtin/meshes/sphere.obj"); });
    queue.add_task([&] { create_mesh(capsule, "builtin/meshes/capsule.obj"); });
    queue.add_task([&] { create_mesh(monke, "builtin/meshes/monke.obj"); });
    queue.finalize();
}
