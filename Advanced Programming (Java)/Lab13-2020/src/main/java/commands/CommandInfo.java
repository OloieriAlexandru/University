package commands;

import app.LocaleExplore;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Locale;
import java.util.Properties;

/**
 * A class that represents a command
 */
public class CommandInfo {
    private String propertiesFileName;
    private String name;
    private Command instantiatedObject;
    private Method method;

    public CommandInfo(String propertiesFileName) {
        this.propertiesFileName = propertiesFileName;
    }

    /**
     * Loads properties about the command from Commands.properties file
     * And creates a Method object that will later be used to execute the command
     */
    public boolean load() {
        try {
            CommandPropertiesFileInfo commandInfo = getCommandInfo();
            if (commandInfo == null) {
                return false;
            }
            name = commandInfo.getCommandString();

            Class clazz = Class.forName(commandInfo.getImplementationClass());
            instantiatedObject = (Command) clazz.newInstance();
            method = instantiatedObject.getClass().getMethod("execute", Locale.class);
        } catch (ClassNotFoundException | IllegalAccessException | InstantiationException | NoSuchMethodException e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    /**
     * Executes the command on the object "instantiatedObject"
     */
    public Locale run(Locale locale) {
        try {
            return (Locale) method.invoke(instantiatedObject, locale);
        } catch (IllegalAccessException | InvocationTargetException e) {
            e.printStackTrace();
            return locale;
        }
    }

    /**
     * Loads the Commands.properties file and reads the two information provided about the command
     */
    private CommandPropertiesFileInfo getCommandInfo() {
        CommandPropertiesFileInfo res = null;
        Properties properties = new Properties();
        InputStream stream = LocaleExplore.class.getResourceAsStream("/Commands.properties");
        try {
            properties.load(stream);
            res = new CommandPropertiesFileInfo(properties.getProperty(propertiesFileName + ".impl"), properties.getProperty(propertiesFileName + ".cmd"));
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
        return res;
    }

    /**
     * Returns the name of the command, that is, the string that the user has to introduce in order to run this command
     */
    public String getName() {
        return name;
    }
}
