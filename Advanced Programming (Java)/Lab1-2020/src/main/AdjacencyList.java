package main;

import java.util.ArrayList;

public class AdjacencyList {
    private ArrayList<ArrayList<Integer>> adjacencyLists;

    AdjacencyList(int nodesNumber){
        adjacencyLists = new ArrayList<>();
        for (int i=0;i<nodesNumber;++i){
            adjacencyLists.add(new ArrayList<>());
        }
    }

    void addAdjacentNodes(int firstNode, int secondNode){
        adjacencyLists.get(firstNode).add(secondNode);
        adjacencyLists.get(secondNode).add(firstNode);
    }

    int getMaxNumberOfNeighbors(){
        if (adjacencyLists.size() == 0){
            return 0;
        }
        int ans = adjacencyLists.get(0).size();
        for (int i=1;i<adjacencyLists.size();++i){
            int current = adjacencyLists.get(i).size();
            if (current > ans){
                ans = current;
            }
        }
        return ans;
    }

    int getMinNumberOfNeighbors(){
        if (adjacencyLists.size() == 0){
            return 0;
        }
        int ans = adjacencyLists.get(0).size();
        for (int i=1;i<adjacencyLists.size();++i){
            int current = adjacencyLists.get(i).size();
            if (current < ans){
                ans = current;
            }
        }
        return ans;
    }

    int getAdjacentNodesCount(int node){
        return adjacencyLists.get(node).size();
    }

    int getKthAdjacentNode(int node, int k){
        return adjacencyLists.get(node).get(k);
    }
}
