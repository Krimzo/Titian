package math;

import java.io.Serializable;

public class Mat3 extends Matrix implements Serializable {
    public Mat3() {
        super(3, 3);
    }

    public Mat3(float[] data) {
        this(); System.arraycopy(data, 0, this.data, 0, this.data.length);
    }

    public Mat3(Matrix matrix) {
        this(matrix.data);
    }

    public Mat3(Mat3 mat) {
        this(mat.data);
    }

    public Mat3 add(Mat3 mat) {
        return new Mat3(super.add(mat));
    }

    public Mat3 subtract(Mat3 mat) {
        return new Mat3(super.subtract(mat));
    }

    public Float3 multiply(Float3 vec) {
        return new Float3(super.multiply(vec.array(), 1, 3).data);
    }

    public Mat3 multiply(Mat3 mat) {
        return new Mat3(super.multiply(mat));
    }

    public Mat3 absolute() {
        return new Mat3(super.absolute());
    }

    public Mat3 negate() {
        return new Mat3(super.negate());
    }

    public Mat3 transpose() {
        return new Mat3(super.transpose());
    }

    public Mat3 inverse() {
        return new Mat3(super.inverse());
    }

    public static Mat3 translation(Float2 translation) {
        Mat3 result = new Mat3();
        result.data[2] = translation.x;
        result.data[5] = translation.y;
        return result;
    }

    public static Mat3 rotation(float rotation) {
	    final float zSin = (float) Math.sin(Math.toRadians(rotation));
	    final float zCos = (float) Math.cos(Math.toRadians(rotation));

        Mat3 result = new Mat3();
        result.data[0] =  zCos;
        result.data[1] = -zSin;
        result.data[3] =  zSin;
        result.data[4] =  zCos;
        return result;
    }

    public static Mat3 scaling(Float2 size) {
        Mat3 result = new Mat3();
        result.data[0] = size.x;
        result.data[4] = size.y;
        return result;
    }
}
