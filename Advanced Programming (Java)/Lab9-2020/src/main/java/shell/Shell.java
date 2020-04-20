package shell;

import util.Helper;

import java.util.ArrayList;
import java.util.List;

public class Shell {
    private List<Command> commands = new ArrayList<>();

    /**
     * Returns the command that has to be executed
     */
    public Command getCommand(String cmd) {
        if (Helper.stringIsUInt(cmd)) {
            int idx = Integer.parseInt(cmd) - 1;
            if (idx >= 0 && idx < commands.size()) {
                return commands.get(idx);
            }
        } else {
            for (Command command : commands) {
                if (command.getCommand().equals(cmd)) {
                    return command;
                }
            }
        }
        return null;
    }

    public void addCommand(Command command) {
        commands.add(command);
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();

        for (int i = 0; i < commands.size(); ++i) {
            stringBuilder.append((i + 1 < 10 ? " " : "")).append((i + 1));
            stringBuilder.append(". ").append(commands.get(i).toString());
            if (i + 1 < commands.size()) {
                stringBuilder.append('\n');
            }
        }

        return stringBuilder.toString();
    }
}
