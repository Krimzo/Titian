package gui.helper

import imgui.ImGui
import imgui.flag.ImGuiInputTextFlags
import imgui.type.ImString
import math.*
import script.abs.Scriptable
import java.awt.Color
import java.lang.reflect.Field

object GUIEdit {
    fun editInt(name: String, data: Int, precision: Int): Int {
        val dataArray = intArrayOf(data)
        ImGui.dragInt(name, dataArray, precision.toFloat())
        return dataArray[0]
    }

    fun editInt(name: String, data: Int, precision: Int, min: Int, max: Int): Int {
        val dataArray = intArrayOf(data)
        ImGui.dragInt(name, dataArray, precision.toFloat(), min.toFloat(), max.toFloat())
        return dataArray[0]
    }

    fun editFloat(name: String, data: Float, precision: Float): Float {
        val dataArray = floatArrayOf(data)
        ImGui.dragFloat(name, dataArray, precision)
        return dataArray[0]
    }

    fun editFloat(name: String, data: Float, precision: Float, min: Float, max: Float): Float {
        val dataArray = floatArrayOf(data)
        ImGui.dragFloat(name, dataArray, precision, min, max)
        return dataArray[0]
    }

    fun editFloat2(name: String, data: Float2, precision: Float) {
        val dataArray = data.array
        if (ImGui.dragFloat2(name, dataArray, precision)) {
            data.x = dataArray[0]
            data.y = dataArray[1]
        }
    }

    fun editFloat2(name: String, data: Float2, precision: Float, min: Float, max: Float) {
        val dataArray = data.array
        if (ImGui.dragFloat2(name, dataArray, precision, min, max)) {
            data.x = dataArray[0]
            data.y = dataArray[1]
        }
    }

    fun editFloat3(name: String, data: Float3, precision: Float) {
        val dataArray = data.array
        if (ImGui.dragFloat3(name, dataArray, precision)) {
            data.x = dataArray[0]
            data.y = dataArray[1]
            data.z = dataArray[2]
        }
    }

    fun editFloat3(name: String, data: Float3, precision: Float, min: Float, max: Float) {
        val dataArray = data.array
        if (ImGui.dragFloat3(name, dataArray, precision, min, max)) {
            data.x = dataArray[0]
            data.y = dataArray[1]
            data.z = dataArray[2]
        }
    }

    fun editFloat4(name: String, data: Float4, precision: Float) {
        val dataArray = data.array
        if (ImGui.dragFloat4(name, dataArray, precision)) {
            data.x = dataArray[0]
            data.y = dataArray[1]
            data.z = dataArray[2]
            data.w = dataArray[3]
        }
    }

    fun editFloat4(name: String, data: Float4, precision: Float, min: Float, max: Float) {
        val dataArray = data.array
        if (ImGui.dragFloat4(name, dataArray, precision, min, max)) {
            data.x = dataArray[0]
            data.y = dataArray[1]
            data.z = dataArray[2]
            data.w = dataArray[3]
        }
    }

    fun editColor3(name: String, data: Float3) {
        val dataArray = data.array
        if (ImGui.colorEdit3(name, dataArray)) {
            data.x = dataArray[0]
            data.y = dataArray[1]
            data.z = dataArray[2]
        }
    }

    fun editColor4(name: String, data: Float4) {
        val dataArray = data.array
        if (ImGui.colorEdit4(name, dataArray)) {
            data.x = dataArray[0]
            data.y = dataArray[1]
            data.z = dataArray[2]
            data.w = dataArray[3]
        }
    }

    fun useColor3(styleType: Int, color: Float3, callback: () -> Unit) {
        ImGui.pushStyleColor(styleType, color.x, color.y, color.z, 1f)
        callback()
        ImGui.popStyleColor()
    }

    fun useColor4(styleType: Int, color: Float4, callback: () -> Unit) {
        ImGui.pushStyleColor(styleType, color.x, color.y, color.z, color.w)
        callback()
        ImGui.popStyleColor()
    }

    @Throws(Exception::class)
    fun editField(field: Field, instance: Scriptable) {
        when (val type = field.genericType.typeName) {
            "boolean" -> {
                val state = field.getBoolean(instance)
                if (ImGui.checkbox(field.name, state)) {
                    field.setBoolean(instance, !state)
                }
            }

            "byte" -> {
                val value = intArrayOf(field.getByte(instance).toInt())
                if (ImGui.dragInt(field.name, value)) {
                    field.setByte(instance, value[0].toByte())
                }
            }

            "short" -> {
                val value = intArrayOf(field.getShort(instance).toInt())
                if (ImGui.dragInt(field.name, value)) {
                    field.setShort(instance, value[0].toShort())
                }
            }

            "int" -> {
                val value = intArrayOf(field.getInt(instance))
                if (ImGui.dragInt(field.name, value)) {
                    field.setInt(instance, value[0])
                }
            }

            "long" -> {
                val value = intArrayOf(field.getLong(instance).toInt())
                if (ImGui.dragInt(field.name, value)) {
                    field.setLong(instance, value[0].toLong())
                }
            }

            "float" -> {
                val value = floatArrayOf(field.getFloat(instance))
                if (ImGui.dragFloat(field.name, value)) {
                    field.setFloat(instance, value[0])
                }
            }

            "double" -> {
                val value = floatArrayOf(field.getDouble(instance).toFloat())
                if (ImGui.dragFloat(field.name, value)) {
                    field.setDouble(instance, value[0].toDouble())
                }
            }

            "char" -> {
                val data = ImString("" + field.getChar(instance))
                if (ImGui.inputText(field.name, data, ImGuiInputTextFlags.EnterReturnsTrue)) {
                    val result = data.toString()
                    if (result.length == 1) {
                        field.setChar(instance, result[0])
                    }
                }
            }

            "math.Int2" -> {
                val obj = field[instance] as Int2?
                if (obj == null) {
                    ImGui.text("Field \"" + field.name + "\" is null")
                    return
                }
                val data = obj.array
                if (ImGui.dragInt2(field.name, data)) {
                    obj.x = data[0]
                    obj.y = data[1]
                }
            }

            "math.Int3" -> {
                val obj = field[instance] as Int3?
                if (obj == null) {
                    ImGui.text("Field \"" + field.name + "\" is null")
                    return
                }
                val data = obj.array
                if (ImGui.dragInt3(field.name, data)) {
                    obj.x = data[0]
                    obj.y = data[1]
                    obj.z = data[2]
                }
            }

            "math.Int4" -> {
                val obj = field[instance] as Int4?
                if (obj == null) {
                    ImGui.text("Field \"" + field.name + "\" is null")
                    return
                }
                val data = obj.array
                if (ImGui.dragInt4(field.name, data)) {
                    obj.x = data[0]
                    obj.y = data[1]
                    obj.z = data[2]
                    obj.w = data[3]
                }
            }

            "math.Float2" -> {
                val obj = field[instance] as Float2?
                if (obj == null) {
                    ImGui.text("Field \"" + field.name + "\" is null")
                    return
                }
                val data = obj.array
                if (ImGui.dragFloat2(field.name, data)) {
                    obj.x = data[0]
                    obj.y = data[1]
                }
            }

            "math.Float3" -> {
                val obj = field[instance] as Float3?
                if (obj == null) {
                    ImGui.text("Field \"" + field.name + "\" is null")
                    return
                }
                val data = obj.array
                if (ImGui.dragFloat3(field.name, data)) {
                    obj.x = data[0]
                    obj.y = data[1]
                    obj.z = data[2]
                }
            }

            "math.Float4" -> {
                val obj = field[instance] as Float4?
                if (obj == null) {
                    ImGui.text("Field \"" + field.name + "\" is null")
                    return
                }
                val data = obj.array
                if (ImGui.dragFloat4(field.name, data)) {
                    obj.x = data[0]
                    obj.y = data[1]
                    obj.z = data[2]
                    obj.w = data[3]
                }
            }

            "java.awt.Color" -> {
                val obj = field[instance] as Color?
                if (obj == null) {
                    ImGui.text("Field \"" + field.name + "\" is null")
                    return
                }
                val data = Float4(obj).array
                if (ImGui.colorEdit4(field.name, data)) {
                    field[instance] = Float4(data).color
                }
            }

            "java.lang.String" -> {
                val obj = field[instance] as String?
                if (obj == null) {
                    ImGui.text("Field \"" + field.name + "\" is null")
                    return
                }
                val data = ImString(obj, 50)
                if (ImGui.inputText(field.name, data, ImGuiInputTextFlags.EnterReturnsTrue)) {
                    field[instance] = data.toString()
                }
            }

            else -> {
                ImGui.text("Unknown type \"" + type + "\" for field \"" + field.name + "\"")
            }
        }
    }
}