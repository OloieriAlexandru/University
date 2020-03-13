package abstractions;

import knapsack.Problem;
import knapsack.Solution;

public interface Algorithm {
    String getName();

    Solution solveKnapsackProblem(Problem problem);
}
