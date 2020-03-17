package common;

import exceptions.InvalidCatalogFileException;

import java.util.Scanner;

public class Helper {
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

    public static void checkNull(String str) throws InvalidCatalogFileException {
        if (str == null) {
            throw new InvalidCatalogFileException();
        }
    }
}
