package script

class ScriptLoader : ClassLoader() {
    fun load(name: String, data: ByteArray): Class<*>? {
        defineClass(name, data, 0, data.size)

        return try {
            loadClass(name)
        }
        catch (ignored: Exception) {
            null
        }
    }
}