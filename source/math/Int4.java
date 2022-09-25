package math;

import java.io.Serializable;

public class Int4 implements Serializable {
    public int x, y, z, w;

    public Int4() {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }
    public Int4(int a) {
        x = a;
        y = a;
        z = a;
        w = a;
    }
    public Int4(int x, int y, int z, int w) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }
    public Int4(Int2 v, int z, int w) {
        x = v.x;
        y = v.y;
        this.z = z;
        this.w = w;
    }
    public Int4(int x, Int2 v, int w) {
        this.x = x;
        y = v.x;
        z = v.y;
        this.w = w;
    }
    public Int4(int x, int y, Int2 v) {
        this.x = x;
        this.y = y;
        z = v.x;
        w = v.y;
    }
    public Int4(Int2 v1, Int2 v2) {
        x = v1.x;
        y = v1.y;
        z = v2.x;
        w = v2.y;
    }
    public Int4(Int3 v, int w) {
        x = v.x;
        y = v.y;
        z = v.z;
        this.w = w;
    }
    public Int4(int x, Int3 v) {
        this.x = x;
        y = v.x;
        z = v.y;
        w = v.z;
    }
    public Int4(Int4 v) {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
    }

    // Addition
    public Int4 add(Int4 v) {
        return new Int4(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    // Subtraction
    public Int4 sub(Int4 v) {
        return new Int4(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    // Multiplication
    public Int4 mul(int a) {
        return new Int4(x * a, y * a, z * a, w * a);
    }
    public Int4 mul(Int4 v) {
        return new Int4(x * v.x, y * v.y, z * v.z, w * v.w);
    }

    // Division
    public Int4 div(int a) {
        return new Int4(x / a, y / a, z / a, w / a);
    }
    public Int4 div(Int4 v) {
        return new Int4(x / v.x, y / v.y, z / v.z, w / v.w);
    }

    // Comparison
    public boolean equals(Int4 v) {
        return x == v.x && y == v.y && z == v.z && w == v.w;
    }

    // Negation
    public Int4 neg() {
        return mul(-1);
    }

    // Absolute
    public Int4 abs() {
        return new Int4(Math.abs(x), Math.abs(y), Math.abs(z), Math.abs(w));
    }

    // Constants
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

    // String
    public String toString() {
        return "(" + x + ", " + y + ", " + z + ", " + w + ")";
    }
}
