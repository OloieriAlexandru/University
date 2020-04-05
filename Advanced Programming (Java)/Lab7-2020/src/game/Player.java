package game;

public abstract class Player implements Runnable {
    private Object lock;
    protected String name;

    public Player(String name) {
        this.name = name;
    }

    /**
     * The player waits his turn
     * It will remain blocked in the wait() call
     * Until the synchronizer will notify that his turn has come
     */
    private void waitForMyTurn() {
        synchronized (this.lock) {
            try {
                this.lock.wait();
            } catch (InterruptedException e) {
                System.out.println(e.getMessage());
            }
        }
    }

    /**
     * Notifies the synchronizer that this player's turn has ended
     */
    private void endMyTurn() {
        synchronized (this.lock) {
            this.lock.notify();
        }
    }

    /**
     * Each subclass of this class implements its own logic to check if the game is still running
     */
    protected abstract boolean checkIfTheGameIsRunning();

    /**
     * The actual logic of the turn of a player
     */
    protected abstract void myTurnLogic();

    /**
     * The sequence of operations done by all the player classes in the application
     */
    @Override
    public void run() {
        while (true) {
            waitForMyTurn();
            if (!checkIfTheGameIsRunning()) {
                break;
            }
            myTurnLogic();
            endMyTurn();
        }
        endMyTurn();
    }

    public void setLock(Object lock) {
        this.lock = lock;
    }

    public String getName() {
        return name;
    }

    /**
     * Returns the current score of this player
     */
    public abstract int getScore();
}
