package gui;

import callback.EmptyCallback;
import imgui.ImGui;
import math.*;

public final class GUIUtil {
    private GUIUtil() {}

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
}
