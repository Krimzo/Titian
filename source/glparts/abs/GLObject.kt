package glparts.abs

import named.NameHolder
import named.Named
import window.GLContext
import java.io.Serializable

abstract class GLObject(holder: NameHolder?, name: String?, @field:Transient val context: GLContext?) : Named(holder, name), Serializable {
    protected fun finalize() {
        if (isAllocated()) {
            synchronized(objects) {
                objects.add(this)
            }
        }
    }

    abstract fun deallocate()

    abstract fun isAllocated(): Boolean

    companion object {
        private val objects: HashSet<GLObject> = HashSet()

        fun cleanup() {
            synchronized(objects) {
                for (`object` in objects) {
                    `object`.deallocate()
                }
                objects.clear()
            }
        }
    }
}