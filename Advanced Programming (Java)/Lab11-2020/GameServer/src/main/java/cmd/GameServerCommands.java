package cmd;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class GameServerCommands {
    private ArrayList<ServerCommand> commands = null;
    private int commandsThatRequireLogin = 0;

    public GameServerCommands() {
        commands = new ArrayList<>();
    }

    /**
     * Adds a command with a description
     */
    public void addCommand(String cmd, String description, boolean loginRequired) {
        commands.add(new ServerCommand(cmd, description, loginRequired));
        if (loginRequired) {
            ++commandsThatRequireLogin;
        }
    }

    public void addCommand(String cmd, boolean loginRequired) {
        commands.add(new ServerCommand(cmd, loginRequired));
        if (loginRequired) {
            ++commandsThatRequireLogin;
        }
    }

    public int getCommandsCount() {
        return commands.size();
    }

    /**
     * toString() over the network
     */
    public void writeToNetworkCommands(PrintWriter out, boolean loggedIn) {
        if (loggedIn) {
            writeLoggedInCommandsToNetwork(out);
        } else {
            writeLoggedOutCommandsToNetwork(out);
        }
    }

    private void writeLoggedInCommandsToNetwork(PrintWriter out) {
        out.println(commandsThatRequireLogin);
        int index = 1;
        for (int i = 0; i < commands.size(); ++i) {
            if (commands.get(i).isLoginRequired()) {
                commands.get(i).printNetworkCommand(index, out);
                ++index;
            }
        }
        out.flush();
    }

    private void writeLoggedOutCommandsToNetwork(PrintWriter out) {
        out.println(commands.size() - commandsThatRequireLogin);
        int index = 1;
        for (int i = 0; i < commands.size(); ++i) {
            if (!commands.get(i).isLoginRequired()) {
                commands.get(i).printNetworkCommand(index, out);
                ++index;
            }
        }
        out.flush();
    }

    /**
     * Gets the index of the command which starts with "cmdPrefix"
     */
    public int getCommandIndex(String cmdPrefix, boolean loggedIn) {
        for (int i = 0; i < commands.size(); ++i) {
            if (cmdPrefix.startsWith(commands.get(i).getCommand()) && commands.get(i).isLoginRequired() == loggedIn) {
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

    public List<String> getCommandsDescription(boolean loggedIn) {
        List<String> returnedCommands = new ArrayList<>();

        int index = 1;
        for (int i = 0; i < commands.size(); ++i) {
            if (commands.get(i).isLoginRequired() == loggedIn) {
                returnedCommands.add((index) + ". " + commands.get(i).toString());
                ++index;
            }
        }

        return returnedCommands;
    }
}
