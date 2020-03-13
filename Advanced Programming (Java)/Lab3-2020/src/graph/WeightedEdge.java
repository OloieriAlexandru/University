package graph;

public class WeightedEdge {
    private Node toNode;
    private double weight;

    WeightedEdge(Node node, double weight) {
        this.toNode = node;
        this.weight = weight;
    }

    public double getWeight() {
        return weight;
    }

    public Node getToNode() {
        return toNode;
    }
}
