package game.clique;

import common.Helper;

public class PlayerRandom extends PlayerCliqueGame {
    public PlayerRandom(CliqueGame game, String name) {
        super(game, name);
    }

    @Override
    protected void myTurnLogic() {
        int edgesCount = graph.getEdgesCount();
        if (edgesCount == 0) {
            return;
        }
        System.out.println(name + "'s turn. Random player.");
        Edge myEdge = graph.getKthEdge(Helper.generateRandomInt(0, edgesCount - 1));
        graph.removeEdge(myEdge);
        myEdges.add(myEdge);

        verifyScore();
    }
}
