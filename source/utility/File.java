package utility;

import math.*;

import java.io.*;
import java.nio.file.*;
import java.util.*;

import static org.lwjgl.opengl.GL46.*;

public class File {
    public static String read(String filePath) throws Exception {
        return Files.readString(Path.of(filePath));
    }

    private static String getShaderDeclaration(int type) {
        return switch (type) {
            case GL_VERTEX_SHADER ->  "//vertexshader";
            case GL_FRAGMENT_SHADER ->  "//fragmentshader";
            case GL_GEOMETRY_SHADER ->  "//geometryshader";
            case GL_COMPUTE_SHADER ->  "//computeshader";
            default -> "";
        };
    }

    private static boolean isShaderDeclaration(String line) {
        return switch (line.toLowerCase().replaceAll("\\s", "")) {
            case "//vertexshader", "//fragmentshader", "//geometryshader", "//computeshader" -> true;
            default -> false;
        };
    }

    public static String parseShader(String filePath, int type) throws Exception {
        FileReader file = new FileReader(filePath);
        BufferedReader reader = new BufferedReader(file);
        StringBuilder builder = new StringBuilder();
        boolean shouldSave = false;
        final String neededDeclaration = getShaderDeclaration(type);
        for (String fileLine; (fileLine = reader.readLine()) != null;) {
            if (shouldSave) {
                if (!isShaderDeclaration(fileLine)) {
                    builder.append(fileLine).append('\n');
                }
                else {
                    break;
                }
            }
            else if (fileLine.toLowerCase().replaceAll("\\s", "").equals(neededDeclaration)) {
                shouldSave = true;
            }
        }
        reader.close();
        file.close();
        return builder.toString();
    }

    public static Vertex[] parseMeshFile(String objFilePath) throws Exception {
        FileReader file = new FileReader(objFilePath);
        BufferedReader reader = new BufferedReader(file);

        ArrayList<Vertex> vertices = new ArrayList<>();
        ArrayList<Float3> xyzBuffer = new ArrayList<>();
        ArrayList<Float2> uvBuffer = new ArrayList<>();
        ArrayList<Float3> normBuffer = new ArrayList<>();

        for (String fileLine; (fileLine = reader.readLine()) != null;) {
            String[] lineParts = fileLine.split(" ");

            // Parsing the data
            if (Objects.equals(lineParts[0], "v")) {
                xyzBuffer.add(new Float3(Float.parseFloat(lineParts[1]), Float.parseFloat(lineParts[2]), Float.parseFloat(lineParts[3]) * -1));
            }
            else if (Objects.equals(lineParts[0], "vt")) {
                uvBuffer.add(new Float2(Float.parseFloat(lineParts[1]), Float.parseFloat(lineParts[2])));
            }
            else if (Objects.equals(lineParts[0], "vn")) {
                normBuffer.add(new Float3(Float.parseFloat(lineParts[1]), Float.parseFloat(lineParts[2]), Float.parseFloat(lineParts[3]) * -1));
            }
            else if (Objects.equals(lineParts[0], "f")) {
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

        Vertex[] verts = new Vertex[vertices.size()];
        verts = vertices.toArray(verts);

        return verts;
    }
}
