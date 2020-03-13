package knapsack;

import abstractions.Algorithm;
import abstractions.Item;
import common.Helper;
import common.RandomGenerator;

import java.util.Date;

public class ProblemGenerator {
    private final int knapsackCapacity = 100;
    private int minimumNumberOfItems;
    private int maximumNumberOfItems;

    public ProblemGenerator(int minimumNumberOfItems, int maximumNumberOfItems) {
        this.minimumNumberOfItems = minimumNumberOfItems;
        this.maximumNumberOfItems = maximumNumberOfItems;
    }

    public void analyze(int runs, Algorithm... algorithms) {
        double[] runningTimes = new double[algorithms.length];
        double[] solutionsSum = new double[algorithms.length];
        double[] memoryUsed = new double[algorithms.length];
        int numberOfItems = RandomGenerator.intBetween(minimumNumberOfItems, maximumNumberOfItems);
        for (int i = 0; i < runs; ++i) {
            Problem thisProblem = generateProblem(numberOfItems);
            for (int j = 0; j < algorithms.length; ++j) {
                Date startTime = new Date();
                Solution thisSolution = algorithms[j].solveKnapsackProblem(thisProblem);
                solutionsSum[j] += thisSolution.getBestValue();
                memoryUsed[j] += thisSolution.getMemoryUsed();
                Date now = new Date();
                double runningTime = (now.getTime() - startTime.getTime()) / 1000.0;
                runningTimes[j] += runningTime;
            }
            System.out.println("Finished problem " + i);
        }
        System.out.println(runs + " problems were generated, each having " + numberOfItems + " items");
        printStatistics(runningTimes, solutionsSum, memoryUsed, runs, algorithms);
    }

    private void printStatistics(double[] runningTimes, double[] solutionsSum, double[] memoryUsed,
                                 int runs, Algorithm... algorithms) {
        System.out.println("Statistics:");
        for (int i = 0; i < algorithms.length; ++i) {
            runningTimes[i] /= runs;
            solutionsSum[i] /= runs;
            memoryUsed[i] /= runs;
            memoryUsed[i] /= (1024 * 1024 * 1024);
            System.out.println("Algorithm: " + algorithms[i].getName());
            System.out.println("\tAverage solution: " + Helper.roundToNDecimals(solutionsSum[i], 6));
            System.out.println("\tAverage running time: " + Helper.roundToNDecimals(runningTimes[i], 6));
            System.out.println("\tAverage used memory (GB): " + Helper.roundToNDecimals(memoryUsed[i], 6));
        }
    }

    private Problem generateProblem(int numberOfItems) {
        Knapsack knapsack = new Knapsack("Generated knapsack", knapsackCapacity);
        Problem generatedProblem = new Problem("Random generated problem", knapsack);

        double avg = (knapsackCapacity * 6) / (double) numberOfItems;
        int eps = (int) (avg * 20.0 / 100.0);
        int minWeight = Math.max(1, (int) avg - eps);
        int maxWeight = (int) avg + eps;
        int minValue = Math.max(1, minWeight - (minWeight * 50 / 100));
        int maxValue = maxWeight + (maxWeight * 50 / 100);
        for (int i = 0; i < numberOfItems; ++i) {
            int itemType = RandomGenerator.intBetween(0, 2);
            Item item = ItemFactory.getItem(itemType, minWeight, maxWeight, minValue, maxValue);
            generatedProblem.addItems(item);
        }

        return generatedProblem;
    }
}
