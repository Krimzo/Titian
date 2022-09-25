package math;

import java.io.Serializable;

public class Int2 implements Serializable {
    public int x, y;

    public Int2() {
        x = 0;
        y = 0;
    }
    public Int2(int a) {
        x = a;
        y = a;
    }
    public Int2(int x, int y) {
        this.x = x;
        this.y = y;
    }
    public Int2(Int2 v) {
        x = v.x;
        y = v.y;
    }

    // Addition
    public Int2 add(Int2 v) {
        return new Int2(x + v.x, y + v.y);
    }

    // Subtraction
    public Int2 sub(Int2 v) {
        return new Int2(x - v.x, y - v.y);
    }

    // Multiplication
    public Int2 mul(int a) {
        return new Int2(x * a, y * a);
    }
    public Int2 mul(Int2 v) {
        return new Int2(x * v.x, y * v.y);
    }

    // Division
    public Int2 div(int a) {
        return new Int2(x / a, y / a);
    }
    public Int2 div(Int2 v) {
        return new Int2(x / v.x, y / v.y);
    }

    // Comparison
    public boolean equals(Int2 v) {
        return x == v.x && y == v.y;
    }

    // Negation
    public Int2 neg() {
        return mul(-1);
    }

    // Absolute
    public Int2 abs() {
        return new Int2(Math.abs(x), Math.abs(y));
    }

    // Constants
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

    // String
    public String toString() {
        return "(" + x + ", " + y + ")";
    }
}
