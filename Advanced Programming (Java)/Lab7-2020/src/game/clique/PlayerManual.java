package game.clique;

import common.Helper;

import java.util.Scanner;

public class PlayerManual extends PlayerCliqueGame {
    public PlayerManual(CliqueGame game, String name) {
        super(game, name);
    }

    @Override
    protected void myTurnLogic() {
        int edgesCount = graph.getEdgesCount();
        if (edgesCount == 0) {
            return;
        }
        System.out.println(name + "'s turn. Available edges: ");
        graph.printEdges();
        System.out.println("Your edges: ");
        printMyEdges();

        Edge myEdge = graph.getKthEdge(readEdgePosition());

        myEdges.add(myEdge);
        graph.removeEdge(myEdge);

        verifyScore();
    }

    /**
     * Manual player - a user has to choose an edge
     * This function reads an edge from stdin
     */
    private int readEdgePosition() {
        Scanner scanner = new Scanner(System.in);
        int position = 0;
        while (true) {
            int firstNode = Helper.readIntOption(scanner, "Enter the first node: ");
            int secondNode = Helper.readIntOption(scanner, "Enter the second node: ");
            position = graph.getPositionOfEdge(firstNode, secondNode);
            if (position == -1) {
                System.out.println("Wrong edge!");
                continue;
            }
            break;
        }
        return position;
    }
}
