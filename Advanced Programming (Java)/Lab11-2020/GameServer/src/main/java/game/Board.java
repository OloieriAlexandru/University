package game;

public class Board {
    private static final int[] moveLine = {-1, 0, 1, 0};
    private static final int[] moveColumn = {0, 1, 0, -1};
    private int[][] pieces;
    private int n;

    /**
     * Creates the board and initializes all the positions with empty cells (2)
     */
    public Board(int size) {
        n = size;
        pieces = new int[size][size];

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                pieces[i][j] = 2;
            }
        }
    }

    /**
     * Adds a piece to the board and returns true or false, depending on whether the player has won the game or not
     */
    public boolean addPiece(int player, int x, int y) {
        int horizontalCount = 1;
        int verticalCount = 1;

        pieces[x][y] = player;
        horizontalCount += countPiecesLineLeft(player, x, y) + countPiecesLineRight(player, x, y);
        verticalCount += countPiecesLineUp(player, x, y) + countPiecesLineDown(player, x, y);

        return horizontalCount >= 5 || verticalCount >= 5;
    }

    /**
     * Checks whether the position (i,j) is inside the board
     */
    private boolean inInterior(int i, int j) {
        return i >= 0 && i < n && j >= 0 && j < n;
    }

    private int countPiecesLineUp(int player, int x, int y) {
        return countPiecesMoveDirection(player, 0, x, y);
    }

    private int countPiecesLineDown(int player, int x, int y) {
        return countPiecesMoveDirection(player, 2, x, y);
    }

    private int countPiecesLineRight(int player, int x, int y) {
        return countPiecesMoveDirection(player, 1, x, y);
    }

    private int countPiecesLineLeft(int player, int x, int y) {
        return countPiecesMoveDirection(player, 3, x, y);
    }

    /**
     * Counts how many pieces are in the line with the direction "direction" starting from position (x,y)
     */
    private int countPiecesMoveDirection(int player, int direction, int x, int y) {
        int result = 0;
        x += moveLine[direction];
        y += moveColumn[direction];

        while (inInterior(x, y) && pieces[x][y] == player) {
            ++result;
            x += moveLine[direction];
            y += moveColumn[direction];
        }

        return result;
    }

    public int[][] getPieces() {
        return pieces;
    }

    /**
     * Returns a HTML representation of the board
     */
    public String getAsHTML() {
        StringBuilder htmlDocument = new StringBuilder("<html>");
        htmlDocument.append("<body>");
        htmlDocument.append("<head><style>.wrapper {height: 950px;width: 950px;display: flex;flex-direction: row;}.row {display: flex;flex-direction: column;width: calc(950px / 19);height: 100%;}.empty {width: 100%;height: 100%;border: 1px solid black;}.pieceWrapper {width: 100%;height: 100%;border: 1px solid black;display: flex;justify-content: center;align-items: center;}.playerOne {border-radius: 50%;background-color: orange;width: 80%;height: 80%;}.playerTwo {border-radius: 50%;background-color: cyan;width: 80%;height: 80%;}</style></head>\n");
        htmlDocument.append("<div class=\"wrapper\">");
        for (int[] piece : pieces) {
            htmlDocument.append("<div class=\"row\">");
            for (int j = 0; j < pieces.length; ++j) {
                if (piece[j] == 0) {
                    htmlDocument.append("<div class=\"pieceWrapper\">");
                    htmlDocument.append("<div class=\"playerOne\"></div>");
                    htmlDocument.append("</div>");
                } else if (piece[j] == 1) {
                    htmlDocument.append("<div class=\"pieceWrapper\">");
                    htmlDocument.append("<div class=\"playerTwo\"></div>");
                    htmlDocument.append("</div>");
                } else {
                    htmlDocument.append("<div class=\"empty\"></div>");
                }
            }
            htmlDocument.append("</div>");
        }
        htmlDocument.append("</div>");
        htmlDocument.append("</body>");
        htmlDocument.append("</html>\n");
        return htmlDocument.toString();
    }
}
