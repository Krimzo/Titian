package named

import java.io.Serializable

abstract class Named(private val holder: NameHolder?, name: String?) : Serializable {
    var name: String? = null
        private set

    init {
        setName(name ?: "None")
    }

    fun eraseName() {
        holder?.names?.remove(name)
        name = null
    }

    fun setName(name: String) {
        var possibleName = name
        holder?.let {
            holder.names.remove(this.name)
            var i = 1
            while (holder.names.contains(possibleName)) {
                possibleName = name + i++
            }
            holder.names.add(possibleName)
        }
        this.name = possibleName
    }
}