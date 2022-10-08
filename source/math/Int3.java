package math;

import java.awt.*;
import java.io.Serializable;

public class Int3 implements Serializable {
    public int x;
    public int y;
    public int z;

    public Int3() {
        set(0);
    }

    public Int3(int a) {
        set(a);
    }

    public Int3(int x, int y, int z) {
        set(x, y, z);
    }

    public Int3(Int2 v, int z) {
        set(v, z);
    }

    public Int3(int x, Int2 v) {
        set(x, v);
    }

    public Int3(Float3 v) {
        set(v);
    }

    public Int3(Int3 v) {
        set(v);
    }

    // Setters
    public void set(int x, int y, int z) {
        this.x = x; this.y = y; this.z = z;
    }

    public void set(int a) {
        set(a, a, a);
    }

    public void set(int[] data) {
        set(data[0], data[1], data[2]);
    }

    public void set(Int2 v, int z) {
        set(v.x, v.y, z);
    }

    public void set(int x, Int2 v) {
        set(x, v.x, v.y);
    }

    public void set(Int3 v) {
        set(v.x, v.y, v.z);
    }

    public void set(Float3 v) {
        set((int) v.x, (int) v.y, (int) v.z);
    }

    public void set(Color color) {
        set(color.getRed(), color.getGreen(), color.getBlue());
    }

    // Getters
    public Int2 xy() {
        return new Int2(x, y);
    }

    public int[] array() {
        return new int[] { x, y, z };
    }

    public Color color() {
        return new Color(x, y, z);
    }

    // Math
    public Int3 add(Int3 v) {
        return new Int3(x + v.x, y + v.y, z + v.z);
    }

    public Int3 subtract(Int3 v) {
        return new Int3(x - v.x, y - v.y, z - v.z);
    }

    public Int3 multiply(int a) {
        return new Int3(x * a, y * a, z * a);
    }

    public Int3 multiply(Int3 v) {
        return new Int3(x * v.x, y * v.y, z * v.z);
    }

    public Int3 divide(int a) {
        return new Int3(x / a, y / a, z / a);
    }

    public Int3 divide(Int3 v) {
        return new Int3(x / v.x, y / v.y, z / v.z);
    }

    public boolean equals(Int3 v) {
        return x == v.x && y == v.y && z == v.z;
    }

    public Int3 negate() {
        return multiply(-1);
    }

    public Int3 absolute() {
        return new Int3(Math.abs(x), Math.abs(y), Math.abs(z));
    }

    public static Int3 getPosX() {
        return new Int3(1, 0, 0);
    }
    public static Int3 getNegX() {
        return new Int3(-1, 0, 0);
    }
    public static Int3 getPosY() {
        return new Int3(0, 1, 0);
    }
    public static Int3 getNegY() {
        return new Int3(0, -1, 0);
    }
    public static Int3 getPosZ() {
        return new Int3(0, 0, 1);
    }
    public static Int3 getNegZ() {
        return new Int3(0, 0, -1);
    }

    public String toString() {
        return "(" + x + ", " + y + ", " + z + ")";
    }
}
