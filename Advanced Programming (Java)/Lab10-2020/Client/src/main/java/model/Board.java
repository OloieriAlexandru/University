package model;

public class Board {
    private int[][] pieces = null;
    private int n;

    public Board(int size) {
        n = size;
        pieces = new int[size][size];

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                pieces[i][j] = 2;
            }
        }
    }

    public void addPiece(int player, int x, int y) {
        if (!inInterior(x, y)) {
            return;
        }
        pieces[x][y] = player;
    }

    public boolean checkIfSpotIsFree(int x, int y) {
        if (!inInterior(x, y)) {
            return false;
        }
        return pieces[x][y] == 2;
    }

    private boolean inInterior(int i, int j) {
        return i >= 0 && i < n && j >= 0 && j < n;
    }
}
