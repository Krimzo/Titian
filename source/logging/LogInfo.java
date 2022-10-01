package logging;

import utility.Time;

public class LogInfo {
    public final LogType type;
    public final String firstMessagePart;
    public final String secondMessagePart;
    public final int senderLength;

    public LogInfo(LogType type, String sender, Object object) {
        this.type = type;
        firstMessagePart = type + " " + Time.now() + "  " + sender;
        secondMessagePart = object.toString();
        senderLength = sender.length();
    }
}
