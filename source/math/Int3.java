package math;

import java.io.Serializable;

public class Int3 implements Serializable {
    public int x;
    public int y;
    public int z;

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

    public Int3(Float3 v) {
        x = (int) v.x;
        y = (int) v.y;
        z = (int) v.z;
    }

    public Int3(Int3 v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }

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
