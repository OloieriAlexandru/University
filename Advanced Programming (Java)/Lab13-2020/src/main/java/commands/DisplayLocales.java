package commands;

import helper.Helper;

import java.util.Locale;

public class DisplayLocales implements Command {
    private static String[] localesStr = new String[]{
            "en", "ro_RO"
    };

    /**
     * Displays all the supported locales
     */
    public Locale execute(Locale locale) {
        System.out.println(Helper.loadPropertyFromMessagesFile(locale, "locales"));

        for (String localeStr : localesStr) {
            System.out.println(localeStr);
        }

        return locale;
    }
}
