package math;

import java.awt.*;
import java.io.Serializable;

public class Int4 implements Serializable {
    public int x;
    public int y;
    public int z;
    public int w;

    public Int4() {
        set(0);
    }

    public Int4(int a) {
        set(a);
    }

    public Int4(int[] data) {
        set(data);
    }

    public Int4(int x, int y, int z, int w) {
        set(x, y, z, w);
    }

    public Int4(Int2 v, int z, int w) {
        set(v, z, w);
    }

    public Int4(int x, Int2 v, int w) {
        set(x, v, w);
    }

    public Int4(int x, int y, Int2 v) {
        set(x, y, v);
    }

    public Int4(Int2 v1, Int2 v2) {
        set(v1, v2);
    }

    public Int4(Int3 v, int w) {
        set(v, w);
    }

    public Int4(int x, Int3 v) {
        set(x, v);
    }

    public Int4(Int4 v) {
        set(v);
    }

    public Int4(Float4 v) {
        set(v);
    }

    public Int4(Color color) {
        set(color);
    }

    // Setters
    public void set(int x, int y, int z, int w) {
        this.x = x; this.y = y; this.z = z; this.w = w;
    }

    public void set(int a) {
        set(a, a, a, a);
    }

    public void set(int[] data) {
        set(data[0], data[1], data[2], data[3]);
    }

    public void set(Int2 v, int z, int w) {
        set(v.x, v.y, z, w);
    }

    public void set(int x, Int2 v, int w) {
        set(x, v.x, v.y, w);
    }

    public void set(int x, int y, Int2 v) {
        set(x, y, v.x, v.y);
    }

    public void set(Int2 v1, Int2 v2) {
        set(v1.x, v1.y, v2.x, v2.y);
    }

    public void set(Int3 v, int w) {
        set(v.x, v.y, v.z, w);
    }

    public void set(int x, Int3 v) {
        set(x, v.x, v.y, v.z);
    }

    public void set(Int4 v) {
        set(v.x, v.y, v.z, v.w);
    }

    public void set(Float4 v) {
        set((int) v.x, (int) v.y, (int) v.z, (int) v.w);
    }

    public void set(Color color) {
        set(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
    }

    // Getters
    public Int2 xy() {
        return new Int2(x, y);
    }

    public Int3 xyz() {
        return new Int3(x, y, z);
    }

    public int[] array() {
        return new int[] { x, y, z, w };
    }

    public Color color() {
        return new Color(x, y, z, w);
    }

    // Math
    public Int4 add(Int4 v) {
        return new Int4(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    public Int4 subtract(Int4 v) {
        return new Int4(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    public Int4 multiply(int a) {
        return new Int4(x * a, y * a, z * a, w * a);
    }

    public Int4 multiply(Int4 v) {
        return new Int4(x * v.x, y * v.y, z * v.z, w * v.w);
    }

    public Int4 divide(int a) {
        return new Int4(x / a, y / a, z / a, w / a);
    }

    public Int4 divide(Int4 v) {
        return new Int4(x / v.x, y / v.y, z / v.z, w / v.w);
    }

    public boolean equals(Int4 v) {
        return x == v.x && y == v.y && z == v.z && w == v.w;
    }

    public Int4 negate() {
        return multiply(-1);
    }

    public Int4 absolute() {
        return new Int4(Math.abs(x), Math.abs(y), Math.abs(z), Math.abs(w));
    }

    public static Int4 getPosX() {
        return new Int4(1, 0, 0, 0);
    }
    public static Int4 getNegX() {
        return new Int4(-1, 0, 0, 0);
    }
    public static Int4 getPosY() {
        return new Int4(0, 1, 0, 0);
    }
    public static Int4 getNegY() {
        return new Int4(0, -1, 0, 0);
    }
    public static Int4 getPosZ() {
        return new Int4(0, 0, 1, 0);
    }
    public static Int4 getNegZ() {
        return new Int4(0, 0, -1, 0);
    }
    public static Int4 getPosW() {
        return new Int4(0, 0, 0, 1);
    }
    public static Int4 getNegW() {
        return new Int4(0, 0, 0, -1);
    }

    public String toString() {
        return "(" + x + ", " + y + ", " + z + ", " + w + ")";
    }
}
