package logging;

import math.Float3;

import java.awt.*;

public enum LogType {
    INFO,
    DEBUG,
    SCRIPT,
    WARNING,
    ERROR;

    LogType() {}

    private static final Float3 infoColor = new Float3(new Color(215, 215, 215));
    private static final Float3 debugColor = new Float3(new Color(64, 194, 159));
    private static final Float3 scriptColor = new Float3(new Color(203, 189, 89));
    private static final Float3 warningColor = new Float3(new Color(210, 126, 66));
    private static final Float3 errorColor = new Float3(new Color(204, 63, 75));

    public Float3 toColor() {
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
