package utility

import java.time.LocalDateTime
import java.time.format.DateTimeFormatter

object Time {
    fun now(): String {
        return LocalDateTime.now().format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"))
    }
}