package common;

import java.util.Random;

public class RandomGenerator {
    private static Random random = new Random();

    public static int intBetween(int min, int max) {
        return random.nextInt(max - min + 1) + min;
    }

    public static double doubleBetween(int min, int max) {
        return (max - min) * random.nextDouble() + min;
    }
}
