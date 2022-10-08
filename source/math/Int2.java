package math;

import java.io.Serializable;

public class Int2 implements Serializable {
    public int x;
    public int y;

    public Int2() {
        set(0);
    }

    public Int2(int a) {
        set(a);
    }

    public Int2(int[] data) {
        set(data);
    }

    public Int2(int x, int y) {
        set(x, y);
    }

    public Int2(Int2 v) {
        set(v);
    }

    public Int2(Float2 v) {
        set(v);
    }

    // Setters
    public void set(int x, int y) {
        this.x = x; this.y = y;
    }

    public void set(int a) {
        set(a, a);
    }

    public void set(int[] data) {
        set(data[0], data[1]);
    }

    public void set(Int2 v) {
        set(v.x, v.y);
    }

    public void set(Float2 v) {
        set((int) v.x, (int) v.y);
    }

    // Getters
    public int[] array() {
        return new int[] { x, y };
    }

    // Math
    public Int2 add(Int2 v) {
        return new Int2(x + v.x, y + v.y);
    }

    public Int2 subtract(Int2 v) {
        return new Int2(x - v.x, y - v.y);
    }

    public Int2 multiply(int a) {
        return new Int2(x * a, y * a);
    }

    public Int2 multiply(Int2 v) {
        return new Int2(x * v.x, y * v.y);
    }

    public Int2 divide(int a) {
        return new Int2(x / a, y / a);
    }

    public Int2 divide(Int2 v) {
        return new Int2(x / v.x, y / v.y);
    }

    public boolean equals(Int2 v) {
        return x == v.x && y == v.y;
    }

    public Int2 negate() {
        return multiply(-1);
    }

    public Int2 absolute() {
        return new Int2(Math.abs(x), Math.abs(y));
    }

    public static Int2 getPosX() {
        return new Int2(1, 0);
    }
    public static Int2 getNegX() {
        return new Int2(-1, 0);
    }
    public static Int2 getPosY() {
        return new Int2(0, 1);
    }
    public static Int2 getNegY() {
        return new Int2(0, -1);
    }

    public String toString() {
        return "(" + x + ", " + y + ")";
    }
}
