package gui.section.script

internal object LanguageData {
    val keywords: Array<String>
        get() = arrayOf(
            "do", "if", "for", "new", "int", "try", "null",
            "else", "case", "enum", "this", "byte", "char",
            "void", "long", "throw", "catch", "final", "break",
            "class", "short", "float", "super", "while", "assert",
            "static", "return", "double", "import", "public", "native",
            "throws", "switch", "finally", "boolean", "extends", "default",
            "package", "private", "abstract", "continue", "strictfp", "volatile",
            "interface", "transient", "protected", "instanceof", "implements", "synchronized",
            "self", "true", "false"
        )

    val identifiers: Array<String>
        get() = arrayOf(
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
            "Window", "Input", "Key", "Mouse"
        )
}