package math;

import utility.helper.StringHelper;

import java.io.Serializable;

public class Matrix implements Serializable {
    public final int width;
    public final int height;
    public final float[] data;

    public Matrix(int width, int height) {
        this.width = width;
        this.height = height;
        data = new float[width * height];

        if (isSquare()) {
            for (int i = 0; i < (width * height); i += (width + 1)) {
                data[i] = 1;
            }
        }
    }

    public float get(int i) {
        return data[i];
    }

    public void set(int i, float value) {
        data[i] = value;
    }

    public float get(int x, int y) {
        return get(y * width + x);
    }

    public void set(int x, int y, float value) {
        set(y * width + x, value);
    }

    public boolean isSquare() {
        return width == height;
    }

    public boolean sizeEquals(int width, int height) {
        return this.width == width && this.height == height;
    }

    public boolean sizeEquals(Matrix matrix) {
        return sizeEquals(matrix.width, matrix.height);
    }

    public Matrix add(Matrix matrix) {
        Matrix result = new Matrix(width, height);

        if (sizeEquals(matrix)) {
            for (int i = 0; i < (width * height); i++) {
                result.set(i, get(i) + matrix.get(i));
            }
        }

        return result;
    }

    public Matrix subtract(Matrix matrix) {
        Matrix result = new Matrix(width, height);

        if (sizeEquals(matrix)) {
            for (int i = 0; i < (width * height); i++) {
                result.set(i, get(i) - matrix.get(i));
            }
        }

        return result;
    }

    public Matrix multiply(float value) {
        Matrix result = new Matrix(width, height);

        for (int i = 0; i < (width * height); i++) {
            result.set(i, get(i) * value);
        }

        return result;
    }

    public Matrix multiply(float[] data, int W, int H) {
        Matrix result = new Matrix(W, height);

        if (width == H) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < W; x++) {
                    result.set(x, y, 0);

                    for (int i = 0; i < width; i++) {
                        result.set(x, y, result.get(x, y) + get(i, y) * data[i * W + x]);
                    }
                }
            }
        }

        return result;
    }
    
    public Matrix multiply(Matrix matrix) {
        return multiply(matrix.data, matrix.width, matrix.height);
    }

    public boolean equals(Matrix matrix) {
        if (!sizeEquals(matrix)) {
            return false;
        }

        for (int i = 0; i < (width * height); i++) {
            if (get(i) != matrix.get(i)) {
                return false;
            }
        }

        return true;
    }

    public Matrix absolute() {
        Matrix result = new Matrix(width, height);

        for (int i = 0; i < (width * height); i++) {
            result.set(i, Math.abs(get(i)));
        }

        return result;
    }

    public Matrix negate() {
        return multiply(-1);
    }

    public Matrix transpose() {
        Matrix result = new Matrix(width, height);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                result.set(x, y, get(y, x));
            }
        }

        return result;
    }

    public Matrix cofactor(int index) {
        return cofactor(index % width, index / height);
    }

    public Matrix cofactor(int X, int Y) {
        Matrix result = new Matrix(width - 1, height - 1);

        if (isSquare()) {
            int counter = 0;
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    if (x != X && y != Y) {
                        result.set(counter++, get(x, y));
                    }
                }
            }
        }

        return result;
    }

    public Matrix cofactor() {
        Matrix result = new Matrix(width, height);

        if (isSquare()) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    result.set(x, y, (((y + x + 2) % 2 != 0) ? -1 : 1) * cofactor(y * width + x).determinant());
                }
            }
        }

        return result;
    }

    public float determinant() {
        if (isSquare()) {
            if (width == 2) {
                return get(0) * get(3) - get(1) * get(2);
            }

            if (width > 2) {
                int multi = -1;
                float result = 0;
                for (int i = 0; i < width; i++) {
                    result += (multi *= -1) * get(i) * cofactor(i).determinant();
                }
                return result;
            }
        }

        return 0;
    }

    public Matrix adjoint() {
        if (isSquare()) {
            return cofactor().transpose();
        }
        return new Matrix(width, height);
    }

    public Matrix inverse() {
        if (isSquare()) {
            return adjoint().multiply(1 / determinant());
        }
        return new Matrix(width, height);
    }

    @Override
    public String toString() {
        int[] maxLengths = new int[width];
        String[] outputData = new String[width * height];

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                outputData[y * width + x] = String.format("%.2f", get(y * width + x));
                maxLengths[x] = Math.max(maxLengths[x], outputData[y * width + x].length());
            }
        }

        StringBuilder stream = new StringBuilder();

        for (int y = 0; y < height; y++) {
            stream.append("[");

            for (int x = 0; x < (width - 1); x++) {
                stream.append(StringHelper.spaces(maxLengths[x] - outputData[y * width + x].length()));
                stream.append(outputData[y * width + x]);
                stream.append(" ");
            }

            String lastLine = outputData[y * width + (width - 1)];
            stream.append(StringHelper.spaces(maxLengths[width - 1] - lastLine.length()));
            stream.append(lastLine);
            stream.append((y == (height - 1) ? "]" : "]\n"));
        }

        return stream.toString();
    }
}
