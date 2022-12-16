package named

import java.io.Serializable

class NameHolder : Serializable {
    val names: HashSet<String> = HashSet()

    fun clear() {
        names.clear()
    }
}