package math;

import java.awt.*;
import java.io.Serializable;

public class Float3 implements Serializable {
    public float x;
    public float y;
    public float z;

    public Float3() {
        set(0);
    }

    public Float3(float a) {
        set(a);
    }

    public Float3(float[] data) {
        set(data);
    }

    public Float3(float x, float y, float z) {
        set(x, y, z);
    }

    public Float3(Float2 v, float z) {
        set(v, z);
    }

    public Float3(float x, Float2 v) {
        set(x, v);
    }

    public Float3(Float3 v) {
        set(v);
    }

    public Float3(Int3 v) {
        set(v);
    }

    public Float3(Color color) {
        set(color);
    }

    // Setters
    public void set(float x, float y, float z) {
        this.x = x; this.y = y; this.z = z;
    }

    public void set(float a) {
        set(a, a, a);
    }

    public void set(float[] data) {
        set(data[0], data[1], data[2]);
    }

    public void set(Float2 v, float z) {
        set(v.x, v.y, z);
    }

    public void set(float x, Float2 v) {
        set(x, v.x, v.y);
    }

    public void set(Float3 v) {
        set(v.x, v.y, v.z);
    }

    public void set(Int3 v) {
        set(v.x, v.y, v.z);
    }

    public void set(Color color) {
        final float conv = 1.0f / 255.0f;
        set(color.getRed() * conv, color.getGreen() * conv, color.getBlue() * conv);
    }

    // Getters
    public Float2 xy() {
        return new Float2(x, y);
    }

    public float[] array() {
        return new float[] { x, y, z };
    }

    public Color color() {
        return new Color(
            (int) Math.min(Math.max(x * 255, 0), 255),
            (int) Math.min(Math.max(y * 255, 0), 255),
            (int) Math.min(Math.max(z * 255, 0), 255)
        );
    }

    // Math
    public Float3 add(Float3 v) {
        return new Float3(x + v.x, y + v.y, z + v.z);
    }

    public Float3 subtract(Float3 v) {
        return new Float3(x - v.x, y - v.y, z - v.z);
    }

    public Float3 multiply(float a) {
        return new Float3(x * a, y * a, z * a);
    }

    public Float3 multiply(Float3 v) {
        return new Float3(x * v.x, y * v.y, z * v.z);
    }

    public Float3 divide(float a) {
        return multiply(1.0f / a);
    }

    public Float3 divide(Float3 v) {
        return new Float3(x / v.x, y / v.y, z / v.z);
    }

    public boolean equals(Float3 v) {
        return x == v.x && y == v.y && z == v.z;
    }

    public Float3 negate() {
        return multiply(-1.0f);
    }

    public Float3 absolute() {
        return new Float3(Math.abs(x), Math.abs(y), Math.abs(z));
    }

    public float length() {
        return (float) Math.sqrt(dot(this));
    }

    public Float3 normalize() {
        return divide(length());
    }

    public float dot(Float3 v) {
        return x * v.x + y * v.y + z * v.z;
    }

    public Float3 cross(Float3 v) {
        return new Float3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    public float angle(Float3 v) {
        return (float) Math.toDegrees(Math.acos(normalize().dot(v.normalize())));
    }

    public Float3 rotate(float angle, Float3 axis) {
        final float angleSin = (float) Math.sin(Math.toRadians(angle * 0.5f));
        final float angleCos = (float) Math.cos(Math.toRadians(angle * 0.5f));

        final float qx = axis.x * angleSin;
        final float qy = axis.y * angleSin;
        final float qz = axis.z * angleSin;
        final float x2 = qx * qx;
        final float y2 = qy * qy;
        final float z2 = qz * qz;
        final float w2 = angleCos * angleCos;
        final float xy = qx * qy;
        final float xz = qx * qz;
        final float yz = qy * qz;
        final float xw = qx * angleCos;
        final float yw = qy * angleCos;
        final float zw = qz * angleCos;

        Float3 temp = new Float3(0);
        temp.x = (w2 + x2 - z2 - y2) * x + (-zw + xy - zw + xy) * y + (yw + xz + xz + yw) * z;
        temp.y = (xy + zw + zw + xy) * x + ( y2 - z2 + w2 - x2) * y + (yz + yz - xw - xw) * z;
        temp.z = (xz - yw + xz - yw) * x + ( yz + yz + xw + xw) * y + (z2 - y2 - x2 + w2) * z;
        return temp;
    }

    public Float3 reflect(Float3 v) {
        final Float3 normal = v.normalize();
        return this.subtract(normal.multiply(2 * this.dot(normal)));
    }

    public static Float3 getPosX() {
        return new Float3(1.0f, 0.0f, 0.0f);
    }
    public static Float3 getNegX() {
        return new Float3(-1.0f, 0.0f, 0.0f);
    }
    public static Float3 getPosY() {
        return new Float3(0.0f, 1.0f, 0.0f);
    }
    public static Float3 getNegY() {
        return new Float3(0.0f, -1.0f, 0.0f);
    }
    public static Float3 getPosZ() {
        return new Float3(0.0f, 0.0f, 1.0f);
    }
    public static Float3 getNegZ() {
        return new Float3(0.0f, 0.0f, -1.0f);
    }

    public String toString() {
        return "(" + x + ", " + y + ", " + z + ")";
    }
}
