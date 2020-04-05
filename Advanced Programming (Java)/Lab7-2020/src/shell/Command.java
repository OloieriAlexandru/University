package shell;

public class Command {
    private String command;
    private CommandEnum cmdEnum;

    public Command(CommandEnum cmdEnum, String command) {
        this.cmdEnum = cmdEnum;
        this.command = command;
    }

    public CommandEnum getCmdEnum() {
        return cmdEnum;
    }

    public String getCommand() {
        return command;
    }

    @Override
    public String toString() {
        return command;
    }
}
