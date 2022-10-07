package math;

import java.awt.*;
import java.io.Serializable;

public class Float4 implements Serializable {
    public float x;
    public float y;
    public float z;
    public float w;

    public Float4() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
    }

    public Float4(float a) {
        x = a;
        y = a;
        z = a;
        w = a;
    }

    public Float4(float[] data) {
        this(data[0], data[1], data[2], data[3]);
    }

    public Float4(float x, float y, float z, float w) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }

    public Float4(Float2 v, float z, float w) {
        x = v.x;
        y = v.y;
        this.z = z;
        this.w = w;
    }

    public Float4(float x, Float2 v, float w) {
        this.x = x;
        y = v.x;
        z = v.y;
        this.w = w;
    }

    public Float4(float x, float y, Float2 v) {
        this.x = x;
        this.y = y;
        z = v.x;
        w = v.y;
    }

    public Float4(Float2 v1, Float2 v2) {
        x = v1.x;
        y = v1.y;
        z = v2.x;
        w = v2.y;
    }

    public Float4(Float3 v, float w) {
        x = v.x;
        y = v.y;
        z = v.z;
        this.w = w;
    }

    public Float4(float x, Float3 v) {
        this.x = x;
        y = v.x;
        z = v.y;
        w = v.z;
    }

    public Float4(Float4 v) {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
    }

    public Float4(Int4 v) {
        x = (float)v.x;
        y = (float)v.y;
        z = (float)v.z;
        w = (float)v.w;
    }

    public Float4(Color color) {
        this(new Float3(color), color.getAlpha() / 255.0f);
    }

    public Float2 xy() {
        return new Float2(x, y);
    }

    public Float3 xyz() {
        return new Float3(x, y, z);
    }

    public float[] array() {
        return new float[] { x, y, z, w };
    }

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
        return multiply(-1.0f);
    }

    public float length() {
        return (float) Math.sqrt(dot(this));
    }

    public Float4 normalize() {
        return divide(length());
    }

    float dot(Float4 v) {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    float angle(Float4 v) {
        return (float) Math.toDegrees(Math.acos(normalize().dot(v.normalize())));
    }

    public static Float4 getPosX() {
        return new Float4(1.0f, 0.0f, 0.0f, 0.0f);
    }
    public static Float4 getNegX() {
        return new Float4(-1.0f, 0.0f, 0.0f, 0.0f);
    }
    public static Float4 getPosY() {
        return new Float4(0.0f, 1.0f, 0.0f, 0.0f);
    }
    public static Float4 getNegY() {
        return new Float4(0.0f, -1.0f, 0.0f, 0.0f);
    }
    public static Float4 getPosZ() {
        return new Float4(0.0f, 0.0f, 1.0f, 0.0f);
    }
    public static Float4 getNegZ() {
        return new Float4(0.0f, 0.0f, -1.0f, 0.0f);
    }
    public static Float4 getPosW() {
        return new Float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    public static Float4 getNegW() {
        return new Float4(0.0f, 0.0f, 0.0f, -1.0f);
    }

    public String toString() {
        return "(" + x + ", " + y + ", " + z + ", " + w + ")";
    }
}
