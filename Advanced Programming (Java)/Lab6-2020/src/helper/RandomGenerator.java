package helper;

import java.util.Random;

public class RandomGenerator {
    private static Random random = new Random();

    /**
     * Generates a random double value
     * In this app, the random values are only used for the Spring Shape Graph Drawing Algorithm
     */
    public static double getRandomValue() {
        return random.nextDouble();
    }
}
