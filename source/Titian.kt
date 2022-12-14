import editor.Editor
import entity.Entity
import entity.material.Material
import glparts.Mesh
import glparts.Texture
import light.AmbientLight
import light.DirectionalLight
import math.Float2
import math.Float3
import scene.Scene
import script.abs.Scriptable
import java.util.*

private fun saveTestScene(editor: Editor) {
    val scene = Scene()
    scene.selected.ambientLight = AmbientLight(scene.names.entity, "Ambient Light", editor)
    scene.selected.directionalLight = DirectionalLight(scene.names.entity, "Directional Light", editor)

    scene.selected.ambientLight?.let {
        scene.add(it)
    }

    scene.selected.directionalLight?.let {
        it.setDirection(Float3(1f, -1f, -1f))
        scene.add(it)
    }

    val mesh = Mesh(scene.names.mesh, "Monke Mesh", editor.window.context, "resource/meshes/monke.obj")
    val material = Material(scene.names.material, "Monke Material")
    material.colorMap = Texture(scene.names.texture, "Checkers Texture", editor.window.context, "test/textures/checkers.png")
    material.colorBlend = 1f

    val size = 1
    for (y in -size..size) {
        for (x in -size..size) {
            val monke = Entity(scene.names.entity, "Monke", editor)
            monke.components.transform.position = Float3(Float2(x.toFloat(), y.toFloat()) * 2.5f, 0f)
            monke.components.physics.angular.y = (Random().nextFloat() * 2 - 1) * 36
            monke.components.mesh.mesh = mesh
            monke.components.material.material = material
            scene.add(monke)
        }
    }
    scene.toFile("test/scenes/test.titian")
}

private fun loadTestScene(editor: Editor) {
    editor.scene = Scene.fromFile("test/scenes/test.titian", editor) ?: Scene()
}

fun main() {
    val editor = Editor()

    System.setOut(editor.logger)
    System.setErr(editor.logger)
    Scriptable.setEditor(editor)

    editor.setup {
        try {
            saveTestScene(it)
            loadTestScene(it)
        }
        catch (ignored: Exception) {
            println("Editor setup error")
        }
    }

    while (editor.window.process()) {
        editor.update()
    }
    editor.destroy()
}
