package game.clique;

import java.util.ArrayList;
import java.util.List;

public class Graph {
    private List<Edge> edges;
    private int numberOfNodes;

    public Graph(int n) {
        numberOfNodes = n;
        edges = new ArrayList<>();
        for (int i = 1; i < n; ++i) {
            for (int j = i + 1; j <= n; ++j) {
                edges.add(new Edge(i, j));
            }
        }
    }

    /**
     * Returns the number of edges from the graph
     */
    public int getEdgesCount() {
        return edges.size();
    }

    public int getNumberOfNodes() {
        return numberOfNodes;
    }

    /**
     * Binary searches the position of the edge {firstNode, secondNode}
     */
    public int getPositionOfEdge(int firstNode, int secondNode) {
        if (firstNode > secondNode) {
            int aux = secondNode;
            secondNode = firstNode;
            firstNode = aux;
        }
        int left = 0;
        int right = edges.size() - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            Edge edge = edges.get(mid);
            if (edge.getFirstNode() == firstNode) {
                if (edge.getSecondNode() == secondNode) {
                    return mid;
                } else if (edge.getSecondNode() > secondNode) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            } else if (edge.getFirstNode() > firstNode) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        return -1;
    }

    /**
     * Returns the k'th edge
     */
    public Edge getKthEdge(int k) {
        return edges.get(k);
    }

    /**
     * Removes an edge from the graph
     */
    public void removeEdge(Edge edge) {
        int rm = -1;
        for (int i = 0; i < edges.size(); ++i) {
            if (edge.equals(edges.get(i))) {
                rm = i;
                break;
            }
        }
        if (rm == -1) {
            return;
        }
        edges.remove(rm);
    }

    /**
     * Prints all the remaining edges
     */
    public void printEdges() {
        for (int i = 0; i < edges.size(); ++i) {
            System.out.print(edges.get(i));
            if (i + 1 < edges.size()) {
                System.out.print(", ");
            }
        }
        System.out.println();
    }
}
