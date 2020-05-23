package app;

import commands.CommandInfo;
import helper.Helper;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Scanner;

public class LocaleExplore {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        List<CommandInfo> commands = initCommands();

        Locale locale = Locale.getDefault();
        boolean running = true;
        while (running) {
            String line = Helper.readLine(scanner, Helper.loadPropertyFromMessagesFile(locale, "prompt"));
            locale = runCommand(line, locale, commands);
            System.out.println();
        }
    }

    /**
     * Checks whether the input introduced by user corresponds to a command and if it does, the command is run
     */
    private static Locale runCommand(String command, Locale locale, List<CommandInfo> commands) {
        for (CommandInfo cmd : commands) {
            if (cmd.getName().equals(command)) {
                Locale newLocale = cmd.run(locale);
                if (newLocale == null) {
                    return locale;
                }
                return newLocale;
            }
        }
        System.out.println(Helper.loadPropertyFromMessagesFile(locale, "invalid"));
        return locale;
    }

    /**
     * Initializes the commands
     */
    private static List<CommandInfo> initCommands() {
        String[] commandsPropertiesFileNames = new String[]{
                "display-command",
                "set-command",
                "info-command"
        };
        List<CommandInfo> commands = new ArrayList<CommandInfo>();

        for (String str : commandsPropertiesFileNames) {
            CommandInfo commandInfo = new CommandInfo(str);
            if (commandInfo.load()) {
                commands.add(commandInfo);
            }
        }

        return commands;
    }
}
