package knapsack;

import abstractions.Item;
import graph.DirectedGraph;

import java.util.ArrayList;
import java.util.Arrays;

public class Problem {
    private String name;
    private ArrayList<Item> items = new ArrayList<>();
    private Knapsack knapsack = null;

    public Problem(String name, Knapsack knapsack) {
        this.name = name;
        this.knapsack = knapsack;
    }

    public void addItems(Item... newItems) {
        items.addAll(Arrays.asList(newItems));
    }

    public Knapsack getKnapsack() {
        return knapsack;
    }

    public ArrayList<Item> getItems() {
        return items;
    }

    public DirectedGraph getAssociatedGraph() {
        if (knapsack == null) {
            return null;
        }
        return new DirectedGraph(this);
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();

        stringBuilder.append("Problem ").append(name).append('\n');
        stringBuilder.append(knapsack.toString()).append('\n');
        stringBuilder.append("Available items:\n");

        items.sort((item1, item2) -> item1.getName().compareTo(item2.getName()));

        for (int i = 0; i < items.size(); ++i) {
            stringBuilder.append('\t').append(items.get(i).toString()).append('\n');
        }

        return stringBuilder.toString();
    }
}
