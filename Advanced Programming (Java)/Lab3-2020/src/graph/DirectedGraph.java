package graph;

import abstractions.Item;
import knapsack.Knapsack;
import knapsack.Problem;

import java.util.*;

public class DirectedGraph {
    private Problem problem = null;
    private ArrayList<Node> nodes = new ArrayList<>();
    private HashMap<Node, Integer> nodeToIndexMap = new HashMap<>();
    private HashMap<NodeIdentifier, Integer> nodeInfoToIndexMap = new HashMap<>();
    private ArrayList<Integer> degrees = new ArrayList<>();

    public DirectedGraph(Problem problem) {
        this.problem = problem;
        buildGraph(problem);
    }

    public double getLongestPath() {
        ArrayList<Item> items = problem.getItems();
        double[] longestPaths = new double[degrees.size()];
        int[] deg = new int[degrees.size()];
        for (int i = 0; i < deg.length; ++i) {
            deg[i] = degrees.get(i);
        }
        Queue<Node> queue = new LinkedList<>();
        queue.add(nodes.get(0));
        while (!queue.isEmpty()) {
            Node currentNode = queue.remove();
            int currentNodeIndex = nodeToIndexMap.get(currentNode);
            int adjacentNodes = currentNode.getEdgesCount();
            for (int i = 0; i < adjacentNodes; ++i) {
                WeightedEdge edge = currentNode.getKthEdge(i);
                Node otherNode = edge.getToNode();
                double weight = edge.getWeight();
                int otherNodeIndex = nodeToIndexMap.get(otherNode);
                longestPaths[otherNodeIndex] = Math.max(longestPaths[otherNodeIndex],
                        longestPaths[currentNodeIndex] + weight);
                --deg[otherNodeIndex];
                if (deg[otherNodeIndex] == 0) {
                    queue.add(edge.getToNode());
                }
            }
        }
        double result = longestPaths[0];
        for (int i = 1; i < longestPaths.length; ++i) {
            result = Math.max(result, longestPaths[i]);
        }
        return result;
    }

    private void buildGraph(Problem problem) {
        Knapsack knapsack = problem.getKnapsack();
        ArrayList<Item> items = problem.getItems();
        Queue<Node> queue = new LinkedList<>();
        Node node = addNodeToGraph(0, 0);
        queue.add(node);
        while (!queue.isEmpty()) {
            node = queue.remove();

            int level = node.getLevel();
            if (level == items.size()) {
                continue;
            }
            int weight = node.getWeight();

            Node otherNode = addNodeToGraph(level + 1, weight);
            node.addEdge(otherNode, 0);
            queue.add(otherNode);
            degrees.set(nodeToIndexMap.get(otherNode), degrees.get(nodeToIndexMap.get(otherNode)) + 1);

            if (weight + items.get(level).getWeight() <= knapsack.getCapacity()) {
                otherNode = addNodeToGraph(level + 1, weight + items.get(level).getWeight());
                node.addEdge(otherNode, items.get(level).getValue());
                queue.add(otherNode);
                degrees.set(nodeToIndexMap.get(otherNode), degrees.get(nodeToIndexMap.get(otherNode)) + 1);
            }
        }
    }

    private Node addNodeToGraph(int level, int weight) {
        NodeIdentifier nodeIdentifier = new NodeIdentifier(level, weight);
        if (nodeInfoToIndexMap.containsKey(nodeIdentifier)) {
            return nodes.get(nodeInfoToIndexMap.get(nodeIdentifier));
        } else {
            Node newNode = new Node(level, weight);
            nodeToIndexMap.put(newNode, nodes.size());
            nodeInfoToIndexMap.put(nodeIdentifier, nodes.size());
            nodes.add(newNode);
            degrees.add(0);
            return newNode;
        }
    }

    private static class NodeIdentifier {
        private int level;
        private int weight;

        public NodeIdentifier(int level, int weight) {
            this.level = level;
            this.weight = weight;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            NodeIdentifier that = (NodeIdentifier) o;
            return level == that.level &&
                    weight == that.weight;
        }

        @Override
        public int hashCode() {
            return Objects.hash(level, weight);
        }
    }
}
