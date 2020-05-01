package cmd;

import java.io.PrintWriter;

public class ServerCommand {
    private String command;
    private String description;

    public ServerCommand(String command) {
        this.command = command;
        this.description = command;
    }

    public ServerCommand(String command, String description) {
        this.command = command;
        this.description = description;
    }

    /**
     * toString over network
     */
    public void printNetworkCommand(int index, PrintWriter out) {
        out.println(index + ". " + description);
    }

    public String getCommand() {
        return command;
    }

    @Override
    public String toString() {
        return description;
    }
}
