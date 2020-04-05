package game.clique;

import game.Player;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public abstract class PlayerCliqueGame extends Player {
    protected CliqueGame game;
    protected Graph graph;
    protected List<Edge> myEdges;

    public PlayerCliqueGame(CliqueGame game, String name) {
        super(name);
        this.game = game;
        this.graph = game.getGraph();
        this.myEdges = new ArrayList<>();
    }

    /**
     * Prints to stdout the best clique of this player
     */
    public void printClique() {
        Clique clique = getMaximumClique();
        List<Integer> nodes = clique.getNodes();
        for (int i = 0; i < nodes.size(); ++i) {
            System.out.print(nodes.get(i));
            if (i + 1 < nodes.size()) {
                System.out.print(", ");
            }
        }
        System.out.println();
    }

    /**
     * Prints all the edges of the current player
     */
    public void printEdges() {
        for (Edge edge : myEdges) {
            System.out.println(edge);
        }
    }

    protected void printMyEdges() {
        for (int i = 0; i < myEdges.size(); ++i) {
            System.out.println(myEdges.get(i));
            if (i + 1 < myEdges.size()) {
                System.out.print(", ");
            }
        }
        System.out.println();
    }

    /**
     * Checks if this player has won the game
     */
    protected void verifyScore() {
        int myScore = getScore();
        if (myScore == game.getK()) {
            game.announceWinner(this);
        }
    }

    @Override
    protected boolean checkIfTheGameIsRunning() {
        return game.isRunning();
    }

    /**
     * Returns the score of this player
     */
    @Override
    public int getScore() {
        return getMaximumClique().getNodesCount();
    }

    /**
     * Returns the clique with the maximum number of nodes of this player
     */
    private Clique getMaximumClique() {
        Clique clique = new Clique();
        Collections.sort(myEdges);
        int n = graph.getNumberOfNodes();
        int lastConfiguration = (1 << n);
        for (int i = 0; i < lastConfiguration; ++i) {
            List<Integer> nodes = new ArrayList<>();
            for (int j = 0; j < n; ++j) {
                if ((i & (1 << j)) != 0) {
                    nodes.add(j + 1);
                }
            }
            if (goodSubsetOfNodes(nodes)) {
                if (nodes.size() > clique.getNodesCount()) {
                    clique.setNodes(nodes);
                }
            }
        }
        return clique;
    }

    /**
     * Checks whether this player has all the edges of the clique formed by the nodes from "nodes" list
     */
    private boolean goodSubsetOfNodes(List<Integer> nodes) {
        for (int i = 0; i < nodes.size() - 1; ++i) {
            for (int j = i + 1; j < nodes.size(); ++j) {
                int positionInArray = binarySearchEdgePosition(nodes.get(i), nodes.get(j));
                if (positionInArray == -1) {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Binary searches the position of the edge {firstNode, secondNodes} in myEdges array of edges
     */
    private int binarySearchEdgePosition(int firstNode, int secondNode) {
        int left = 0;
        int right = myEdges.size() - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            Edge edge = myEdges.get(mid);
            if (edge.getFirstNode() == firstNode) {
                if (edge.getSecondNode() == secondNode) {
                    return mid;
                } else if (edge.getSecondNode() > secondNode) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            } else {
                if (edge.getSecondNode() > secondNode) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }
        }
        return -1;
    }
}
