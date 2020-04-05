package game.clique;

import java.util.Objects;

public class Edge implements Comparable {
    private int firstNode;
    private int secondNode;

    public Edge(int firstNode, int secondNode) {
        this.firstNode = firstNode;
        this.secondNode = secondNode;
    }

    public int getFirstNode() {
        return firstNode;
    }

    public int getSecondNode() {
        return secondNode;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Edge edge = (Edge) o;
        return firstNode == edge.firstNode &&
                secondNode == edge.secondNode;
    }

    @Override
    public int hashCode() {
        return Objects.hash(firstNode, secondNode);
    }

    @Override
    public String toString() {
        return "{" + firstNode + ", " + secondNode + "}";
    }

    @Override
    public int compareTo(Object o) {
        if (this == o) {
            return 0;
        }
        if (o == null || getClass() != o.getClass()) {
            return 1;
        }
        Edge edge = (Edge) o;
        if (firstNode == edge.firstNode) {
            return Integer.compare(secondNode, edge.secondNode);
        }
        return Integer.compare(firstNode, edge.firstNode);
    }
}
