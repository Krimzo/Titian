package utility;

import math.Float2;
import math.Float3;
import math.Int2;
import math.Vertex;
import org.lwjgl.stb.STBImage;
import utility.nncollection.NNArrayList;

import java.io.*;
import java.nio.ByteBuffer;
import java.nio.file.Path;

import static org.lwjgl.opengl.GL33.*;

public final class Files {
    private Files() {}

    public static boolean isEmpty(String filepath) {
        try {
            File[] files = new File(filepath).listFiles();
            return files == null || files.length == 0;
        }
        catch (Exception ignored) {
            System.out.println("File \"" + filepath + "\" isEmpty() error");
            return false;
        }
    }

    public static boolean delete(String filepath) {
        try {
            return new File(filepath).delete();
        }
        catch (Exception ignored) {
            System.out.println("File \"" + filepath + "\" delete() error");
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
        try (FileOutputStream stream = new FileOutputStream(filepath)) {
            stream.write(data.getBytes());
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
            System.out.println("File \"" + filepath + "\" getName() error");
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
            return (folders != null) ? folders : new File[0];
        }
        catch (Exception ignored) {
            System.out.println("Path \"" + filepath + "\" listFolders() error");
            return null;
        }
    }

    public static File[] listFiles(String filepath) {
        try {
            File[] files = new File(filepath).listFiles(File::isFile);
            return (files != null) ? files : new File[0];
        }
        catch (Exception ignored) {
            System.out.println("Path \"" + filepath + "\" listFiles() error");
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
                default -> null;
            };

            for (String line; (line = reader.readLine()) != null;) {
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

            for (String fileLine; (fileLine = reader.readLine()) != null; ) {
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

        if (buffer != null) {
            byte[] data = Memory.readByteBuffer(buffer);
            STBImage.stbi_image_free(buffer);
            return data;
        }

        return null;
    }
}
