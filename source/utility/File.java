package utility;

import math.*;
import org.lwjgl.stb.STBImage;

import java.io.*;
import java.nio.ByteBuffer;
import java.nio.file.*;
import java.util.*;

import static org.lwjgl.opengl.GL33.*;

public final class File {
    private File() {}

    public static String read(String filePath) {
        try {
            return Files.readString(Path.of(filePath));
        }
        catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    private static String getShaderDeclaration(int type) {
        return switch (type) {
            case GL_VERTEX_SHADER -> "//vertexshader";
            case GL_FRAGMENT_SHADER -> "//fragmentshader";
            case GL_GEOMETRY_SHADER -> "//geometryshader";
            default -> null;
        };
    }

    private static boolean isShaderDeclaration(String line) {
        return switch (line.toLowerCase().replaceAll("\\s", "")) {
            case "//vertexshader", "//fragmentshader", "//geometryshader" -> true;
            default -> false;
        };
    }

    public static String parseShader(String filePath, int type) {
        try {
            FileReader file = new FileReader(filePath);
            BufferedReader reader = new BufferedReader(file);
            StringBuilder builder = new StringBuilder();

            boolean shouldSave = false;
            final String neededDeclaration = getShaderDeclaration(type);
            for (String fileLine; (fileLine = reader.readLine()) != null; ) {
                if (shouldSave) {
                    if (!isShaderDeclaration(fileLine)) {
                        builder.append(fileLine).append('\n');
                    } else {
                        break;
                    }
                } else if (fileLine.toLowerCase().replaceAll("\\s", "").equals(neededDeclaration)) {
                    shouldSave = true;
                }
            }

            reader.close();
            file.close();

            return builder.toString();
        }
        catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public static Vertex[] parseMeshFile(String filepath) {
        try {
            FileReader file = new FileReader(filepath);
            BufferedReader reader = new BufferedReader(file);

            ArrayList<Vertex> vertices = new ArrayList<>();
            ArrayList<Float3> xyzBuffer = new ArrayList<>();
            ArrayList<Float2> uvBuffer = new ArrayList<>();
            ArrayList<Float3> normBuffer = new ArrayList<>();

            for (String fileLine; (fileLine = reader.readLine()) != null; ) {
                String[] lineParts = fileLine.split(" ");

                if (Objects.equals(lineParts[0], "v")) {
                    xyzBuffer.add(new Float3(Float.parseFloat(lineParts[1]), Float.parseFloat(lineParts[2]), Float.parseFloat(lineParts[3])));
                } else if (Objects.equals(lineParts[0], "vt")) {
                    uvBuffer.add(new Float2(Float.parseFloat(lineParts[1]), Float.parseFloat(lineParts[2])));
                } else if (Objects.equals(lineParts[0], "vn")) {
                    normBuffer.add(new Float3(Float.parseFloat(lineParts[1]), Float.parseFloat(lineParts[2]), Float.parseFloat(lineParts[3])));
                } else if (Objects.equals(lineParts[0], "f")) {
                    for (int i = 1; i < 4; i++) {
                        String[] linePartParts = lineParts[i].split("/");
                        vertices.add(
                                new Vertex(
                                        xyzBuffer.get(Integer.parseInt(linePartParts[0]) - 1),
                                        uvBuffer.get(Integer.parseInt(linePartParts[1]) - 1),
                                        normBuffer.get(Integer.parseInt(linePartParts[2]) - 1)
                                )
                        );
                    }
                }
            }

            reader.close();
            file.close();

            Vertex[] result = new Vertex[vertices.size()];
            result = vertices.toArray(result);

            return result;
        }
        catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public static Int2 getImageSize(String filePath) {
        int[] width = new int[1];
        int[] height = new int[1];
        int[] dontCare = new int[1];

        STBImage.stbi_info(filePath, width, height, dontCare);

        return new Int2(width[0], height[0]);
    }

    public static byte[] getImageData(String filepath) {
        return getImageData(filepath, true);
    }

    public static byte[] getImageData(String filepath, boolean flipOnY) {
        STBImage.stbi_set_flip_vertically_on_load(flipOnY);

        int[] dontCare = new int[1];
        ByteBuffer imageBuffer = STBImage.stbi_load(filepath, dontCare, dontCare, dontCare, 4);

        if (imageBuffer != null) {
            byte[] imageData = Memory.readByteBuffer(imageBuffer);
            STBImage.stbi_image_free(imageBuffer);
            return imageData;
        }

        return null;
    }
}
