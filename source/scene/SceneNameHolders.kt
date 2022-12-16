package scene

import named.NameHolder
import java.io.Serializable

class SceneNameHolders : Serializable {
    val entity = NameHolder()
    val mesh = NameHolder()
    val material = NameHolder()
    val texture = NameHolder()
}