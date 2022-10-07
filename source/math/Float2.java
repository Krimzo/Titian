package math;

import java.io.Serializable;

public class Float2 implements Serializable {
    public float x;
    public float y;

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

    public float[] array() {
        return new float[] { x, y };
    }

    public Float2 add(Float2 v) {
        return new Float2(x + v.x, y + v.y);
    }

    public Float2 subtract(Float2 v) {
        return new Float2(x - v.x, y - v.y);
    }

    public Float2 multiply(float a) {
        return new Float2(x * a, y * a);
    }

    public Float2 multiply(Float2 v) {
        return new Float2(x * v.x, y * v.y);
    }

    public Float2 divide(float a) {
        return multiply(1.0f / a);
    }

    public Float2 divide(Float2 v) {
        return new Float2(x / v.x, y / v.y);
    }

    public boolean equals(Float2 v) {
        return x == v.x && y == v.y;
    }

    public Float2 negate() {
        return multiply(-1.0f);
    }

    public Float2 absolute() {
        return new Float2(Math.abs(x), Math.abs(y));
    }

    public float length() {
        return (float) Math.sqrt(dot(this));
    }

    public Float2 normalize() {
        return divide(length());
    }

    public float dot(Float2 v) {
        return x * v.x + y * v.y;
    }

    public float angle(Float2 v) {
        return (float) Math.toDegrees(Math.acos(normalize().dot(v.normalize())));
    }

    public Float2 rotate(float angle) {
        final float sinA = (float) Math.sin(Math.toRadians(angle));
        final float cosA = (float) Math.cos(Math.toRadians(angle));
        return new Float2(cosA * x - sinA * y, sinA * x + cosA * y);
    }

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

    public String toString() {
        return "(" + x + ", " + y + ")";
    }
}
