package gui;

import callback.EmptyCallback;
import imgui.ImGui;
import imgui.flag.ImGuiInputTextFlags;
import imgui.type.ImString;
import math.*;
import script.abs.Scriptable;

import java.awt.*;
import java.lang.reflect.Field;

public final class GUIEdit {
    private GUIEdit() {}

    public static int editInt(String name, int data, int precision) {
        int[] dataArray = { data };
        ImGui.dragInt(name, dataArray, precision);
        return dataArray[0];
    }

    public static int editInt(String name, int data, int precision, int min, int max) {
        int[] dataArray = { data };
        ImGui.dragInt(name, dataArray, precision, min, max);
        return dataArray[0];
    }

    public static float editFloat(String name, float data, float precision) {
        float[] dataArray = { data };
        ImGui.dragFloat(name, dataArray, precision);
        return dataArray[0];
    }

    public static float editFloat(String name, float data, float precision, float min, float max) {
        float[] dataArray = { data };
        ImGui.dragFloat(name, dataArray, precision, min, max);
        return dataArray[0];
    }

    public static void editFloat3(String name, Float3 data, float precision) {
        float[] dataArray = data.array();
        if (ImGui.dragFloat3(name, dataArray, precision)) {
            data.x = dataArray[0];
            data.y = dataArray[1];
            data.z = dataArray[2];
        }
    }

    public static void editFloat3(String name, Float3 data, float precision, float min, float max) {
        float[] dataArray = data.array();
        if (ImGui.dragFloat3(name, dataArray, precision, min, max)) {
            data.x = dataArray[0];
            data.y = dataArray[1];
            data.z = dataArray[2];
        }
    }

    public static void editFloat4(String name, Float4 data, float precision) {
        float[] dataArray = data.array();
        if (ImGui.dragFloat4(name, dataArray, precision)) {
            data.x = dataArray[0];
            data.y = dataArray[1];
            data.z = dataArray[2];
            data.w = dataArray[3];
        }
    }

    public static void editFloat4(String name, Float4 data, float precision, float min, float max) {
        float[] dataArray = data.array();
        if (ImGui.dragFloat4(name, dataArray, precision, min, max)) {
            data.x = dataArray[0];
            data.y = dataArray[1];
            data.z = dataArray[2];
            data.w = dataArray[3];
        }
    }

    public static void editColor3(String name, Float3 data) {
        float[] dataArray = data.array();
        if (ImGui.colorEdit3(name, dataArray)) {
            data.x = dataArray[0];
            data.y = dataArray[1];
            data.z = dataArray[2];
        }
    }

    public static void editColor4(String name, Float4 data) {
        float[] dataArray = data.array();
        if (ImGui.colorEdit4(name, dataArray)) {
            data.x = dataArray[0];
            data.y = dataArray[1];
            data.z = dataArray[2];
            data.w = dataArray[3];
        }
    }

    public static void useColor3(int styleType, Float3 color, EmptyCallback callback) {
        ImGui.pushStyleColor(styleType, color.x, color.y, color.z, 1);
        callback.method();
        ImGui.popStyleColor();
    }

    public static void useColor4(int styleType, Float4 color, EmptyCallback callback) {
        ImGui.pushStyleColor(styleType, color.x, color.y, color.z, color.w);
        callback.method();
        ImGui.popStyleColor();
    }

    public static void editField(Field field, Scriptable instance) throws Exception {
        String type = field.getGenericType().getTypeName();
        switch (type) {
            case "boolean" -> {
                boolean state = field.getBoolean(instance);
                if (ImGui.checkbox(field.getName(), state)) {
                    field.setBoolean(instance, !state);
                }
            }
            case "byte" -> {
                int[] value = { field.getByte(instance) };
                if (ImGui.dragInt(field.getName(), value)) {
                    field.setByte(instance, (byte) value[0]);
                }
            }
            case "short" -> {
                int[] value = { field.getShort(instance) };
                if (ImGui.dragInt(field.getName(), value)) {
                    field.setShort(instance, (short) value[0]);
                }
            }
            case "int" -> {
                int[] value = { field.getInt(instance) };
                if (ImGui.dragInt(field.getName(), value)) {
                    field.setInt(instance, value[0]);
                }
            }
            case "long" -> {
                int[] value = { (int) field.getLong(instance) };
                if (ImGui.dragInt(field.getName(), value)) {
                    field.setLong(instance, value[0]);
                }
            }
            case "float" -> {
                float[] value = { field.getFloat(instance) };
                if (ImGui.dragFloat(field.getName(), value)) {
                    field.setFloat(instance, value[0]);
                }
            }
            case "double" -> {
                float[] value = { (float) field.getDouble(instance) };
                if (ImGui.dragFloat(field.getName(), value)) {
                    field.setDouble(instance, value[0]);
                }
            }
            case "char" -> {
                ImString data = new ImString("" + field.getChar(instance));
                if (ImGui.inputText(field.getName(), data, ImGuiInputTextFlags.EnterReturnsTrue)) {
                    String result = data.toString();
                    if (result.length() == 1) {
                        field.setChar(instance, result.charAt(0));
                    }
                }
            }

            case "math.Int2" -> {
                Int2 object = (Int2) field.get(instance);
                if (object == null) {
                    ImGui.text("Field \"" + field.getName() + "\" is null");
                    return;
                }

                int[] data = object.array();
                if (ImGui.dragInt2(field.getName(), data)) {
                    object.set(data);
                }
            }
            case "math.Int3" -> {
                Int3 object = (Int3) field.get(instance);
                if (object == null) {
                    ImGui.text("Field \"" + field.getName() + "\" is null");
                    return;
                }

                int[] data = object.array();
                if (ImGui.dragInt3(field.getName(), data)) {
                    object.set(data);
                }
            }
            case "math.Int4" -> {
                Int4 object = (Int4) field.get(instance);
                if (object == null) {
                    ImGui.text("Field \"" + field.getName() + "\" is null");
                    return;
                }

                int[] data = object.array();
                if (ImGui.dragInt4(field.getName(), data)) {
                    object.set(data);
                }
            }
            case "math.Float2" -> {
                Float2 object = (Float2) field.get(instance);
                if (object == null) {
                    ImGui.text("Field \"" + field.getName() + "\" is null");
                    return;
                }

                float[] data = object.array();
                if (ImGui.dragFloat2(field.getName(), data)) {
                    object.set(data);
                }
            }
            case "math.Float3" -> {
                Float3 object = (Float3) field.get(instance);
                if (object == null) {
                    ImGui.text("Field \"" + field.getName() + "\" is null");
                    return;
                }

                float[] data = object.array();
                if (ImGui.dragFloat3(field.getName(), data)) {
                    object.set(data);
                }
            }
            case "math.Float4" -> {
                Float4 object = (Float4) field.get(instance);
                if (object == null) {
                    ImGui.text("Field \"" + field.getName() + "\" is null");
                    return;
                }

                float[] data = object.array();
                if (ImGui.dragFloat4(field.getName(), data)) {
                    object.set(data);
                }
            }
            case "java.awt.Color" -> {
                Color object = (Color) field.get(instance);
                if (object == null) {
                    ImGui.text("Field \"" + field.getName() + "\" is null");
                    return;
                }

                float[] data = new Float4(object).array();
                if (ImGui.colorEdit4(field.getName(), data)) {
                    field.set(instance, new Float4(data).color());
                }
            }
            case "java.lang.String" -> {
                String object = (String) field.get(instance);
                if (object == null) {
                    ImGui.text("Field \"" + field.getName() + "\" is null");
                    return;
                }

                ImString data = new ImString(object, 50);
                if (ImGui.inputText(field.getName(), data, ImGuiInputTextFlags.EnterReturnsTrue)) {
                    field.set(instance, data.toString());
                }
            }

            default -> {
                ImGui.text("Unknown type \"" + type + "\" for field \"" + field.getName() + "\"");
            }
        }
    }
}
