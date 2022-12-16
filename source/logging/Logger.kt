package logging

import utility.Time
import utility.helper.StringHelper
import java.io.OutputStream
import java.io.PrintStream
import java.util.*

class Logger : PrintStream(object : OutputStream() { override fun write(b: Int) {} }) {
    private val maxSize = 100
    private val logs = ArrayList<LogInfo>()
    private var lastClearMessage: String? = null
    private var longestSender = 0
    private var unseenCount = 0

    init {
        clear()
    }

    fun log(log: LogInfo) {
        longestSender = Math.max(longestSender, log.senderLength)
        logs.add(log)
        if (logs.size >= maxSize) {
            logs.removeAt(0)
        }
        unseenCount += 1
    }

    fun getUnseenCount(): Int {
        return unseenCount
    }

    fun clearUnseen() {
        unseenCount = 0
    }

    fun getLastClearMessage(): String? {
        return lastClearMessage
    }

    fun formatSpaces(log: LogInfo): String? {
        return StringHelper.spaces(longestSender - log.senderLength + 2)
    }

    fun iterate(callback: (LogInfo) -> Unit) {
        for (log in logs) {
            callback(log!!)
        }
    }

    fun clear() {
        logs.clear()
        lastClearMessage = "CLEARED ON " + Time.now()
        longestSender = 0
        unseenCount = 0
    }

    private fun logSystemOut(`object`: Any) {
        log(LogInfo(LogType.INFO, "System.print", `object`))
    }

    override fun print(data: Boolean) {
        logSystemOut(data)
    }

    override fun print(data: Char) {
        logSystemOut(data)
    }

    override fun print(data: Int) {
        logSystemOut(data)
    }

    override fun print(data: Long) {
        logSystemOut(data)
    }

    override fun print(data: Float) {
        logSystemOut(data)
    }

    override fun print(data: Double) {
        logSystemOut(data)
    }

    override fun print(data: CharArray) {
        logSystemOut(String(data))
    }

    override fun print(data: String) {
        logSystemOut(data)
    }

    override fun print(data: Any) {
        logSystemOut(data)
    }

    override fun println() {
        print("")
    }

    override fun println(data: Boolean) {
        print(data)
    }

    override fun println(data: Char) {
        print(data)
    }

    override fun println(data: Int) {
        print(data)
    }

    override fun println(data: Long) {
        print(data)
    }

    override fun println(data: Float) {
        print(data)
    }

    override fun println(data: Double) {
        print(data)
    }

    override fun println(data: CharArray) {
        print(data)
    }

    override fun println(data: String) {
        print(data)
    }

    override fun println(data: Any) {
        print(data)
    }

    override fun write(b: Int) {
        print(b)
    }

    override fun write(buf: ByteArray, off: Int, len: Int) {
        print(String(buf, off, len))
    }

    override fun write(buf: ByteArray) {
        print(String(buf))
    }

    override fun writeBytes(buf: ByteArray) {
        print(String(buf))
    }

    override fun printf(format: String, vararg args: Any): PrintStream {
        print(String.format(format, *args))
        return this
    }

    override fun printf(l: Locale, format: String, vararg args: Any): PrintStream {
        print(String.format(l, format, *args))
        return this
    }

    override fun format(format: String, vararg args: Any): PrintStream {
        printf(format, *args)
        return this
    }

    override fun format(l: Locale, format: String, vararg args: Any): PrintStream {
        printf(l, format, *args)
        return this
    }

    override fun append(csq: CharSequence): PrintStream {
        print(csq.toString())
        return this
    }

    override fun append(csq: CharSequence, start: Int, end: Int): PrintStream {
        val builder = StringBuilder()
        for (i in start until end) {
            builder.append(csq[i])
        }
        print(builder.toString())
        return this
    }

    override fun append(c: Char): PrintStream {
        print(c)
        return this
    }
}