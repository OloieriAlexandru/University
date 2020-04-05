package game;

/**
 * The class sleeps from {{duration}} seconds
 * And stops the game after this
 */
public class Daemon implements Runnable {
    private Game game;
    private int duration;

    public Daemon(Game game, int duration) {
        this.game = game;
        this.duration = duration;
    }

    @Override
    public void run() {
        try {
            Thread.sleep(duration * 1000);
        } catch (InterruptedException e) {
            return;
        }
        game.interruptGame("The game ended because it exceeded the set duration!");
    }
}
