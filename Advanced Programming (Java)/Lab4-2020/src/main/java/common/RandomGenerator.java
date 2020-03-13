package common;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;

public class RandomGenerator {
    private static Random random = new Random();

    public static int intBetween(int min, int max) {
        if (min > max){
            return 0;
        }
        return random.nextInt(max - min + 1) + min;
    }

    public static List<Integer> getPermutationOfKNumbers(int k){
        List<Integer> result = new ArrayList<>();
        for (int i=0;i<k;++i){
            result.add(i);
        }
        Collections.shuffle(result);
        return result;
    }
}
