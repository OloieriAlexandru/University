package mdvsp.graph;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.PriorityQueue;
import java.util.Queue;

public class DirectedGraph {
    private ArrayList<Node> nodes = null;

    /**
     * The only available constructor - creates a graph with a specified number of nodes
     * @param nodesCount the number of nodes in the graph
     */
    public DirectedGraph(int nodesCount){
        nodes = new ArrayList<>();
        for (int i=0;i<nodesCount;++i){
            nodes.add(new Node(i));
        }
    }

    /**
     * Adds a directed weighted edge in the graph
     * @param node1 the first node
     * @param node2 the second node
     * @param cost the cost of the edge
     */
    public void addWeightedEdge(int node1, int node2, int cost){
        if (!checkIfTheNodeExists(node1) || !checkIfTheNodeExists(node2)){
            return;
        }
        nodes.get(node1).addAdjacentNode(nodes.get(node2), cost);
    }

    /**
     * Returns an array containing the cost of a minimum cost path starting from a specified note
     * @param node the start node
     * @param nLog true if the used algorithm should be Dijkstra, false if the used algorithm should be Bellman-Ford
     * @return a minimum-cost array
     */
    public int[] getShortestPathsFromNodeK(int node, boolean nLog){
         int[] bestCost = new int[nodes.size()];
         for (int i=0;i<nodes.size();++i){
            bestCost[i] = Integer.MAX_VALUE;
         }

         if (nLog){
             dijkstra(node, bestCost);
         } else {
             bellmanFord(node, bestCost);
         }

         for (int i=0;i<nodes.size();++i){
             if (bestCost[i] == Integer.MAX_VALUE){
                  bestCost[i] = -1;
             }
         }
         return bestCost;
    }

    /**
     * Runs the Bellman-Ford algorithm on the graph, starting with a specified node
     * @param node the start node
     * @param bestCost the array where the minimum costs will be written
     */
    private void bellmanFord(int node, int[] bestCost){
        Queue<Integer> queue = new LinkedList<>();
        boolean[] inQueue = new boolean[nodes.size()];
        queue.add(node);
        bestCost[node] = 0;
        while (!queue.isEmpty()){
            int currentNode = queue.remove();
            int adjacentNodesCount = nodes.get(currentNode).getAdjacentNodesCount();
            inQueue[currentNode] = false;
            for (int i=0;i<adjacentNodesCount;++i){
                Node adjacentNode = nodes.get(currentNode).getKthAdjacentNode(i);
                int cost = nodes.get(currentNode).getKthAdjacentNodeCost(i);
                int nodeIndex = adjacentNode.getIndex();
                if (bestCost[nodeIndex] > bestCost[currentNode] + cost){
                    bestCost[nodeIndex] = bestCost[currentNode] + cost;
                    if (!inQueue[nodeIndex]){
                        inQueue[nodeIndex] = true;
                        queue.add(nodeIndex);
                    }
                }
            }
        }
    }

    /**
     * Runs the Dijkstra algorithm on the graph, starting with a specified node
     * @param node the start node
     * @param bestCost the array where the minimum costs will be written
     */
    private void dijkstra(int node, int[] bestCost){
        PriorityQueue<PriorityQueueEntry> pairPriorityQueue = new PriorityQueue<>();
        bestCost[node] = 0;
        pairPriorityQueue.add(new PriorityQueueEntry(0, node));
        pairPriorityQueue.add(new PriorityQueueEntry(1, node));
        int chosenNodes = 0;
        while (!pairPriorityQueue.isEmpty() && chosenNodes < nodes.size()-1){
            PriorityQueueEntry thisEntry = pairPriorityQueue.remove();
            int currentNode = thisEntry.getNode();
            if (thisEntry.getCost() > bestCost[currentNode]){
                continue;
            }
            ++chosenNodes;
            int adjacentNodesCount = nodes.get(currentNode).getAdjacentNodesCount();
            for (int i=0;i<adjacentNodesCount;++i){
                Node adjacentNode = nodes.get(currentNode).getKthAdjacentNode(i);
                int cost = nodes.get(currentNode).getKthAdjacentNodeCost(i);
                int nodeIndex = adjacentNode.getIndex();
                if (bestCost[nodeIndex] > bestCost[currentNode] + cost){
                    bestCost[nodeIndex] = bestCost[currentNode] + cost;
                    pairPriorityQueue.add(new PriorityQueueEntry(bestCost[nodeIndex], nodeIndex));
                }
            }
        }
    }

    /**
     * Checks whether the node with the specified index exists
     * @param node the verified index
     * @return true if the node exists, false otherwise
     */
    private boolean checkIfTheNodeExists(int node){
        if (node < 0 || node >= nodes.size()){
            return false;
        }
        return true;
    }

    private class PriorityQueueEntry implements Comparable<PriorityQueueEntry>{
        private int cost;
        private int node;

        public PriorityQueueEntry(int cost, int node){
            this.cost = cost;
            this.node = node;
        }

        public int getCost() {
            return cost;
        }

        public int getNode() {
            return node;
        }

        @Override
        public int compareTo(PriorityQueueEntry other) {
            return Integer.compare(this.getCost(), other.getCost());
        }
    }
}
