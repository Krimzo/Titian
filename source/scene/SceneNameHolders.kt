package scene

import named.NameHolder
import java.io.Serializable

class SceneNameHolders : Serializable {
    val entity: NameHolder = NameHolder()
    val mesh: NameHolder = NameHolder()
    val material: NameHolder = NameHolder()
    val texture: NameHolder = NameHolder()
}
