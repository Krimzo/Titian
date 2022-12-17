package window.input

import imgui.ImGui
import imgui.flag.ImGuiKey

object Key {
    const val Q: Int = 'Q'.code
    const val W: Int = 'W'.code
    const val E: Int = 'E'.code
    const val R: Int = 'R'.code
    const val T: Int = 'T'.code
    const val Z: Int = 'Z'.code
    const val U: Int = 'U'.code
    const val I: Int = 'I'.code
    const val O: Int = 'O'.code
    const val P: Int = 'P'.code
    const val A: Int = 'A'.code
    const val S: Int = 'S'.code
    const val D: Int = 'D'.code
    const val F: Int = 'F'.code
    const val G: Int = 'G'.code
    const val H: Int = 'H'.code
    const val J: Int = 'J'.code
    const val K: Int = 'K'.code
    const val L: Int = 'L'.code
    const val Y: Int = 'Y'.code
    const val X: Int = 'X'.code
    const val C: Int = 'C'.code
    const val V: Int = 'V'.code
    const val B: Int = 'B'.code
    const val N: Int = 'N'.code
    const val M: Int = 'M'.code

    const val Num0: Int = '0'.code
    const val Num1: Int = '1'.code
    const val Num2: Int = '2'.code
    const val Num3: Int = '3'.code
    const val Num4: Int = '4'.code
    const val Num5: Int = '5'.code
    const val Num6: Int = '6'.code
    const val Num7: Int = '7'.code
    const val Num8: Int = '8'.code
    const val Num9: Int = '9'.code

    const val Period: Int = '.'.code
    const val Comma: Int = ','.code
    const val Minus: Int = '-'.code
    const val Plus: Int = '+'.code

    val Esc: Int = ImGui.getKeyIndex(ImGuiKey.Escape)
    val Tab: Int = ImGui.getKeyIndex(ImGuiKey.Tab)
    val Space: Int = ImGui.getKeyIndex(ImGuiKey.Space)
    val Enter: Int = ImGui.getKeyIndex(ImGuiKey.Enter)

    val Insert: Int = ImGui.getKeyIndex(ImGuiKey.Insert)
    val Delete: Int = ImGui.getKeyIndex(ImGuiKey.Delete)
    val Backspace: Int = ImGui.getKeyIndex(ImGuiKey.Backspace)

    val PageUp: Int = ImGui.getKeyIndex(ImGuiKey.PageUp)
    val PageDown: Int = ImGui.getKeyIndex(ImGuiKey.PageDown)

    val Up: Int = ImGui.getKeyIndex(ImGuiKey.UpArrow)
    val Down: Int = ImGui.getKeyIndex(ImGuiKey.DownArrow)
    val Left: Int = ImGui.getKeyIndex(ImGuiKey.LeftArrow)
    val Right: Int = ImGui.getKeyIndex(ImGuiKey.RightArrow)

/* UNKNOWN
    public static final int Caps = 595;

    public static final int F1 = 572;
    public static final int F2 = 573;
    public static final int F3 = 574;
    public static final int F4 = 575;
    public static final int F5 = 576;
    public static final int F6 = 577;
    public static final int F7 = 578;
    public static final int F8 = 579;
    public static final int F9 = 580;
    public static final int F10 = 581;
    public static final int F11 = 582;
    public static final int F12 = 583;
*/
}