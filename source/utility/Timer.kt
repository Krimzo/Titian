package utility

class Timer {
    private var elapsedStart: Long
    private var deltaStart: Long
    private var deltaEnd: Long

    init {
        val currTime = System.nanoTime()
        elapsedStart = currTime
        deltaStart = currTime
        deltaEnd = currTime
    }

    fun reset() {
        updateElapsedT()
        updateDeltaT()
    }

    fun updateElapsedT() {
        elapsedStart = System.nanoTime()
    }

    fun getElapsedT(): Float {
        return (System.nanoTime() - elapsedStart) * 1e-9f
    }

    fun updateDeltaT() {
        deltaStart = deltaEnd
        deltaEnd = System.nanoTime()
    }

    fun getDeltaT(): Float {
        return (deltaEnd - deltaStart) * 1e-9f
    }
}