package common;

import java.util.Random;
import java.util.Scanner;

public class Helper {
    private static Random random = new Random();

    /**
     * Generates a random integer between min and max
     */
    public static int generateRandomInt(int min, int max) {
        if (min >= max) {
            return max;
        }
        return min + random.nextInt(max - min);
    }

    /**
     * Checks if the string str is a positive integer
     */
    public static boolean stringIsUInt(String str) {
        if (str.length() > 9 || str.length() == 0) {
            return false;
        }
        for (int i = 0; i < str.length(); ++i) {
            if (!(str.charAt(i) >= '0' && str.charAt(i) <= '9')) {
                return false;
            }
        }
        return true;
    }

    /**
     * A function used by shells
     * Reads a line from stdin
     */
    public static String readLine(Scanner scanner, String message) {
        String line;
        while (true) {
            if (message != null) {
                System.out.print(message);
            }
            line = scanner.nextLine();
            if (line.length() == 0) {
                continue;
            }
            break;
        }
        return line.trim();
    }

    /**
     * Reads a string from stdin
     * The string has to match one of the options (String[] options parameter)
     */
    public static String readStringOption(Scanner scanner, String message, String[] options) {
        while (true) {
            System.out.print("Options: {");
            for (int i = 0; i < options.length; ++i) {
                System.out.print(" " + options[i]);
                if (i + 1 < options.length) {
                    System.out.print(",");
                }
            }
            System.out.println(" }");
            System.out.print(message);
            String line = scanner.nextLine();
            for (String option : options) {
                if (option.equals(line)) {
                    return option;
                }
            }
            System.out.println("You have not introduced a valid option!");
        }
    }

    /**
     * Reads a positive integer from stdin, displaying also a message
     */
    public static int readIntOption(Scanner scanner, String message) {
        while (true) {
            System.out.print(message);
            String line = scanner.nextLine();
            try {
                int number = Integer.parseInt(line);
                if (number < 0) {
                    System.out.println("You have to introduce a positive number!");
                } else {
                    return number;
                }
            } catch (Exception e) {
                System.out.println("You have to introduce a positive number!");
            }
        }
    }
}
