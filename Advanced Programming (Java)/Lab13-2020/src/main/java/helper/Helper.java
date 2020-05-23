package helper;

import java.util.Locale;
import java.util.ResourceBundle;
import java.util.Scanner;

public class Helper {
    /**
     * A function used by shells
     * Reads a line from stdin
     */
    public static String readLine(Scanner scanner, String message) {
        String line;
        while (true) {
            if (message != null) {
                System.out.print(message);
            }
            line = scanner.nextLine();
            if (line.length() == 0) {
                continue;
            }
            break;
        }
        return line.trim();
    }

    /**
     * Returns a property from the specific locale Messages.properties file
     */
    public static String loadPropertyFromMessagesFile(Locale locale, String property) {
        ResourceBundle messages = ResourceBundle.getBundle("Messages", locale);
        return messages.getString(property);
    }
}
