package utility.helper

import math.Float2
import math.Float3
import math.Int2
import math.Vertex
import org.lwjgl.opengl.GL20
import org.lwjgl.opengl.GL32
import org.lwjgl.stb.STBImage
import utility.Memory
import java.awt.Desktop
import java.io.*
import java.nio.file.Files
import java.nio.file.Path
import java.nio.file.Paths
import java.util.*

object FileHelper {
    val separator: String = System.getProperty("file.separator")

    fun defaultPath(): String {
        return Paths.get("").toFile().absolutePath
    }

    fun isEmpty(filepath: String): Boolean {
        return try {
            (File(filepath).listFiles()?.size ?: 0) == 0
        }
        catch (ignored: Exception) {
            println("Folder \"$filepath\" isEmpty() error")
            false
        }
    }

    fun delete(filepath: String): Boolean {
        return try {
            File(filepath).delete()
        }
        catch (ignored: Exception) {
            println("File/folder \"$filepath\" delete() error")
            false
        }
    }

    fun createFile(filepath: String): Boolean {
        return try {
            val file = File(filepath)
            file.parentFile.mkdirs()
            file.createNewFile()
        } catch (ignored: Exception) {
            println("File/folder \"$filepath\" create() error")
            false
        }
    }

    fun createFolder(filepath: String): Boolean {
        return try {
            File(filepath).mkdirs()
        } catch (ignored: Exception) {
            println("File/folder \"$filepath\" create() error")
            false
        }
    }

    fun rename(filepath: String, name: String): Boolean {
        return try {
            val file = File(filepath)
            val newPath = file.parent + separator + name
            file.renameTo(File(newPath))
        } catch (ignored: Exception) {
            println("File/folder \"$filepath\" rename() error")
            false
        }
    }

    fun move(filepath: String, toFolder: String): Boolean {
        return try {
            val from = File(filepath)
            val to = toFolder + separator + from.name
            from.renameTo(File(to))
        } catch (ignored: Exception) {
            println("File/folder \"$filepath\" move() error")
            false
        }
    }

    fun openDefault(filepath: String): Boolean {
        return try {
            Desktop.getDesktop().open(File(filepath))
            true
        } catch (ignored: Exception) {
            println("File \"$filepath\" openDefault() error")
            false
        }
    }

    fun readString(filepath: String): String {
        return try {
            Files.readString(Path.of(filepath))
        }
        catch (ignored: Exception) {
            println("File \"$filepath\" readString() error")
            ""
        }
    }

    fun writeString(filepath: String, data: String): Boolean {
        return try {
            val file = File(filepath)
            file.parentFile.mkdirs()
            FileOutputStream(file).use { stream -> stream.write(data.toByteArray()) }
            true
        }
        catch (ignored: Exception) {
            println("File \"$filepath\" writeString() error")
            false
        }
    }

    fun readBytes(filepath: String): ByteArray? {
        try {
            FileInputStream(filepath).use { stream -> return stream.readAllBytes() }
        }
        catch (ignored: Exception) {
            println("File \"$filepath\" readBytes() error")
            return null
        }
    }

    fun getName(filepath: String): String {
        return try {
            File(filepath).name
        }
        catch (ignored: Exception) {
            println("File/folder \"$filepath\" getName() error")
            ""
        }
    }

    fun getExtension(filepath: String): String {
        val index = filepath.lastIndexOf('.')
        return if (index >= 0) filepath.substring(index + 1) else ""
    }

    fun getWithoutExtension(filepath: String): String {
        val index = filepath.indexOf('.')
        if (index > 0) {
            return filepath.substring(0, index)
        }
        return if (index < 0) {
            filepath
        }
        else {
            ""
        }
    }

    fun getNameWithoutExtension(filepath: String): String {
        return getWithoutExtension(getName(filepath))
    }

    fun listFolders(filepath: String): Array<File>? {
        return try {
            File(filepath).listFiles { obj: File -> obj.isDirectory }
        }
        catch (ignored: Exception) {
            println("Folder \"$filepath\" listFolders() error")
            null
        }
    }

    fun listFiles(filepath: String): Array<File>? {
        return try {
            File(filepath).listFiles { obj: File -> obj.isFile }
        }
        catch (ignored: Exception) {
            println("Folder \"$filepath\" listFiles() error")
            null
        }
    }

    fun parseShader(filepath: String, type: Int): String {
        return try {
            val file = FileReader(filepath)
            val reader = BufferedReader(file)
            val builder = StringBuilder()
            var shouldSave = false
            val shaderName = when (type) {
                GL20.GL_VERTEX_SHADER -> "//vertexshader"
                GL20.GL_FRAGMENT_SHADER -> "//fragmentshader"
                GL32.GL_GEOMETRY_SHADER -> "//geometryshader"
                else -> ""
            }
            var line = ""
            while (reader.readLine()?.also { line = it } != null) {
                val formattedLine = line.lowercase(Locale.getDefault()).replace("\\s".toRegex(), "")
                if (shouldSave) {
                    if (when (formattedLine) {
                            "//vertexshader", "//fragmentshader", "//geometryshader" -> false
                            else -> true
                        }) {
                        builder.append(line).append('\n')
                    } else {
                        break
                    }
                } else if (formattedLine == shaderName) {
                    shouldSave = true
                }
            }
            reader.close()
            file.close()
            builder.toString()
        } catch (e: Exception) {
            e.printStackTrace()
            ""
        }
    }

    fun parseMeshFile(filepath: String): Array<Vertex> {
        return try {
            val file = FileReader(filepath)
            val reader = BufferedReader(file)
            val vertices = ArrayList<Vertex>()
            val xyzBuffer = ArrayList<Float3>()
            val uvBuffer = ArrayList<Float2>()
            val normBuffer = ArrayList<Float3>()

            var fileLine: String
            while (reader.readLine().also { fileLine = it } != null) {
                val lineParts = fileLine.split(" ".toRegex()).dropLastWhile { it.isEmpty() }.toTypedArray()
                when (lineParts[0]) {
                    "v" -> {
                        xyzBuffer.add(Float3(lineParts[1].toFloat(), lineParts[2].toFloat(), lineParts[3].toFloat()))
                    }
                    "vt" -> {
                        uvBuffer.add(Float2(lineParts[1].toFloat(), lineParts[2].toFloat()))
                    }
                    "vn" -> {
                        normBuffer.add(Float3(lineParts[1].toFloat(), lineParts[2].toFloat(), lineParts[3].toFloat()))
                    }
                    "f" -> {
                        for (i in 1..3) {
                            val linePartParts = lineParts[i].split("/".toRegex()).dropLastWhile { it.isEmpty() }.toTypedArray()
                            vertices.add(Vertex(
                                xyzBuffer[linePartParts[0].toInt() - 1],
                                uvBuffer[linePartParts[1].toInt() - 1],
                                normBuffer[linePartParts[2].toInt() - 1]
                            ))
                        }
                    }
                }
            }
            reader.close()
            file.close()
            var result: Array<Vertex> = Array(vertices.size) { Vertex() }
            result = vertices.toArray(result)
            result
        }
        catch (ignored: Exception) {
            println("File \"$filepath\" parseMesh() error")
            Array(0) { Vertex() }
        }
    }

    fun getImageSize(filePath: String): Int2 {
        val data = Array(3) { IntArray(1) }
        STBImage.stbi_info(filePath, data[0], data[1], data[2])
        return Int2(data[0][0], data[1][0])
    }

    fun getImageData(filepath: String, flipY: Boolean): ByteArray? {
        STBImage.stbi_set_flip_vertically_on_load(flipY)
        val ignored = IntArray(1)
        STBImage.stbi_load(filepath, ignored, ignored, ignored, 4)?.let {
            val data = Memory.readByteBuffer(it)
            STBImage.stbi_image_free(it)
            return data
        }
        return null
    }
}