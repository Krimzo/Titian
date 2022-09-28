package utility;

public class Timer {
    private long elapsedStart;
    private long deltaStart;
    private long deltaEnd;

    public Timer() {
        long currTime = System.nanoTime();
        elapsedStart = currTime;
        deltaStart = currTime;
        deltaEnd = currTime;
    }

    public void reset() {
        updateElapsedT();
        updateDeltaT();
    }

    public void updateElapsedT() {
        elapsedStart = System.nanoTime();
    }
    public float getElapsedT() {
        return (System.nanoTime() - elapsedStart) * 1e-9f;
    }

    public void updateDeltaT() {
        deltaStart = deltaEnd;
        deltaEnd = System.nanoTime();
    }
    public float getDeltaT() {
        return (deltaEnd - deltaStart) * 1e-9f;
    }
}
