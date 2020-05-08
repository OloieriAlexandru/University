package cmd;

import java.io.PrintWriter;

public class ServerCommand {
    private String command;
    private String description;
    private boolean loginRequired;

    public ServerCommand(String command, boolean loginRequired) {
        this.command = command;
        this.description = command;
        this.loginRequired = loginRequired;
    }

    public ServerCommand(String command, String description, boolean loginRequired) {
        this.command = command;
        this.description = description;
        this.loginRequired = loginRequired;
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

    public boolean isLoginRequired() {
        return loginRequired;
    }

    @Override
    public String toString() {
        return description;
    }
}
