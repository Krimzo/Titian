package gui.section;

import editor.Editor;
import gui.abs.GUISection;
import gui.helper.GUIDragDrop;
import imgui.ImGui;
import imgui.extension.texteditor.TextEditor;
import imgui.extension.texteditor.TextEditorLanguageDefinition;
import imgui.flag.ImGuiStyleVar;
import imgui.flag.ImGuiWindowFlags;
import utility.Files;
import utility.Instance;
import window.input.Input;
import window.input.Key;

import java.awt.*;
import java.util.HashMap;
import java.util.Map;

class ColorPalette {
    private ColorPalette() {}

    public static final int Default = 0;
    public static final int Keyword = 1;
    public static final int Number = 2;
    public static final int String = 3;
    public static final int CharLiteral = 4;
    public static final int Punctuation = 5;
    public static final int Preprocessor = 6;
    public static final int Identifier = 7;
    public static final int KnownIdentifier = 8;
    public static final int PreprocIdentifier = 9;
    public static final int Comment = 10;
    public static final int MultiLineComment = 11;
    public static final int Background = 12;
    public static final int Cursor = 13;
    public static final int Selection = 14;
    public static final int ErrorMarker = 15;
    public static final int Breakpoint = 16;
    public static final int LineNumber = 17;
    public static final int CurrentLineFill = 18;
    public static final int CurrentLineFillInactive = 19;
    public static final int CurrentLineEdge = 20;

    public static final int MAX = 21;

    public static int convertColor(Color color) {
        return new Color(color.getBlue(), color.getGreen(), color.getRed(), color.getAlpha()).getRGB();
    }

    public static int[] getCustomDark(TextEditor textEditor) {
        final int[] result = textEditor.getDarkPalette();
        result[Keyword] = convertColor(new Color(240, 155, 120));
        result[Number] = convertColor(new Color(200, 95, 95));
        result[String] = convertColor(new Color(211, 158, 104));
        result[CharLiteral] = convertColor(new Color(204, 170, 135));
        result[Punctuation] = convertColor(new Color(225, 225, 225));
        result[Preprocessor] = convertColor(new Color(224, 179, 215));
        result[Identifier] = convertColor(new Color(210, 210, 210));
        result[KnownIdentifier] = convertColor(new Color(105, 210, 190));
        result[PreprocIdentifier] = convertColor(new Color(210, 178, 203));
        result[Comment] = convertColor(new Color(128, 128, 128));
        result[MultiLineComment] = convertColor(new Color(116, 116, 116));
        result[Background] = convertColor(new Color(25, 25, 25));
        result[Cursor] = convertColor(new Color(220, 220, 220));
        result[Selection] = convertColor(new Color(70, 70, 70));
        result[ErrorMarker] = convertColor(new Color(196, 57, 57));
        result[Breakpoint] = convertColor(new Color(222, 73, 73));
        result[LineNumber] = convertColor(new Color(210, 210, 210));
        result[CurrentLineFill] = convertColor(new Color(51, 51, 51, 10));
        result[CurrentLineFillInactive] = convertColor(new Color(51, 51, 51, 20));
        result[CurrentLineEdge] = convertColor(new Color(89, 89, 89));
        return result;
    }
}

class LanguageData {
    private LanguageData() {}

    public static String[] getKeywords() {
        return new String[] {
            "do", "if", "for", "new", "int", "try", "null",
            "else", "case", "enum", "this", "byte", "char",
            "void", "long", "throw", "catch", "final", "break",
            "class", "short", "float", "super", "while", "assert",
            "static", "return", "double", "import", "public", "native",
            "throws", "switch", "finally", "boolean", "extends", "default",
            "package", "private", "abstract", "continue", "strictfp", "volatile",
            "interface", "transient", "protected", "instanceof", "implements", "synchronized",
        };
    }

    public static String[] getIdentifiers() {
        return new String[] {
            "Override", "Serial",
            "Math", "Time",
            "String", "Color",
            "Int2", "Int3", "Int4",
            "Float2", "Float3", "Float4",
            "Matrix", "Mat3", "Mat4",
            "Ray", "Plane", "Sphere", "Vertex", "Triangle",
            "Entity", "Mesh", "Texture", "Material",
            "Camera", "PerspectiveCamera", "OrthographicCamera",
            "Light", "AmbientLight", "DirectionalLight", "PositionalLight",
            "Scene", "Scriptable",
            "Window", "Input", "Key", "Mouse",
        };
    }
}

public final class GUIScriptEditor extends GUISection {
    private final TextEditor textEditor = new TextEditor();
    private final int[] darkPalette;

    private String filepath = null;
    private float lastMenuTextSize = 0;

    public GUIScriptEditor(Editor editor) {
        super(editor);

        TextEditorLanguageDefinition languageDefinition = TextEditorLanguageDefinition.cPlusPlus();

        languageDefinition.setName("Java");
        languageDefinition.setKeywords(LanguageData.getKeywords());

        Map<String, String> identifiers = new HashMap<>();
        for (String val : LanguageData.getIdentifiers()) {
            identifiers.put(val, "");
        }
        languageDefinition.setIdentifiers(identifiers);

        textEditor.setLanguageDefinition(languageDefinition);

        darkPalette = ColorPalette.getCustomDark(textEditor);
        textEditor.setPalette(darkPalette);

        textEditor.setTabSize(4);
        textEditor.setShowWhitespaces(false);
        textEditor.setColorizerEnable(true);
    }

    private void saveFileText() {
        Files.writeString(filepath, textEditor.getText());
    }

    private void loadFileText() {
        textEditor.setText(Files.readString(filepath));
    }

    private void handleInput() {
        if (Input.isCtrlDown()) {
            if (Input.isKeyPressed(Key.S)) {
                saveFileText();
            }
            if (Input.isKeyPressed(Key.R)) {
                loadFileText();
            }
        }
    }

    private void renderMenu() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 10, 10);

        if (ImGui.beginMenuBar()) {
            if (ImGui.beginMenu("File")) {
                if (ImGui.menuItem("Save", "Ctrl-R")) {
                    saveFileText();
                }

                if (ImGui.menuItem("Reload", "Ctrl-R")) {
                    loadFileText();
                }

                ImGui.endMenu();
            }

            if (ImGui.beginMenu("Edit")) {
                if (ImGui.menuItem("Undo", "Ctrl-Z", false, textEditor.canUndo())) {
                    textEditor.undo(1);
                }
                if (ImGui.menuItem("Redo", "Ctrl-Y", false, textEditor.canRedo())) {
                    textEditor.redo(1);
                }

                ImGui.separator();

                if (ImGui.menuItem("Copy", "Ctrl-C", false, textEditor.hasSelection())) {
                    textEditor.copy();
                }
                if (ImGui.menuItem("Cut", "Ctrl-X", false, textEditor.hasSelection())) {
                    textEditor.cut();
                }
                if (ImGui.menuItem("Paste", "Ctrl-V", false, ImGui.getClipboardText() != null)) {
                    textEditor.paste();
                }

                ImGui.endMenu();
            }

            if (ImGui.beginMenu("View")) {
                if (ImGui.menuItem("Dark palette")) {
                    textEditor.setPalette(darkPalette);
                }
                if (ImGui.menuItem("Light palette")) {
                    textEditor.setPalette(textEditor.getLightPalette());
                }

                ImGui.endMenu();
            }

            ImGui.setCursorPosX(ImGui.getWindowWidth() * 0.5f - lastMenuTextSize * 0.5f);
            ImGui.text(Instance.isValid(this.filepath) ? this.filepath : "Unknown");
            lastMenuTextSize = ImGui.getItemRectSizeX();

            ImGui.endMenuBar();
        }

        ImGui.popStyleVar();
    }

    private void renderTextEditor() {
        ImGui.pushFont(editor.guiRenderer.fonts.scriptEditorFont);

        textEditor.render("Script Editor");

        GUIDragDrop.getData("ScriptFile", filepath -> {
            this.filepath = (String) filepath;
            loadFileText();
        });

        ImGui.popFont();
    }

    @Override
    public void renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 1, 1);

        if (ImGui.begin("Script Editor##Window", ImGuiWindowFlags.MenuBar)) {
            handleInput();
            renderMenu();
            renderTextEditor();
        }
        ImGui.end();

        ImGui.popStyleVar();
    }
}