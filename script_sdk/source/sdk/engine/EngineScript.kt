package sdk.engine

abstract class EngineScript protected constructor(protected val state: EngineState) {
    abstract fun start()
    abstract fun update()
}
