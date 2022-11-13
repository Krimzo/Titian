package script;

import utility.helper.FileHelper;

import javax.tools.JavaCompiler;
import javax.tools.ToolProvider;

public class ScriptCompiler {
    private final JavaCompiler compiler;

    public ScriptCompiler() {
        compiler = ToolProvider.getSystemJavaCompiler();
    }

    public byte[] compile(String filepath) {
        try {
            if (compiler.run(null, null, null, filepath) == 0) {
                String compiledPath = FileHelper.getWithoutExtension(filepath) + ".class";
                byte[] data = FileHelper.readBytes(compiledPath);
                FileHelper.delete(compiledPath);
                return data;
            }
        }
        catch (Exception ignored) {
            System.out.println("Script \"" + filepath + "\" compilation error");
        }
        return null;
    }
}
