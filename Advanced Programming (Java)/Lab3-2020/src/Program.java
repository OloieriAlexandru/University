import abstractions.*;
import knapsack.*;
import models.*;

public class Program {
    public static void main(String[] args) {
        // Compulsory
        Knapsack knapsack = new Knapsack("Olo Knapsack", 10);
        Problem statementProblem = new Problem("Statement problem", knapsack);

        Item book1 = new Book("Dragon Rising", 320, 5.0);
        Item book2 = new Book("A Blade in the Dark", 355, 5.0);
        Item food1 = new Food("Cabbage", 2);
        Item food2 = new Food("Rabbit", 2);
        Item weapon = new Weapon(WeaponType.SWORD, 5, 10);

        statementProblem.addItems(book1, book2, food1, food2, weapon);

        System.out.println(statementProblem);

        // Optional
        Algorithm greedyAlgorithm = new GreedyAlgorithm();
        System.out.print("Greedy solution profit: ");
        System.out.println(greedyAlgorithm.solveKnapsackProblem(statementProblem));

        Algorithm dynamicProgrammingAlgorithm = new DynamicProgrammingAlgorithm();
        System.out.print("Dynamic programming solution profit: ");
        System.out.println(dynamicProgrammingAlgorithm.solveKnapsackProblem(statementProblem));

        // Bonus
        Algorithm dagShortestPathAlgorithm = new DirectedAcyclicGraphAlgorithm();
        System.out.print("Weighted DAG shortest path solution profit: ");
        System.out.println(dagShortestPathAlgorithm.solveKnapsackProblem(statementProblem));

        ProblemGenerator problemGenerator = new ProblemGenerator(35, 35);
        problemGenerator.analyze(10, greedyAlgorithm, dynamicProgrammingAlgorithm, dagShortestPathAlgorithm);
    }
}
