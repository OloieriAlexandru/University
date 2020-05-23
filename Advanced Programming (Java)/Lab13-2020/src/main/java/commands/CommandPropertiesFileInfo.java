package commands;

/**
 * A class that represents the information stored in Commands.properties file about a command
 */
public class CommandPropertiesFileInfo {
    private String implementationClass;
    private String commandString;

    public CommandPropertiesFileInfo(String implementationClass, String commandString) {
        this.implementationClass = implementationClass;
        this.commandString = commandString;
    }

    public String getCommandString() {
        return commandString;
    }

    public String getImplementationClass() {
        return implementationClass;
    }
}
