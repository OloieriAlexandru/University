package util;

import java.util.Scanner;

public class Helper {
    /**
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

    public static Pair<Integer, Integer> splitMoveString(String move) {
        String[] aux = move.split(" ");
        return new Pair<>(Integer.parseInt(aux[0]), Integer.parseInt(aux[1]));
    }
}
