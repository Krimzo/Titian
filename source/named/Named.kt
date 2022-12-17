package named

import java.io.Serializable

abstract class Named(protected val holder: NameHolder, name: String) : Serializable {
    var name: String = ""
        private set

    init {
        setName(name)
    }

    fun eraseName() {
        holder.remove(name)
        name = ""
    }

    fun setName(name: String) {
        var possibleName = name
        holder.remove(this.name)
        var i = 1
        while (holder.contains(possibleName)) {
            possibleName = name + i++
        }
        holder.add(possibleName)
        this.name = possibleName
    }
}