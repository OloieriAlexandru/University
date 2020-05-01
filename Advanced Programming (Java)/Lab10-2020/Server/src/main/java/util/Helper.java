package util;

import java.util.Random;

public class Helper {
    private static Random random = new Random();

    public static String getRandomHexString(int length) {
        StringBuilder stringBuilder = new StringBuilder();

        while(stringBuilder.length() < length) {
            stringBuilder.append(Integer.toHexString(random.nextInt()));
        }

        return stringBuilder.toString().substring(0, length);
    }

    public static String[] splitString(String str) {
        return str.trim().split("\\s+", 1);
    }

    public static Pair<Integer, Integer> splitMoveString(String move) {
        String[] aux = move.split(" ");
        return new Pair<>(Integer.parseInt(aux[0]), Integer.parseInt(aux[1]));
    }
}
