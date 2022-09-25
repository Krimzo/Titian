package math;

import java.io.Serializable;

public class Int3 implements Serializable {
    public int x, y, z;

    public Int3() {
        x = 0;
        y = 0;
        z = 0;
    }
    public Int3(int a) {
        x = a;
        y = a;
        z = a;
    }
    public Int3(int x, int y, int z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }
    public Int3(Int2 v, int z) {
        x = v.x;
        y = v.y;
        this.z = z;
    }
    public Int3(int x, Int2 v) {
        this.x = x;
        y = v.x;
        z = v.y;
    }
    public Int3(Int3 v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    // Addition
    public Int3 add(Int3 v) {
        return new Int3(x + v.x, y + v.y, z + v.z);
    }

    // Subtraction
    public Int3 sub(Int3 v) {
        return new Int3(x - v.x, y - v.y, z - v.z);
    }

    // Multiplication
    public Int3 mul(int a) {
        return new Int3(x * a, y * a, z * a);
    }
    public Int3 mul(Int3 v) {
        return new Int3(x * v.x, y * v.y, z * v.z);
    }

    // Division
    public Int3 div(int a) {
        return new Int3(x / a, y / a, z / a);
    }
    public Int3 div(Int3 v) {
        return new Int3(x / v.x, y / v.y, z / v.z);
    }

    // Comparison
    public boolean equals(Int3 v) {
        return x == v.x && y == v.y && z == v.z;
    }

    // Negation
    public Int3 neg() {
        return mul(-1);
    }

    // Absolute
    public Int3 abs() {
        return new Int3(Math.abs(x), Math.abs(y), Math.abs(z));
    }

    // Constants
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

    // String
    public String toString() {
        return "(" + x + ", " + y + ", " + z + ")";
    }
}
