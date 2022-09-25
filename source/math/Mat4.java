package math;

import java.io.Serializable;

public class Mat4 implements Serializable {
    public final float[] data = new float[16];

    public Mat4() {
        data[ 0] = 1.0f; data[ 1] = 0.0f; data[ 2] = 0.0f; data[ 3] = 0.0f;
        data[ 4] = 0.0f; data[ 5] = 1.0f; data[ 6] = 0.0f; data[ 7] = 0.0f;
        data[ 8] = 0.0f; data[ 9] = 0.0f; data[10] = 1.0f; data[11] = 0.0f;
        data[12] = 0.0f; data[13] = 0.0f; data[14] = 0.0f; data[15] = 1.0f;
    }
    public Mat4(Mat4 m) {
        System.arraycopy(m.data, 0, data, 0, data.length);
    }

    // Addition
    public Mat4 add(Mat4 mat) {
        Mat4 temp = new Mat4();
        for (int i = 0; i < 16; i++) {
            temp.data[i] = data[i] + mat.data[i];
        }
        return temp;
    }

    // Subtraction
    public Mat4 sub(Mat4 mat) {
        Mat4 temp = new Mat4();
        for (int i = 0; i < 16; i++) {
            temp.data[i] = data[i] - mat.data[i];
        }
        return temp;
    }

    // Multiplication
    public Mat4 mul(float a) {
        Mat4 temp = new Mat4();
        for (int i = 0; i < 16; i++) {
            temp.data[i] = data[i] * a;
        }
        return temp;
    }
    public Float4 mul(Float4 vec) {
        Float4 temp = new Float4(0);
        temp.x = data[ 0] * vec.x + data[ 1] * vec.y + data[ 2] * vec.z + data[ 3] * vec.w;
        temp.y = data[ 4] * vec.x + data[ 5] * vec.y + data[ 6] * vec.z + data[ 7] * vec.w;
        temp.z = data[ 8] * vec.x + data[ 9] * vec.y + data[10] * vec.z + data[11] * vec.w;
        temp.w = data[12] * vec.x + data[13] * vec.y + data[14] * vec.z + data[15] * vec.w;
        return temp;
    }
    public Mat4 mul(Mat4 mat) {
        Mat4 temp = new Mat4();
        temp.data[ 0] = data[ 0] * mat.data[ 0] + data[ 1] * mat.data[ 4] + data[ 2] * mat.data[ 8] + data[ 3] * mat.data[12];
        temp.data[ 1] = data[ 0] * mat.data[ 1] + data[ 1] * mat.data[ 5] + data[ 2] * mat.data[ 9] + data[ 3] * mat.data[13];
        temp.data[ 2] = data[ 0] * mat.data[ 2] + data[ 1] * mat.data[ 6] + data[ 2] * mat.data[10] + data[ 3] * mat.data[14];
        temp.data[ 3] = data[ 0] * mat.data[ 3] + data[ 1] * mat.data[ 7] + data[ 2] * mat.data[11] + data[ 3] * mat.data[15];
        temp.data[ 4] = data[ 4] * mat.data[ 0] + data[ 5] * mat.data[ 4] + data[ 6] * mat.data[ 8] + data[ 7] * mat.data[12];
        temp.data[ 5] = data[ 4] * mat.data[ 1] + data[ 5] * mat.data[ 5] + data[ 6] * mat.data[ 9] + data[ 7] * mat.data[13];
        temp.data[ 6] = data[ 4] * mat.data[ 2] + data[ 5] * mat.data[ 6] + data[ 6] * mat.data[10] + data[ 7] * mat.data[14];
        temp.data[ 7] = data[ 4] * mat.data[ 3] + data[ 5] * mat.data[ 7] + data[ 6] * mat.data[11] + data[ 7] * mat.data[15];
        temp.data[ 8] = data[ 8] * mat.data[ 0] + data[ 9] * mat.data[ 4] + data[10] * mat.data[ 8] + data[11] * mat.data[12];
        temp.data[ 9] = data[ 8] * mat.data[ 1] + data[ 9] * mat.data[ 5] + data[10] * mat.data[ 9] + data[11] * mat.data[13];
        temp.data[10] = data[ 8] * mat.data[ 2] + data[ 9] * mat.data[ 6] + data[10] * mat.data[10] + data[11] * mat.data[14];
        temp.data[11] = data[ 8] * mat.data[ 3] + data[ 9] * mat.data[ 7] + data[10] * mat.data[11] + data[11] * mat.data[15];
        temp.data[12] = data[12] * mat.data[ 0] + data[13] * mat.data[ 4] + data[14] * mat.data[ 8] + data[15] * mat.data[12];
        temp.data[13] = data[12] * mat.data[ 1] + data[13] * mat.data[ 5] + data[14] * mat.data[ 9] + data[15] * mat.data[13];
        temp.data[14] = data[12] * mat.data[ 2] + data[13] * mat.data[ 6] + data[14] * mat.data[10] + data[15] * mat.data[14];
        temp.data[15] = data[12] * mat.data[ 3] + data[13] * mat.data[ 7] + data[14] * mat.data[11] + data[15] * mat.data[15];
        return temp;
    }

    // Division
    public Mat4 div(float a) {
        return mul(1.0f / a);
    }

    // Inverse
    public Mat4 inv() {
        Mat4 temp = new Mat4();
        temp.data[0] = data[5] * data[10] * data[15] -
                data[5] * data[11] * data[14] -
                data[9] * data[6] * data[15] +
                data[9] * data[7] * data[14] +
                data[13] * data[6] * data[11] -
                data[13] * data[7] * data[10];
        temp.data[4] = -data[4] * data[10] * data[15] +
                data[4] * data[11] * data[14] +
                data[8] * data[6] * data[15] -
                data[8] * data[7] * data[14] -
                data[12] * data[6] * data[11] +
                data[12] * data[7] * data[10];
        temp.data[8] = data[4] * data[9] * data[15] -
                data[4] * data[11] * data[13] -
                data[8] * data[5] * data[15] +
                data[8] * data[7] * data[13] +
                data[12] * data[5] * data[11] -
                data[12] * data[7] * data[9];
        temp.data[12] = -data[4] * data[9] * data[14] +
                data[4] * data[10] * data[13] +
                data[8] * data[5] * data[14] -
                data[8] * data[6] * data[13] -
                data[12] * data[5] * data[10] +
                data[12] * data[6] * data[9];
        temp.data[1] = -data[1] * data[10] * data[15] +
                data[1] * data[11] * data[14] +
                data[9] * data[2] * data[15] -
                data[9] * data[3] * data[14] -
                data[13] * data[2] * data[11] +
                data[13] * data[3] * data[10];
        temp.data[5] = data[0] * data[10] * data[15] -
                data[0] * data[11] * data[14] -
                data[8] * data[2] * data[15] +
                data[8] * data[3] * data[14] +
                data[12] * data[2] * data[11] -
                data[12] * data[3] * data[10];
        temp.data[9] = -data[0] * data[9] * data[15] +
                data[0] * data[11] * data[13] +
                data[8] * data[1] * data[15] -
                data[8] * data[3] * data[13] -
                data[12] * data[1] * data[11] +
                data[12] * data[3] * data[9];
        temp.data[13] = data[0] * data[9] * data[14] -
                data[0] * data[10] * data[13] -
                data[8] * data[1] * data[14] +
                data[8] * data[2] * data[13] +
                data[12] * data[1] * data[10] -
                data[12] * data[2] * data[9];
        temp.data[2] = data[1] * data[6] * data[15] -
                data[1] * data[7] * data[14] -
                data[5] * data[2] * data[15] +
                data[5] * data[3] * data[14] +
                data[13] * data[2] * data[7] -
                data[13] * data[3] * data[6];
        temp.data[6] = -data[0] * data[6] * data[15] +
                data[0] * data[7] * data[14] +
                data[4] * data[2] * data[15] -
                data[4] * data[3] * data[14] -
                data[12] * data[2] * data[7] +
                data[12] * data[3] * data[6];
        temp.data[10] = data[0] * data[5] * data[15] -
                data[0] * data[7] * data[13] -
                data[4] * data[1] * data[15] +
                data[4] * data[3] * data[13] +
                data[12] * data[1] * data[7] -
                data[12] * data[3] * data[5];
        temp.data[14] = -data[0] * data[5] * data[14] +
                data[0] * data[6] * data[13] +
                data[4] * data[1] * data[14] -
                data[4] * data[2] * data[13] -
                data[12] * data[1] * data[6] +
                data[12] * data[2] * data[5];
        temp.data[3] = -data[1] * data[6] * data[11] +
                data[1] * data[7] * data[10] +
                data[5] * data[2] * data[11] -
                data[5] * data[3] * data[10] -
                data[9] * data[2] * data[7] +
                data[9] * data[3] * data[6];
        temp.data[7] = data[0] * data[6] * data[11] -
                data[0] * data[7] * data[10] -
                data[4] * data[2] * data[11] +
                data[4] * data[3] * data[10] +
                data[8] * data[2] * data[7] -
                data[8] * data[3] * data[6];
        temp.data[11] = -data[0] * data[5] * data[11] +
                data[0] * data[7] * data[9] +
                data[4] * data[1] * data[11] -
                data[4] * data[3] * data[9] -
                data[8] * data[1] * data[7] +
                data[8] * data[3] * data[5];
        temp.data[15] = data[0] * data[5] * data[10] -
                data[0] * data[6] * data[9] -
                data[4] * data[1] * data[10] +
                data[4] * data[2] * data[9] +
                data[8] * data[1] * data[6] -
                data[8] * data[2] * data[5];

        // Det calc
	    final float det = data[0] * temp.data[0] + data[1] * temp.data[4] +
                data[2] * temp.data[8] + data[3] * temp.data[12];

        // Det check
        if (det == 0.0f) {
            return new Mat4();
        }

        // Return
        return temp.div(det);
    }

    // Translation matrix
    public static Mat4 translate(Float3 translation) {
        Mat4 temp = new Mat4();
        temp.data[ 3] = translation.x;
        temp.data[ 7] = translation.y;
        temp.data[11] = translation.z;
        return temp;
    }

    // Rotation matrix
    public static Mat4 rotate(Float3 rotation) {
        // Computing x rotation matrix
        final float xSin = (float)Math.sin(Math.toRadians(rotation.x));
        final float xCos = (float)Math.cos(Math.toRadians(rotation.x));
        Mat4 xRot = new Mat4();
        xRot.data[ 5] =  xCos;
        xRot.data[ 6] = -xSin;
        xRot.data[ 9] =  xSin;
        xRot.data[10] =  xCos;

        // Computing y rotation matrix
        final float ySin = (float)Math.sin(Math.toRadians(rotation.y));
        final float yCos = (float)Math.cos(Math.toRadians(rotation.y));
        Mat4 yRot = new Mat4();
        yRot.data[ 0] =  yCos;
        yRot.data[ 2] =  ySin;
        yRot.data[ 8] = -ySin;
        yRot.data[10] =  yCos;

        // Computing z rotation matrix
        final float zSin = (float)Math.sin(Math.toRadians(rotation.z));
        final float zCos = (float)Math.cos(Math.toRadians(rotation.z));
        Mat4 zRot = new Mat4();
        zRot.data[0] =  zCos;
        zRot.data[1] = -zSin;
        zRot.data[4] =  zSin;
        zRot.data[5] =  zCos;

        // Multiplying and returning
        return zRot.mul(yRot).mul(xRot);
    }

    // Scaling matrix
    public static Mat4 scale(Float3 size) {
        Mat4 temp = new Mat4();
        temp.data[ 0] = size.x;
        temp.data[ 5] = size.y;
        temp.data[10] = size.z;
        return temp;
    }

    // Perspective projection matrix
    private static final float halfAngleRad = (float)(Math.PI / 360);
    public static Mat4 persp(float fov, float ar, float zNear, float zFar) {
		final float tanHalf = 1 / (float)Math.tan(fov * halfAngleRad);
        Mat4 temp = new Mat4();
        temp.data[ 0] = tanHalf / ar;
        temp.data[ 5] = tanHalf;
        temp.data[10] = (-zFar - zNear) / (zNear - zFar);
        temp.data[11] = (2 * zNear * zFar) / (zNear - zFar);
        temp.data[14] = 1;
        temp.data[15] = 0;
        return temp;
    }

    // Orthographic projection matrix
    public static Mat4 ortho(float left, float right, float bottom, float top, float nearZ, float farZ) {
        Mat4 temp = new Mat4();
        temp.data[ 0] = 2 / (right - left);
        temp.data[ 5] = 2 / (top - bottom);
        temp.data[10] = -2 / (farZ - nearZ);
        temp.data[ 3] = -(right + left) / (right - left);
        temp.data[ 7] = -(top + bottom) / (top - bottom);
        temp.data[11] = -(farZ + nearZ) / (farZ - nearZ);
        return temp;
    }

    // "Look at" matrix
    public static Mat4 lookAt(Float3 pos, Float3 tar, Float3 up) {
        final Float3 f = tar.sub(pos).norm();
        final Float3 s = up.cross(f).norm();
        final Float3 u = f.cross(s);

        Mat4 temp = new Mat4();
        temp.data[ 0] = s.x;
        temp.data[ 1] = s.y;
        temp.data[ 2] = s.z;
        temp.data[ 3] = -s.dot(pos);
        temp.data[ 4] = u.x;
        temp.data[ 5] = u.y;
        temp.data[ 6] = u.z;
        temp.data[ 7] = -u.dot(pos);
        temp.data[ 8] = f.x;
        temp.data[ 9] = f.y;
        temp.data[10] = f.z;
        temp.data[11] = -f.dot(pos);
        return temp;
    }

    // String
    public String toString() {
        return  "[" + data[ 0] + " " + data[ 1] + " " + data[ 2] + " " + data[ 3] + "]\n" +
                "[" + data[ 4] + " " + data[ 5] + " " + data[ 6] + " " + data[ 7] + "]\n" +
                "[" + data[ 8] + " " + data[ 9] + " " + data[10] + " " + data[11] + "]\n" +
                "[" + data[12] + " " + data[13] + " " + data[14] + " " + data[15] + "]";
    }
}
