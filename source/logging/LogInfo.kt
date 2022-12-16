package logging

import utility.Time

class LogInfo(val type: LogType, sender: String, `object`: Any) {
    val firstMessagePart: String
    val secondMessagePart: String
    val senderLength: Int

    init {
        firstMessagePart = type.toString() + " " + Time.now() + "  " + sender
        secondMessagePart = `object`.toString()
        senderLength = sender.length
    }
}