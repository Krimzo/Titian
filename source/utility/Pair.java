package utility;

public class Pair <T> {
    public T first;
    public T second;

    public Pair() {
        first = null;
        second = null;
    }

    public Pair(T first, T second) {
        this.first = first;
        this.second = second;
    }
}
