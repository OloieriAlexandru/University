package server;

import java.io.IOException;
import java.util.List;

public class GomokuRoom implements Runnable {
    private GomokuGame game;

    public GomokuRoom(GomokuGame gomokuGame) {
        game = gomokuGame;
    }

    /**
     * The starting point of a game room
     */
    @Override
    public void run() {
        try {
            game.startGame();
        } catch (IOException e) {
            System.out.println(e.getMessage());
            game.getServer().removeGameFromList(game);
        }

        notifyAllThreads();
        game.getServer().removeGameFromList(game);
    }

    /**
     * Notifies all the client threads that the game has ended and that they can
     * Continue to solve client's requests
     */
    private void notifyAllThreads() {
        List<Object> locks = game.getLocks();
        for (Object obj : locks) {
            synchronized (obj) {
                obj.notify();
            }
        }
    }
}
