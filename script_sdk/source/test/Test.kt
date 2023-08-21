package test

import script.SpinScript

fun main() {
    val script = SpinScript()

    script.start()
    for (i in 0 until 5) {
        script.update()
    }
}
