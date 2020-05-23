package commands;

import helper.Helper;

import java.text.MessageFormat;
import java.util.Locale;
import java.util.Scanner;

public class SetLocale implements Command {
    private static Locale[] locales;

    static {
        locales = Locale.getAvailableLocales();
    }

    /**
     * Changes the current locale, if the string introduced by the user matches one of the supported locales
     */
    public Locale execute(Locale locale) {
        String msg = Helper.loadPropertyFromMessagesFile(locale, "locale.set");

        String line = Helper.readLine(new Scanner(System.in), "Enter the new locale: ");
        for (Locale loc : locales) {
            if (loc.toString().equals(line)) {
                locale = loc;
            }
        }

        String formattedMsg = new MessageFormat(msg).format(new Object[]{locale.getCountry()});
        System.out.println(formattedMsg);

        return locale;
    }
}
