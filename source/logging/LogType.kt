package logging

import math.Float3
import java.awt.Color

enum class LogType {
    INFO, DEBUG, SCRIPT, WARNING, ERROR;

    fun toColor(): Float3 {
        return when (this) {
            INFO -> infoColor
            DEBUG -> debugColor
            SCRIPT -> scriptColor
            WARNING -> warningColor
            ERROR -> errorColor
        }
    }

    override fun toString(): String {
        return when (this) {
            INFO -> "INFO   "
            DEBUG -> "DEBUG  "
            SCRIPT -> "SCRIPT "
            WARNING -> "WARNING"
            ERROR -> "ERROR  "
        }
    }

    companion object {
        private val infoColor = Float3(Color(215, 215, 215))
        private val debugColor = Float3(Color(64, 194, 159))
        private val scriptColor = Float3(Color(203, 189, 89))
        private val warningColor = Float3(Color(210, 126, 66))
        private val errorColor = Float3(Color(204, 63, 75))
    }
}