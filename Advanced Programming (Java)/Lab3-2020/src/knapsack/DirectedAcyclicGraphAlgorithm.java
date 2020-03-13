package knapsack;

import abstractions.Algorithm;
import graph.DirectedGraph;

public class DirectedAcyclicGraphAlgorithm implements Algorithm {
    @Override
    public String getName() {
        return "DAG Shortest Path Algorithm";
    }

    @Override
    public Solution solveKnapsackProblem(Problem problem) {
        DirectedGraph graph = problem.getAssociatedGraph();
        if (graph == null) {
            return new Solution(0.0, 0.0);
        }
        double value = graph.getLongestPath();
        double memory = Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory();
        return new Solution(value, memory);
    }
}
