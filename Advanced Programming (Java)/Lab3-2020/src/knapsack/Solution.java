package knapsack;

public class Solution {
    private double bestValue;
    private double memoryUsed;

    public Solution(double bestValue, double memoryUsed) {
        this.bestValue = bestValue;
        this.memoryUsed = memoryUsed;
    }

    public double getBestValue() {
        return bestValue;
    }

    public double getMemoryUsed() {
        return memoryUsed;
    }

    @Override
    public String toString() {
        return String.valueOf(bestValue);
    }
}
