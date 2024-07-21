#include "titian.h"


titian::DefaultMeshes::DefaultMeshes(Scene* scene)
{
    auto create_mesh = [&](Ref<Mesh>& mesh, const char* filename)
    {
        if (Optional data = scene->get_assimp_data(filename)) {
            const aiScene* ai_scene = data.value().importer->GetScene();
            for (uint32_t i = 0; i < ai_scene->mNumMeshes; i++) {
                mesh = scene->load_assimp_mesh(ai_scene, ai_scene->mMeshes[i]);
                break;
            }
        }
        kl::assert(mesh->graphics_buffer, "Failed to init mesh: ", filename);
    };

    WorkQueue queue;
    queue.add_task([&] { create_mesh(cube, "package/meshes/cube.obj"); });
    queue.add_task([&] { create_mesh(sphere, "package/meshes/sphere.obj"); });
    queue.add_task([&] { create_mesh(capsule, "package/meshes/capsule.obj"); });
    queue.add_task([&] { create_mesh(monke, "package/meshes/monke.obj"); });
    queue.finalize();
}
