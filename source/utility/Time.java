package utility;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public final class Time {
    private Time() {}

    public static String now() {
        return LocalDateTime.now().format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"));
    }
}
