package scene

import editor.Editor
import entity.Entity
import entity.material.Material
import glparts.Mesh
import glparts.Texture
import physics.abs.Physical
import java.io.*

class Scene : ArrayList<Entity>(), Physical, Serializable {
    val materials: MutableSet<Material> = HashSet()
    val textures: MutableSet<Texture> = HashSet()
    val meshes: MutableSet<Mesh> = HashSet()
    val names: SceneNameHolders = SceneNameHolders()
    val selected: SceneSelectedData = SceneSelectedData()

    override fun add(element: Entity): Boolean {
        if (super.add(element)) {
            element.components.mesh.mesh?.let {
                meshes.add(it)
            }
            element.components.material.material?.let {
                materials.add(it)
                it.colorMap?.let { it1 -> textures.add(it1) }
                it.normalMap?.let { it1 -> textures.add(it1) }
                it.roughnessMap?.let { it1 -> textures.add(it1) }
            }
            return true
        }
        return false
    }

    override fun updatePhysics(deltaT: Float) {
        for (entity in this) {
            entity.updatePhysics(deltaT)
        }
    }

    fun toFile(filepath: String): Boolean {
        return try {
            File(filepath).parentFile.mkdirs()
            ObjectOutputStream(FileOutputStream(filepath)).use { stream ->
                stream.writeObject(this)
            }
            true
        }
        catch (ignored: Exception) {
            println("Scene saving error, \"$filepath\"")
            false
        }
    }

    companion object {
        fun fromFile(filepath: String, editor: Editor): Scene? {
            try {
                ObjectInputStream(FileInputStream(filepath)).use { stream ->
                    val scene = stream.readObject() as Scene
                    for (entity in scene) {
                        entity.editor = editor
                    }
                    return scene
                }
            }
            catch (ignored: Exception) {
                println("Scene loading error, \"$filepath\"")
                return null
            }
        }
    }
}