package math;

import java.awt.*;
import java.io.Serializable;

public class Float4 implements Serializable {
    public float x;
    public float y;
    public float z;
    public float w;

    public Float4() {
        set(0);
    }

    public Float4(float a) {
        set(a);
    }

    public Float4(float[] data) {
        set(data);
    }

    public Float4(float x, float y, float z, float w) {
        set(x, y, z, w);
    }

    public Float4(Float2 v, float z, float w) {
        set(v, z, w);
    }

    public Float4(float x, Float2 v, float w) {
        set(x, v, w);
    }

    public Float4(float x, float y, Float2 v) {
        set(x, y, v);
    }

    public Float4(Float2 v1, Float2 v2) {
        set(v1, v2);
    }

    public Float4(Float3 v, float w) {
        set(v, w);
    }

    public Float4(float x, Float3 v) {
        set(x, v);
    }

    public Float4(Float4 v) {
        set(v);
    }

    public Float4(Int4 v) {
        set(v);
    }

    public Float4(Color color) {
        set(color);
    }

    // Setters
    public void set(float x, float y, float z, float w) {
        this.x = x; this.y = y; this.z = z; this.w = w;
    }

    public void set(float a) {
        set(a, a, a, a);
    }

    public void set(float[] data) {
        set(data[0], data[1], data[2], data[3]);
    }

    public void set(Float2 v, float z, float w) {
        set(v.x, v.y, z, w);
    }

    public void set(float x, Float2 v, float w) {
        set(x, v.x, v.y, w);
    }

    public void set(float x, float y, Float2 v) {
        set(x, y, v.x, v.y);
    }

    public void set(Float2 v1, Float2 v2) {
        set(v1.x, v1.y, v2.x, v2.y);
    }

    public void set(Float3 v, float w) {
        set(v.x, v.y, v.z, w);
    }

    public void set(float x, Float3 v) {
        set(x, v.x, v.y, v.z);
    }

    public void set(Float4 v) {
        set(v.x, v.y, v.z, v.w);
    }

    public void set(Int4 v) {
        set(v.x, v.y, v.z, v.w);
    }

    public void set(Color color) {
        set(new Float3(color), color.getAlpha() / 255.0f);
    }

    // Getters
    public Float2 xy() {
        return new Float2(x, y);
    }

    public Float3 xyz() {
        return new Float3(x, y, z);
    }

    public float[] array() {
        return new float[] { x, y, z, w };
    }

    public Color color() {
        return new Color(
            (int) Math.min(Math.max(x * 255, 0), 255),
            (int) Math.min(Math.max(y * 255, 0), 255),
            (int) Math.min(Math.max(z * 255, 0), 255),
            (int) Math.min(Math.max(w * 255, 0), 255)
        );
    }

    // Math
    public Float4 add(Float4 v) {
        return new Float4(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    public Float4 subtract(Float4 v) {
        return new Float4(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    public Float4 multiply(float a) {
        return new Float4(x * a, y * a, z * a, w * a);
    }

    public Float4 multiply(Float4 v) {
        return new Float4(x * v.x, y * v.y, z * v.z, w * v.w);
    }

    public Float4 divide(float a) {
        return multiply(1.0f / a);
    }

    public Float4 divide(Float4 v) {
        return new Float4(x / v.x, y / v.y, z / v.z, w / v.w);
    }

    public boolean equals(Float4 v) {
        return x == v.x && y == v.y && z == v.z && w == v.w;
    }

    public Float4 negate() {
        return multiply(-1);
    }

    public float length() {
        return (float) Math.sqrt(dot(this));
    }

    public Float4 normalize() {
        return divide(length());
    }

    public float dot(Float4 v) {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    public float angle(Float4 v) {
        return (float) Math.toDegrees(Math.acos(normalize().dot(v.normalize())));
    }

    public static Float4 getPosX() {
        return new Float4(1, 0, 0, 0);
    }
    public static Float4 getNegX() {
        return new Float4(-1, 0, 0, 0);
    }
    public static Float4 getPosY() {
        return new Float4(0, 1, 0, 0);
    }
    public static Float4 getNegY() {
        return new Float4(0, -1, 0, 0);
    }
    public static Float4 getPosZ() {
        return new Float4(0, 0, 1, 0);
    }
    public static Float4 getNegZ() {
        return new Float4(0, 0, -1, 0);
    }
    public static Float4 getPosW() {
        return new Float4(0, 0, 0, 1);
    }
    public static Float4 getNegW() {
        return new Float4(0, 0, 0, -1);
    }

    public String toString() {
        return "(" + x + ", " + y + ", " + z + ", " + w + ")";
    }
}
