package scene

import camera.abs.Camera
import entity.Entity
import light.AmbientLight
import light.DirectionalLight
import java.io.Serializable

class SceneSelectedData : Serializable {
    var entity: Entity? = null
    var camera: Camera? = null
    var ambientLight: AmbientLight? = null
    var directionalLight: DirectionalLight? = null
}
