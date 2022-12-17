package logging

import utility.Time

class LogInfo(val type: LogType, sender: String, obj: Any) {
    val firstMessagePart: String
    val secondMessagePart: String
    val senderLength: Int

    init {
        firstMessagePart = type.toString() + " " + Time.now() + "  " + sender
        secondMessagePart = obj.toString()
        senderLength = sender.length
    }
}