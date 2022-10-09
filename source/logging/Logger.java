package logging;

import callback.LogInfoCallback;
import utility.Strings;
import utility.Time;
import utility.nncollection.NNArrayList;

import java.io.*;
import java.util.Locale;

public class Logger extends PrintStream {
    public int maxSize = 100;

    private final NNArrayList<LogInfo> logs = new NNArrayList<>();
    private String lastClearMessage;
    private int longestSender;
    private int unseenCount = 0;

    public Logger() {
        super(new OutputStream() { public void write(int b) {} });
        clear();
    }

    public void log(LogInfo log) {
        longestSender = Math.max(longestSender, log.senderLength);
        logs.add(log);

        if (logs.size() >= maxSize) {
            logs.remove(0);
        }

        unseenCount += 1;
    }

    public int getUnseenCount() {
        return unseenCount;
    }

    public void clearUnseen() {
        unseenCount = 0;
    }

    public String getLastClearMessage() {
        return lastClearMessage;
    }

    public String formatSpaces(LogInfo log) {
        return Strings.spaces(longestSender - log.senderLength + 2);
    }

    public void iterate(LogInfoCallback callback) {
        for (LogInfo log : logs) {
            callback.method(log);
        }
    }

    public void clear() {
        logs.clear();
        lastClearMessage = "CLEARED ON " + Time.now();
        longestSender = 0;
        unseenCount = 0;
    }

    private void logSystemOut(Object object) {
        log(new LogInfo(LogType.INFO, "System.print", object));
    }

    @Override
    public void print(boolean data) {
        logSystemOut(data);
    }

    @Override
    public void print(char data) {
        logSystemOut(data);
    }

    @Override
    public void print(int data) {
        logSystemOut(data);
    }

    @Override
    public void print(long data) {
        logSystemOut(data);
    }

    @Override
    public void print(float data) {
        logSystemOut(data);
    }

    @Override
    public void print(double data) {
        logSystemOut(data);
    }

    @Override
    public void print(char[] data) {
        logSystemOut(String.valueOf(data));
    }

    @Override
    public void print(String data) {
        logSystemOut(data);
    }

    @Override
    public void print(Object data) {
        logSystemOut(data);
    }

    @Override
    public void println() {
        print("");
    }

    @Override
    public void println(boolean data) {
        print(data);
    }

    @Override
    public void println(char data) {
        print(data);
    }

    @Override
    public void println(int data) {
        print(data);
    }

    @Override
    public void println(long data) {
        print(data);
    }

    @Override
    public void println(float data) {
        print(data);
    }

    @Override
    public void println(double data) {
        print(data);
    }

    @Override
    public void println(char[] data) {
        print(data);
    }

    @Override
    public void println(String data) {
        print(data);
    }

    @Override
    public void println(Object data) {
        print(data);
    }

    @Override
    public void write(int b) {
        print(b);
    }

    @Override
    public void write(byte[] buf, int off, int len) {
        print(new String(buf, off, len));
    }

    @Override
    public void write(byte[] buf) {
        print(new String(buf));
    }

    @Override
    public void writeBytes(byte[] buf) {
        print(new String(buf));
    }

    @Override
    public PrintStream printf(String format, Object... args) {
        print(String.format(format, args));
        return this;
    }

    @Override
    public PrintStream printf(Locale l, String format, Object... args) {
        print(String.format(l, format, args));
        return this;
    }

    @Override
    public PrintStream format(String format, Object... args) {
        printf(format, args);
        return this;
    }

    @Override
    public PrintStream format(Locale l, String format, Object... args) {
        printf(l, format, args);
        return this;
    }

    @Override
    public PrintStream append(CharSequence csq) {
        print(csq.toString());
        return this;
    }

    @Override
    public PrintStream append(CharSequence csq, int start, int end) {
        StringBuilder builder = new StringBuilder();
        for (int i = start; i < end; i++) {
            builder.append(csq.charAt(i));
        }
        print(builder.toString());
        return this;
    }

    @Override
    public PrintStream append(char c) {
        print(c);
        return this;
    }
}
