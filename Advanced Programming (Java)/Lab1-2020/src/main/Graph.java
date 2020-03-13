package main;

import java.util.*;

public class Graph {
    private AdjacencyMatrix adjacencyMatrix;
    private AdjacencyList adjacencyList;
    private String[] words;

    public Graph(String[] words){
        this.words = words;
        adjacencyMatrix = new AdjacencyMatrix(words.length);
        adjacencyList = new AdjacencyList(words.length);

        for (int i=0;i<words.length;++i){
            for (int j=i+1;j<words.length;++j){
                if (wordsHaveACommonLetter(words[i],words[j])){
                    adjacencyMatrix.addAdjacentNodes(i, j);
                    adjacencyList.addAdjacentNodes(i, j);
                }
            }
        }
    }

    AdjacencyMatrix getAdjacencyMatrix() {
        return adjacencyMatrix;
    }

    void printNodesWithMostNeighbors() {
        int max = adjacencyList.getMaxNumberOfNeighbors();
        System.out.println("The words with the most neighbors (" + max + "):");
        for (int i=0;i<words.length;++i){
            if (max == adjacencyList.getAdjacentNodesCount(i)){
                System.out.print(words[i] + " ");
            }
        }
        System.out.println();
    }

    void printNodesWithLeastNeighbors() {
        int minn = adjacencyList.getMinNumberOfNeighbors();
        System.out.println("The words with the least neighbors (" + minn + "):");
        for (int i=0;i<words.length;++i){
            if (minn == adjacencyList.getAdjacentNodesCount(i)){
                System.out.print(words[i] + " ");
            }
        }
        System.out.println();
    }

    boolean allNodesHaveTheSameNumberOfNeighbors() {
        return adjacencyList.getMaxNumberOfNeighbors() == adjacencyList.getMinNumberOfNeighbors();
    }

    ArrayList<ArrayList<String>> getConnectedComponens(){
        ArrayList<ArrayList<String>> connectedComponents = new ArrayList<>();

        boolean[] visited = new boolean[words.length];
        for (int i=0;i<words.length;++i){
            if (visited[i]){
                continue;
            }

            ArrayList<String> connectedComponent = new ArrayList<>();
            ArrayList<Integer> connectedComponentNodes = new ArrayList<>();

            bfs(i, connectedComponentNodes, visited);

            for (Integer connectedComponentNode : connectedComponentNodes) {
                connectedComponent.add(words[connectedComponentNode]);
            }
            connectedComponents.add(connectedComponent);
        }

        return connectedComponents;
    }

    private void bfs(int startNode, ArrayList<Integer> connectedComponentNodes, boolean[] visited){
        Queue<Integer> queue = new LinkedList<>();
        visited[startNode] = true;
        queue.add(startNode);

        while (!queue.isEmpty()){
            int act = queue.remove();
            connectedComponentNodes.add(act);
            for (int i=0;i<adjacencyList.getAdjacentNodesCount(act);++i){
                int neighbor = adjacencyList.getKthAdjacentNode(act, i);
                if (!visited[neighbor]){
                    visited[neighbor] = true;
                    queue.add(neighbor);
                }
            }
        }
    }

    private boolean wordsHaveACommonLetter(String str1, String str2){
        Set<Character> str1CharsSet = new HashSet<>();
        for (int i=0;i<str1.length();++i){
            str1CharsSet.add(str1.charAt(i));
        }
        for (int i=0;i<str2.length();++i){
            if (str1CharsSet.contains(str2.charAt(i))){
                return true;
            }
        }
        return false;
    }
}
