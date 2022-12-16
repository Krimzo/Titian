package script

import utility.helper.FileHelper
import javax.tools.JavaCompiler
import javax.tools.ToolProvider

class ScriptCompiler {
    private val compiler: JavaCompiler = ToolProvider.getSystemJavaCompiler()

    fun compile(filepath: String): ByteArray? {
        try {
            if (compiler.run(null, null, null, filepath) == 0) {
                val compiledPath = FileHelper.getWithoutExtension(filepath) + ".class"
                val data = FileHelper.readBytes(compiledPath)
                FileHelper.delete(compiledPath)
                return data
            }
        }
        catch (ignored: Exception) {
            println("Script \"$filepath\" compilation error")
        }
        return null
    }
}