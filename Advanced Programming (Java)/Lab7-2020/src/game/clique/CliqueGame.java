package game.clique;

import game.Daemon;
import game.Game;
import game.Player;
import game.Synchronizer;
import javafx.util.Pair;

import java.util.ArrayList;
import java.util.List;

/**
 * A subclass of the "Game" class
 * Implements the clique game (Bonus)
 * Javadoc can be found on the base "Game" class
 */
public class CliqueGame extends Game {
    private Graph graph;
    private Daemon daemon;
    private List<Player> players;
    private Player winner = null;

    private int k;
    private boolean running;

    public CliqueGame(int n, int duration, int k) {
        graph = new Graph(n);
        players = new ArrayList<>();
        daemon = new Daemon(this, duration);

        this.k = k;
    }

    @Override
    public void addPlayer(String type, String name) {
        if (type.equals("random")) {
            players.add(new PlayerRandom(this, name));
        } else if (type.equals("manual")) {
            players.add(new PlayerManual(this, name));
        }
    }

    @Override
    public boolean isRunning() {
        synchronized (this) {
            return running;
        }
    }

    @Override
    public void start() {
        Synchronizer synchronizer = new Synchronizer(this, players);
        running = true;

        System.out.println("Clique game started!");
        Thread daemonThread = new Thread(daemon);
        daemonThread.start();
        Thread synchronizerThread = new Thread(synchronizer);
        synchronizerThread.start();
        while (isRunning()) {
            if (graph.getEdgesCount() == 0) {
                running = false;
            }
            synchronized (this) {
                if (winner != null) {
                    running = false;
                }
            }
        }
        if (daemonThread.isAlive()) {
            daemonThread.interrupt();
        }
    }

    @Override
    public void interruptGame(String message) {
        System.out.println(message);
        synchronized (this) {
            running = false;
        }
    }

    @Override
    public void announceWinner(Player player) {
        synchronized (this) {
            winner = player;
        }
    }

    @Override
    protected Player getWinner() {
        return winner;
    }

    @Override
    protected void printWinnerDetails() {
        if (winner == null) {
            return;
        }
        System.out.println("Winner's clique: ");
        PlayerCliqueGame winnerPlayer = (PlayerCliqueGame) winner;
        winnerPlayer.printClique();
        System.out.println("Winner's edges: ");
        winnerPlayer.printEdges();
    }

    @Override
    protected List<Pair<Integer, Player>> getRanking() {
        List<Pair<Integer, Player>> ranking = new ArrayList<>();
        for (Player player : players) {
            int playerScore = player.getScore();
            ranking.add(new Pair<>(playerScore, player));
        }
        return ranking;
    }

    public Graph getGraph() {
        return graph;
    }

    public int getK() {
        return k;
    }
}
