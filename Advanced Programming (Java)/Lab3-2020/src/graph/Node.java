package graph;

import java.util.ArrayList;

public class Node {
    private ArrayList<WeightedEdge> adjacentNodes = new ArrayList<>();
    private int level;
    private int weight;

    public Node(int level, int weight) {
        this.level = level;
        this.weight = weight;
    }

    public int getLevel() {
        return level;
    }

    public int getWeight() {
        return weight;
    }

    public int getEdgesCount() {
        return adjacentNodes.size();
    }

    public WeightedEdge getKthEdge(int k) {
        if (k < 0 || k >= adjacentNodes.size()) {
            return null;
        }
        return adjacentNodes.get(k);
    }

    void addEdge(Node node, double weight) {
        adjacentNodes.add(new WeightedEdge(node, weight));
    }
}
