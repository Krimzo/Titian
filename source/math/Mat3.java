package math;

import java.io.Serializable;

public class Mat3 implements Serializable {
    public final float[] data = new float[9];

    public Mat3() {
        data[0] = 1.0f; data[1] = 0.0f; data[2] = 0.0f;
        data[3] = 0.0f; data[4] = 1.0f; data[5] = 0.0f;
        data[6] = 0.0f; data[7] = 0.0f; data[8] = 1.0f;
    }

    public Mat3(Mat3 m) {
        System.arraycopy(m.data, 0, data, 0, data.length);
    }

    // Getter
    public float get(int x, int y) {
        return data[y * 3 + x];
    }

    // Addition
    public Mat3 add(Mat3 m) {
        Mat3 temp = new Mat3();
        for(int i = 0; i < 9; i++) {
            temp.data[i] = data[i] + m.data[i];
        }
        return temp;
    }

    // Subtraction
    public Mat3 sub(Mat3 m) {
        Mat3 temp = new Mat3();
        for(int i = 0; i < 9; i++) {
            temp.data[i] = data[i] - m.data[i];
        }
        return temp;
    }

    // Multiplication
    public Mat3 mul(float a) {
        Mat3 temp = new Mat3();
        for(int i = 0; i < 9; i++) {
            temp.data[i] = data[i] * a;
        }
        return temp;
    }
    public Float3 mul(Float3 v) {
        Float3 temp = new Float3();
        temp.x = data[0] * v.x + data[1] * v.y + data[2] * v.z;
        temp.y = data[3] * v.x + data[4] * v.y + data[5] * v.z;
        temp.z = data[6] * v.x + data[7] * v.y + data[8] * v.z;
        return temp;
    }
    public Mat3 mul(Mat3 m) {
        Mat3 temp = new Mat3();
        temp.data[0] = data[0] * m.data[0] + data[1] * m.data[3] + data[2] * m.data[6];
        temp.data[1] = data[0] * m.data[1] + data[1] * m.data[4] + data[2] * m.data[7];
        temp.data[2] = data[0] * m.data[2] + data[1] * m.data[5] + data[2] * m.data[8];
        temp.data[3] = data[3] * m.data[0] + data[4] * m.data[3] + data[5] * m.data[6];
        temp.data[4] = data[3] * m.data[1] + data[4] * m.data[4] + data[5] * m.data[7];
        temp.data[5] = data[3] * m.data[2] + data[4] * m.data[5] + data[5] * m.data[8];
        temp.data[6] = data[6] * m.data[0] + data[7] * m.data[3] + data[8] * m.data[6];
        temp.data[7] = data[6] * m.data[1] + data[7] * m.data[4] + data[8] * m.data[7];
        temp.data[8] = data[6] * m.data[2] + data[7] * m.data[5] + data[8] * m.data[8];
        return temp;
    }

    // Division
    public Mat3 div(float a) {
        return mul(1.0f / a);
    }

    // Comparison
    public boolean equals(Mat3 m) {
        for (int i = 0; i < 9; i++) {
            if (data[i] != m.data[i]) {
                return false;
            }
        }
        return true;
    }

    // Inverse
    public Mat3 inv() {
        Mat3 temp = new Mat3();
        temp.data[0] = data[4] * data[8] - data[7] * data[5];
        temp.data[1] = data[2] * data[7] - data[1] * data[8];
        temp.data[2] = data[1] * data[5] - data[2] * data[4];
        temp.data[3] = data[5] * data[6] - data[3] * data[8];
        temp.data[4] = data[0] * data[8] - data[2] * data[6];
        temp.data[5] = data[3] * data[2] - data[0] * data[5];
        temp.data[6] = data[3] * data[7] - data[6] * data[4];
        temp.data[7] = data[6] * data[1] - data[0] * data[7];
        temp.data[8] = data[0] * data[4] - data[3] * data[1];

        // Det calc
	    final float det = data[0] * (data[4] * data[8] - data[7] * data[5]) -
                data[1] * (data[3] * data[8] - data[5] * data[6]) +
                data[2] * (data[3] * data[7] - data[4] * data[6]);

        // Return
        return temp.div(det);
    }

    // Translation matrix
    public static Mat3 translation(Float2 translation) {
        Mat3 temp = new Mat3();
        temp.data[2] = translation.x;
        temp.data[5] = translation.y;
        return temp;
    }

    // Rotation matrix
    public static Mat3 rotation(float rotation) {
        // Computing trig
	    final float zSin = (float)Math.sin(Math.toRadians(rotation));
	    final float zCos = (float)Math.cos(Math.toRadians(rotation));

        // Generating the mat
        Mat3 temp = new Mat3();
        temp.data[0] =  zCos;
        temp.data[1] = -zSin;
        temp.data[3] =  zSin;
        temp.data[4] =  zCos;
        return temp;
    }

    // Scaling matrix
    public static Mat3 scaling(Float2 size) {
        Mat3 temp = new Mat3();
        temp.data[0] = size.x;
        temp.data[4] = size.y;
        return temp;
    }

    // String
    public String toString() {
        return  "[" + data[0] + " " + data[1] + " " + data[2] + "]\n" +
                "[" + data[3] + " " + data[4] + " " + data[5] + "]\n" +
                "[" + data[6] + " " + data[7] + " " + data[8] + "]";
    }
}
