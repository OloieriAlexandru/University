package knapsack;

public class Knapsack {
    private String name;
    private int capacity;

    public Knapsack(String name, int capacity) {
        this.name = name;
        this.capacity = capacity;
    }

    public int getCapacity() {
        return capacity;
    }

    @Override
    public String toString() {
        return "Knapsack: " + name + '\n' + "Capacity of the knapsack = " + getCapacity();
    }
}
