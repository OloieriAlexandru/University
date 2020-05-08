package game;

public class Game {
    private Board board;

    public Game(int size) {
        board = new Board(size);
    }

    public boolean addPiece(int player, int x, int y) {
        return board.addPiece(player, x, y);
    }

    public String getAsHTML() {
        return board.getAsHTML();
    }

    public int[][] getPieces() {
        return board.getPieces();
    }
}
