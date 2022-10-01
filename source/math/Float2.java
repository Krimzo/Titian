package math;

import java.io.Serializable;

public class Float2 implements Serializable {
    public float x, y;

    public Float2() {
        x = 0.0f;
        y = 0.0f;
    }

    public Float2(float a) {
        x = a;
        y = a;
    }

    public Float2(float[] data) {
        this(data[0], data[1]);
    }

    public Float2(float x, float y) {
        this.x = x;
        this.y = y;
    }

    public Float2(Float2 v) {
        x = v.x;
        y = v.y;
    }

    public Float2(Int2 v) {
        x = (float)v.x;
        y = (float)v.y;
    }

    // Getter
    public float[] array() {
        return new float[] { x, y };
    }

    // Addition
    public Float2 add(Float2 v) {
        return new Float2(x + v.x, y + v.y);
    }

    // Subtraction
    public Float2 sub(Float2 v) {
        return new Float2(x - v.x, y - v.y);
    }

    // Multiplication
    public Float2 mul(float a) {
        return new Float2(x * a, y * a);
    }
    public Float2 mul(Float2 v) {
        return new Float2(x * v.x, y * v.y);
    }

    // Division
    public Float2 div(float a) {
        return mul(1.0f / a);
    }
    public Float2 div(Float2 v) {
        return new Float2(x / v.x, y / v.y);
    }

    // Comparison
    public boolean equals(Float2 v) {
        return x == v.x && y == v.y;
    }

    // Negation
    public Float2 neg() {
        return mul(-1.0f);
    }

    // Absolute
    public Float2 abs() {
        return new Float2(Math.abs(x), Math.abs(y));
    }

    // Length
    public float len() {
        return (float) Math.sqrt(x * x + y * y);
    }

    // Normalization
    public Float2 norm() {
        return div(len());
    }

    // Dot product
    public float dot(Float2 v) {
        return x * v.x + y * v.y;
    }

    // Angle between vectors
    public float angle(Float2 v) {
        return (float) Math.toDegrees(Math.acos(norm().dot(v.norm())));
    }

    // Returns a rotated vector around the given point
    public Float2 rotate(float angle) {
        final float sinA = (float) Math.sin(Math.toRadians(angle));
        final float cosA = (float) Math.cos(Math.toRadians(angle));
        return new Float2(cosA * x - sinA * y, sinA * x + cosA * y);
    }

    // Constants
    public static Float2 getPosX() {
        return new Float2(1.0f, 0.0f);
    }
    public static Float2 getNegX() {
        return new Float2(-1.0f, 0.0f);
    }
    public static Float2 getPosY() {
        return new Float2(0.0f, 1.0f);
    }
    public static Float2 getNegY() {
        return new Float2(0.0f, -1.0f);
    }

    // String
    public String toString() {
        return "(" + x + ", " + y + ")";
    }
}
