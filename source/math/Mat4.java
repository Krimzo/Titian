package math;

import java.io.Serializable;

public class Mat4 extends Matrix implements Serializable {
    public Mat4() {
        super(4, 4);
    }

    public Mat4(float[] data) {
        this(); System.arraycopy(data, 0, this.data, 0, this.data.length);
    }

    public Mat4(Matrix matrix) {
        this(matrix.data);
    }

    public Mat4(Mat4 mat) {
        this(mat.data);
    }

    public Mat4 add(Mat4 mat) {
        return new Mat4(super.add(mat));
    }

    public Mat4 subtract(Mat4 mat) {
        return new Mat4(super.subtract(mat));
    }

    public Float4 multiply(Float4 vec) {
        return new Float4(super.multiply(vec.array(), 1, 4).data);
    }

    public Mat4 multiply(Mat4 mat) {
        return new Mat4(super.multiply(mat));
    }

    public Mat4 absolute() {
        return new Mat4(super.absolute());
    }

    public Mat4 negate() {
        return new Mat4(super.negate());
    }

    public Mat4 transpose() {
        return new Mat4(super.transpose());
    }

    public Mat4 inverse() {
        return new Mat4(super.inverse());
    }

    public static Mat4 translation(Float3 translation) {
        Mat4 result = new Mat4();
        result.data[ 3] = translation.x;
        result.data[ 7] = translation.y;
        result.data[11] = translation.z;
        return result;
    }

    public static Mat4 rotation(Float3 rotation) {
        final float xSin = (float) Math.sin(Math.toRadians(rotation.x));
        final float xCos = (float) Math.cos(Math.toRadians(rotation.x));
        Mat4 xRot = new Mat4();
        xRot.data[ 5] =  xCos;
        xRot.data[ 6] = -xSin;
        xRot.data[ 9] =  xSin;
        xRot.data[10] =  xCos;

        final float ySin = (float) Math.sin(Math.toRadians(rotation.y));
        final float yCos = (float) Math.cos(Math.toRadians(rotation.y));
        Mat4 yRot = new Mat4();
        yRot.data[ 0] =  yCos;
        yRot.data[ 2] =  ySin;
        yRot.data[ 8] = -ySin;
        yRot.data[10] =  yCos;

        final float zSin = (float) Math.sin(Math.toRadians(rotation.z));
        final float zCos = (float) Math.cos(Math.toRadians(rotation.z));
        Mat4 zRot = new Mat4();
        zRot.data[0] =  zCos;
        zRot.data[1] = -zSin;
        zRot.data[4] =  zSin;
        zRot.data[5] =  zCos;

        return zRot.multiply(yRot).multiply(xRot);
    }

    public static Mat4 scaling(Float3 size) {
        Mat4 result = new Mat4();
        result.data[ 0] = size.x;
        result.data[ 5] = size.y;
        result.data[10] = size.z;
        return result;
    }

    public static Mat4 perspective(float fov, float aspect, float zNear, float zFar) {
        final float xScale = 1.0f / (float) Math.tan(Math.toRadians(fov * 0.5f));
        final float yScale = xScale * aspect;

        Mat4 result = new Mat4();
        result.data[ 0] = xScale;
        result.data[ 5] = yScale;
        result.data[10] = zFar / (zNear - zFar);
        result.data[11] = zNear * zFar / (zNear - zFar);
        result.data[14] = -1;
        result.data[15] = 0;
        return result;
    }

    public static Mat4 orthographic(float width, float height, float zNear, float zFar) {
        Mat4 result = new Mat4();
        result.data[ 0] = 2 / width;
        result.data[ 5] = 2 / height;
        result.data[10] = 1 / (zNear - zFar);
        result.data[11] = zNear / (zNear - zFar);
        return result;
    }

    public static Mat4 lookAt(Float3 pos, Float3 tar, Float3 up) {
        final Float3 f = pos.subtract(tar).normalize();
        final Float3 s = up.cross(f).normalize();
        final Float3 u = f.cross(s);

        Mat4 result = new Mat4();
        result.data[ 0] = s.x;
        result.data[ 1] = s.y;
        result.data[ 2] = s.z;
        result.data[ 3] = -s.dot(pos);
        result.data[ 4] = u.x;
        result.data[ 5] = u.y;
        result.data[ 6] = u.z;
        result.data[ 7] = -u.dot(pos);
        result.data[ 8] = f.x;
        result.data[ 9] = f.y;
        result.data[10] = f.z;
        result.data[11] = -f.dot(pos);
        return result;
    }
}
