package cmd;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class GameServerCommands {
    private ArrayList<ServerCommand> commands = null;

    public GameServerCommands() {
        commands = new ArrayList<>();
    }

    /**
     * Adds a command with a description
     */
    public void addCommand(String cmd, String description) {
        commands.add(new ServerCommand(cmd, description));
    }

    public void addCommand(String cmd) {
        commands.add(new ServerCommand(cmd));
    }

    public int getCommandsCount() {
        return commands.size();
    }

    /**
     * toString() over the network
     */
    public void writeToNetworkCommands(PrintWriter out) {
        out.println(commands.size());
        for (int i = 0; i < commands.size(); ++i) {
            commands.get(i).printNetworkCommand(i + 1, out);
        }
        out.flush();
    }

    /**
     * Gets the index of the command which starts with "cmdPrefix"
     */
    public int getCommandIndex(String cmdPrefix) {
        for (int i = 0; i < commands.size(); ++i) {
            if (cmdPrefix.startsWith(commands.get(i).getCommand())) {
                return i;
            }
        }
        return -1;
    }

    /**
     * Extracts the arguments from the command "cmd", knowing that it starts with the prefix of command number cmdIndex
     */
    public String getArguments(String cmd, int cmdIndex) {
        if (cmdIndex == -1) {
            return "";
        }
        return cmd.substring(commands.get(cmdIndex).getCommand().length());
    }

    public List<String> getCommandsDescription() {
        List<String> commands = new ArrayList<>();

        for (int i = 0; i < this.commands.size(); ++i) {
            commands.add((i + 1) + ". " + this.commands.get(i).toString());
        }

        return commands;
    }
}
