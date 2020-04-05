package game;

import javafx.util.Pair;

import java.util.List;

public abstract class Game {
    /**
     * Adds a player to the game
     * Acts as a factory method
     */
    public abstract void addPlayer(String type, String name);

    /**
     * Returns whether the game is still running
     */
    public abstract boolean isRunning();

    /**
     * Starts the game
     */
    public abstract void start();

    /**
     * A method called by the timekeeper daemon if the duration of the game exceeded the specified duration
     */
    public abstract void interruptGame(String message);

    /**
     * A method which sets the winner
     */
    public abstract void announceWinner(Player player);

    /**
     * Returns the winner of the game
     * Each subclass of the "Game" class implements its own logic to return the winner
     */
    protected abstract Player getWinner();

    /**
     * In case it's necessary, this method prints additional information about the winner
     */
    protected abstract void printWinnerDetails();

    /**
     * Returns the ranking of the game
     * Each subclass of the "Game" class implements its own logic to return the ranking
     */
    protected abstract List<Pair<Integer, Player>> getRanking();

    /**
     * Prints the winner of the game
     * This method is called only after the "start" method was called
     */
    public void printWinnerOrRanking() {
        Player winner = getWinner();
        if (winner == null) {
            List<Pair<Integer, Player>> ranking = getRanking();
            ranking.sort((o1, o2) -> -1 * Integer.compare(o1.getKey(), o2.getKey()));
            System.out.println("Ranking: ");
            for (int i = 0; i < ranking.size(); ++i) {
                System.out.println((i + 1) + ". " + ranking.get(i).getValue().getName() + " - " + ranking.get(i).getKey());
            }
            announceWinner(ranking.get(0).getValue());
            System.out.println("Winner: " + ranking.get(0).getValue().getName());
        } else {
            System.out.println("Winner: " + winner.getName());
        }
        printWinnerDetails();
        System.out.println();
    }
}
