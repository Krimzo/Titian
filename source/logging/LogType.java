package logging;

import math.Float4;

import java.awt.*;

public enum LogType {
    INFO,
    DEBUG,
    SCRIPT,
    WARNING,
    ERROR;

    LogType() {}

    private static final Float4 infoColor = new Float4(new Color(215, 215, 215));
    private static final Float4 debugColor = new Float4(new Color(25, 175, 135));
    private static final Float4 scriptColor = new Float4(new Color(210, 185, 15));
    private static final Float4 warningColor = new Float4(new Color(225, 100, 10));
    private static final Float4 errorColor = new Float4(new Color(205, 25, 40));

    public Float4 toColor() {
        return switch (this) {
            case INFO    -> infoColor;
            case DEBUG   -> debugColor;
            case SCRIPT  -> scriptColor;
            case WARNING -> warningColor;
            case ERROR   -> errorColor;
        };
    }

    public String toString() {
        return switch (this) {
            case INFO    -> "INFO   ";
            case DEBUG   -> "DEBUG  ";
            case SCRIPT  -> "SCRIPT ";
            case WARNING -> "WARNING";
            case ERROR   -> "ERROR  ";
        };
    }
}
