package logging;

import callback.LogInfoCallback;
import utility.Strings;
import utility.Time;

import java.io.*;
import java.util.ArrayList;

public class Logger extends PrintStream {
    public int maxSize = 100;
    private final ArrayList<LogInfo> logs = new ArrayList<>();
    private String lastClearMessage;
    private int longestSender;

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
    }

    public String getLastClearMessage() {
        return lastClearMessage;
    }

    public String convertInfo(LogInfo log) {
        return log.firstMessagePart + Strings.spaces(longestSender - log.senderLength + 2) + log.secondMessagePart;
    }

    public void iterate(LogInfoCallback callback) {
        for (LogInfo log : logs) {
            callback.method(log);
        }
    }

    public void clear() {
        logs.clear();
        lastClearMessage = "------ CLEARED ON " + Time.now() + " ------";
        longestSender = 0;
    }

    private void logSystemOut(Object object) {
        log(new LogInfo(LogType.INFO, "System.out", object));
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
}
