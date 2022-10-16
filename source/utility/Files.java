package utility;

import math.Float2;
import math.Float3;
import math.Int2;
import math.Vertex;
import org.lwjgl.stb.STBImage;
import utility.nncollection.NNArrayList;

import java.awt.*;
import java.io.*;
import java.nio.ByteBuffer;
import java.nio.file.Path;
import java.nio.file.Paths;

import static org.lwjgl.opengl.GL33.*;

public final class Files {
    private Files() {}

    public static final String separator = System.getProperty("file.separator");

    public static String defaultPath() {
        return Paths.get("").toFile().getAbsolutePath();
    }

    public static boolean isEmpty(String filepath) {
        try {
            File[] files = new File(filepath).listFiles();
            return Instance.isNull(files) || files.length == 0;
        }
        catch (Exception ignored) {
            System.out.println("Folder \"" + filepath + "\" isEmpty() error");
            return false;
        }
    }

    public static boolean delete(String filepath) {
        try {
            return new File(filepath).delete();
        }
        catch (Exception ignored) {
            System.out.println("File/folder \"" + filepath + "\" delete() error");
            return false;
        }
    }

    public static boolean createFile(String filepath) {
        try {
            File file = new File(filepath);
            boolean ignored = file.getParentFile().mkdirs();
            return file.createNewFile();
        }
        catch (Exception ignored) {
            System.out.println("File/folder \"" + filepath + "\" create() error");
            return false;
        }
    }

    public static boolean createFolder(String filepath) {
        try {
            return new File(filepath).mkdirs();
        }
        catch (Exception ignored) {
            System.out.println("File/folder \"" + filepath + "\" create() error");
            return false;
        }
    }

    public static boolean rename(String filepath, String name) {
        try {
            File file = new File(filepath);
            String newPath = file.getParent() + "/" + name;
            return file.renameTo(new File(newPath));
        }
        catch (Exception ignored) {
            System.out.println("File/folder \"" + filepath + "\" rename() error");
            return false;
        }
    }

    public static boolean move(String filepath, String toFolder) {
        try {
            File from = new File(filepath);
            String to = toFolder + "/" + from.getName();
            return from.renameTo(new File(to));
        }
        catch (Exception ignored) {
            System.out.println("File/folder \"" + filepath + "\" move() error");
            return false;
        }
    }

    public static boolean openDefault(String filepath) {
        try {
            Desktop.getDesktop().open(new File(filepath));
            return true;
        }
        catch (Exception ignored) {
            System.out.println("File \"" + filepath + "\" openDefault() error");
            return false;
        }
    }

    public static String readString(String filepath) {
        try {
            return java.nio.file.Files.readString(Path.of(filepath));
        }
        catch (Exception ignored) {
            System.out.println("File \"" + filepath + "\" readString() error");
            return "";
        }
    }

    public static boolean writeString(String filepath, String data) {
        try {
            File file = new File(filepath);
            boolean ignored = file.getParentFile().mkdirs();
            try (FileOutputStream stream = new FileOutputStream(file)) {
                stream.write(data.getBytes());
            }
            return true;
        }
        catch (Exception ignored) {
            System.out.println("File \"" + filepath + "\" writeString() error");
            return false;
        }
    }

    public static byte[] readBytes(String filepath) {
        try (FileInputStream stream = new FileInputStream(filepath)) {
            return stream.readAllBytes();
        }
        catch (Exception ignored) {
            System.out.println("File \"" + filepath + "\" readBytes() error");
            return null;
        }
    }

    public static String getName(String filepath) {
        try {
            return new File(filepath).getName();
        }
        catch (Exception ignored) {
            System.out.println("File/folder \"" + filepath + "\" getName() error");
            return "";
        }
    }

    public static String getExtension(String filepath) {
        final int index = filepath.lastIndexOf('.');
        return (index >= 0) ? filepath.substring(index + 1) : "";
    }

    public static String getWithoutExtension(String filepath) {
        final int index = filepath.indexOf('.');
        if (index > 0) {
            return filepath.substring(0, index);
        }
        if (index < 0) {
            return filepath;
        }
        return "";
    }

    public static String getNameWithoutExtension(String filepath) {
        return getWithoutExtension(getName(filepath));
    }

    public static File[] listFolders(String filepath) {
        try {
            File[] folders = new File(filepath).listFiles(File::isDirectory);
            return Instance.isValid(folders) ? folders : new File[0];
        }
        catch (Exception ignored) {
            System.out.println("Folder \"" + filepath + "\" listFolders() error");
            return null;
        }
    }

    public static File[] listFiles(String filepath) {
        try {
            File[] files = new File(filepath).listFiles(File::isFile);
            return Instance.isValid(files) ? files : new File[0];
        }
        catch (Exception ignored) {
            System.out.println("Folder \"" + filepath + "\" listFiles() error");
            return null;
        }
    }

    public static String parseShader(String filepath, int type) {
        try {
            FileReader file = new FileReader(filepath);
            BufferedReader reader = new BufferedReader(file);
            StringBuilder builder = new StringBuilder();

            boolean shouldSave = false;
            String shaderName = switch (type) {
                case GL_VERTEX_SHADER -> "//vertexshader";
                case GL_FRAGMENT_SHADER -> "//fragmentshader";
                case GL_GEOMETRY_SHADER -> "//geometryshader";
                default -> "";
            };

            for (String line; Instance.isValid(line = reader.readLine());) {
                String formattedLine = line.toLowerCase().replaceAll("\\s", "");
                if (shouldSave) {
                    if (switch (formattedLine) {
                        case "//vertexshader", "//fragmentshader", "//geometryshader" -> false;
                        default -> true;
                    }) {
                        builder.append(line).append('\n');
                    }
                    else {
                        break;
                    }
                }
                else if (formattedLine.equals(shaderName)) {
                    shouldSave = true;
                }
            }

            reader.close();
            file.close();

            return builder.toString();
        }
        catch (Exception ignored) {
            System.out.println("File \"" + filepath + "\" parseShaders() error");
            return null;
        }
    }

    public static Vertex[] parseMeshFile(String filepath) {
        try {
            FileReader file = new FileReader(filepath);
            BufferedReader reader = new BufferedReader(file);

            NNArrayList<Vertex> vertices = new NNArrayList<>();
            NNArrayList<Float3> xyzBuffer = new NNArrayList<>();
            NNArrayList<Float2> uvBuffer = new NNArrayList<>();
            NNArrayList<Float3> normBuffer = new NNArrayList<>();

            for (String fileLine; Instance.isValid(fileLine = reader.readLine()); ) {
                String[] lineParts = fileLine.split(" ");

                switch (lineParts[0]) {
                    case "v" -> {
                        xyzBuffer.add(new Float3(
                            Float.parseFloat(lineParts[1]),
                            Float.parseFloat(lineParts[2]),
                            Float.parseFloat(lineParts[3])
                        ));
                    }
                    case "vt" -> {
                        uvBuffer.add(new Float2(
                            Float.parseFloat(lineParts[1]),
                            Float.parseFloat(lineParts[2])
                        ));
                    }
                    case "vn" -> {
                        normBuffer.add(new Float3(
                            Float.parseFloat(lineParts[1]),
                            Float.parseFloat(lineParts[2]),
                            Float.parseFloat(lineParts[3])
                        ));
                    }
                    case "f" -> {
                        for (int i = 1; i < 4; i++) {
                            String[] linePartParts = lineParts[i].split("/");
                            vertices.add(new Vertex(
                                xyzBuffer.get(Integer.parseInt(linePartParts[0]) - 1),
                                uvBuffer.get(Integer.parseInt(linePartParts[1]) - 1),
                                normBuffer.get(Integer.parseInt(linePartParts[2]) - 1)
                            ));
                        }
                    }
                }
            }

            reader.close();
            file.close();

            Vertex[] result = new Vertex[vertices.size()];
            result = vertices.toArray(result);

            return result;
        }
        catch (Exception ignored) {
            System.out.println("File \"" + filepath + "\" parseMesh() error");
            return null;
        }
    }

    public static Int2 getImageSize(String filePath) {
        int[][] data = new int[3][1];
        STBImage.stbi_info(filePath, data[0], data[1], data[2]);
        return new Int2(data[0][0], data[1][0]);
    }

    public static byte[] getImageData(String filepath, boolean flipY) {
        STBImage.stbi_set_flip_vertically_on_load(flipY);

        int[] ignored = new int[1];
        ByteBuffer buffer = STBImage.stbi_load(filepath, ignored, ignored, ignored, 4);

        if (Instance.isValid(buffer)) {
            byte[] data = Memory.readByteBuffer(buffer);
            STBImage.stbi_image_free(buffer);
            return data;
        }

        return null;
    }
}
