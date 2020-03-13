package main;

public class AdjacencyMatrix {
    private boolean[][] matrix;

    AdjacencyMatrix(int nodesNumber){
        matrix = new boolean[nodesNumber][nodesNumber];
    }

    void addAdjacentNodes(int firstNode, int secondNode){
        matrix[firstNode][secondNode] = matrix[secondNode][firstNode] = true;
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();

        if (matrix.length > 100){
            stringBuilder.append("The graph has more than 100 nodes! The adjacency matrix won't be displayed!");
        } else {
            for (int i=0;i<matrix.length;++i){
                for (int j=0;j<matrix[i].length;++j){
                    stringBuilder.append((matrix[i][j] ? 1 : 0)).append(" ");
                }
                stringBuilder.append('\n');
            }
        }

        return stringBuilder.toString();
    }
}
