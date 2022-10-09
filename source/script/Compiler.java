package script;

import utility.Files;

import javax.tools.*;

public class Compiler {
    private final JavaCompiler compiler;

    public Compiler() {
        compiler = ToolProvider.getSystemJavaCompiler();
    }

    public byte[] compile(String filepath) {
        try {
            if (compiler.run(null, null, null, filepath) == 0) {
                String compiledPath = Files.getWithoutExtension(filepath) + ".class";
                byte[] data = Files.readBytes(compiledPath);
                Files.delete(compiledPath);
                return data;
            }
        }
        catch (Exception ignored) {
            System.out.println("Script \"" + filepath + "\" compilation error");
        }
        return null;
    }
}
