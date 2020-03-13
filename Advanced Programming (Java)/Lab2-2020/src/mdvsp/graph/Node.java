package mdvsp.graph;

import javafx.util.Pair;

import java.util.ArrayList;

public class Node {
    private ArrayList<Pair<Node,Integer>> adjacentNodes = new ArrayList<>();
    private int index;

    /**
     * Creates a new node, assigning to it an index
     * @param index the associated index
     */
    public Node(int index){
        this.index = index;
    }

    /**
     * Adds a node (and the cost of the edge) to the adjacency list of this node
     * @param node the adjacent node
     * @param cost the cost of the edge
     */
    public void addAdjacentNode(Node node, int cost){
        adjacentNodes.add(new Pair<>(node, cost));
    }

    /**
     * Returns the number of the nodes this node is adjacent with
     * @return the number of nodes in the adjacency list of this node
     */
    public int getAdjacentNodesCount(){
        return adjacentNodes.size();
    }

    /**
     * Returns the k-th node in the adjacency list
     * @param k the index of the requested node
     * @return the requested node
     */
    public Node getKthAdjacentNode(int k){
        if (k < 0 || k >= adjacentNodes.size()){
            return null;
        }
        return adjacentNodes.get(k).getKey();
    }

    /**
     * Returns the cost of the k-th edge
     * @param k the index of the edge the cost is requested
     * @return the cost of the edge
     */
    public int getKthAdjacentNodeCost(int k){
        if (k < 0 || k >= adjacentNodes.size()){
            return 0;
        }
        return adjacentNodes.get(k).getValue();
    }

    /**
     * Returns the index of the node in the graph that contains it
     * @return the index of the node
     */
    public int getIndex() {
        return index;
    }
}
