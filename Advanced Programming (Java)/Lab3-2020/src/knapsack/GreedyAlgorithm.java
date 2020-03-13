package knapsack;

import abstractions.Algorithm;
import abstractions.Item;

import java.util.ArrayList;

public class GreedyAlgorithm implements Algorithm {
    @Override
    public String getName() {
        return "Greedy Algorithm";
    }

    @Override
    public Solution solveKnapsackProblem(Problem problem) {
        Knapsack knapsack = problem.getKnapsack();
        if (knapsack == null) {
            System.out.println("Invalid problem! The knapsack is null!");
            return new Solution(0.0, 0.0);
        }
        ArrayList<Item> items = problem.getItems();
        items.sort((item1, item2) -> Double.compare(item2.profitFactor(), item1.profitFactor()));
        double solution = 0.0;
        int currentWeight = 0;
        for (int i = 0; i < items.size(); ++i) {
            if (items.get(i).getWeight() + currentWeight <= knapsack.getCapacity()) {
                solution += items.get(i).getValue();
                currentWeight += items.get(i).getWeight();
            }
        }
        double memory = Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory();
        return new Solution(solution, memory);
    }
}
