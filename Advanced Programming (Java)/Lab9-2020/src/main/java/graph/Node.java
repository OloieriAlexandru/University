package graph;

import java.util.ArrayList;
import java.util.List;

public class Node {
    private List<Integer> adjacentNodes = new ArrayList<>();

    public List<Integer> getAdjacentNodes() {
        return adjacentNodes;
    }

    public void addNodes(int... nodes) {
        for (int i : nodes) {
            adjacentNodes.add(i);
        }
    }
}
