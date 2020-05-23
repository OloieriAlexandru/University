package commands;

import java.util.Locale;

public interface Command {
    Locale execute(Locale locale);
}
