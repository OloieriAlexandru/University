package knapsack;

import abstractions.Algorithm;
import abstractions.Item;

import java.util.ArrayList;

public class DynamicProgrammingAlgorithm implements Algorithm {
    @Override
    public String getName() {
        return "Dynamic Programming Algorithm";
    }

    @Override
    public Solution solveKnapsackProblem(Problem problem) {
        Knapsack knapsack = problem.getKnapsack();
        if (knapsack == null) {
            System.out.println("Invalid problem! The knapsack is null!");
            return new Solution(0.0, 0.0);
        }
        ArrayList<Item> items = problem.getItems();
        if (items.size() == 0) {
            return new Solution(0.0, 0.0);
        }
        double[][] best = new double[2][knapsack.getCapacity() + 1];
        int lastLine = 0, currentLine = 1;
        if (items.get(0).getWeight() <= knapsack.getCapacity()) {
            best[0][items.get(0).getWeight()] = items.get(0).getValue();
        }
        for (int i = 1; i < items.size(); ++i) {
            for (int j = knapsack.getCapacity(); j > 0; --j) {
                best[currentLine][j] = best[lastLine][j];
                if (j - items.get(i).getWeight() < 0) {
                    continue;
                }
                if (best[lastLine][j - items.get(i).getWeight()] == 0.0) {
                    if (j - items.get(i).getWeight() > 0) {
                        continue;
                    }
                }
                if (best[currentLine][j] < best[lastLine][j - items.get(i).getWeight()] + items.get(i).getValue()) {
                    best[currentLine][j] = best[lastLine][j - items.get(i).getWeight()] + items.get(i).getValue();
                }
            }
            lastLine = 1 - lastLine;
            currentLine = 1 - currentLine;
        }
        double solution = 0.0;
        for (int i = 0; i < best[lastLine].length; ++i) {
            if (best[lastLine][i] > solution) {
                solution = best[lastLine][i];
            }
        }
        double memory = Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory();
        return new Solution(solution, memory);
    }
}
