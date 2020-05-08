package com.olo.gomoku.services;

import com.olo.gomoku.dtos.GomokuAiDTO;
import com.olo.gomoku.dtos.GomokuAiResponseDTO;
import com.olo.gomoku.utils.Helper;
import org.springframework.stereotype.Service;

@Service
public class GomokuAiServiceImpl implements GomokuAiService {
    private final static int dir = 4;
    private final static int[] moveLine = new int[]{-1, 0, 1, 0};
    private final static int[] moveColumn = new int[]{0, 1, 0, -1};

    @Override
    public GomokuAiResponseDTO getNextMove(GomokuAiDTO boardState) {
        int[][] board = boardState.getBoard();
        int n = boardState.getBoardSize();
        int player = boardState.getAiPieceNumber();
        GomokuAiResponseDTO response = Helper.getRandomNextMove(board, n);
        boolean moveWasChosen = false;
        int otherPlayerMin = 10;
        int chosenLine = 0;
        int chosenColumn = 0;

        for (int i1 = 0; i1 < n && !moveWasChosen; ++i1) {
            for (int j1 = 0; j1 < n && !moveWasChosen; ++j1) {
                if (board[i1][j1] != 2) {
                    continue;
                }
                board[i1][j1] = player;
                if (playerWins(board, n, player)) {
                    moveWasChosen = true;
                    response.setLine(i1);
                    response.setColumn(j1);
                    continue;
                }

                if (!nextToOpponent(board, n, player, i1, j1)) {
                    board[i1][j1] = 2;
                    continue;
                }

                boolean otherPlayerWins = false;
                for (int i2 = 0; i2 < n && !otherPlayerWins; ++i2) {
                    for (int j2 = 0; j2 < n && !otherPlayerWins; ++j2) {
                        if (board[i2][j2] != 2) {
                            continue;
                        }
                        board[i2][j2] = 1 - player;
                        if (playerWins(board, n, 1 - player)) {
                            otherPlayerWins = true;
                            chosenLine = i2;
                            chosenColumn = j2;
                            continue;
                        }

                        int othMin = countMaxConsecutivePieces(board, n, 1 - player);

                        if (othMin < otherPlayerMin) {
                            otherPlayerMin = othMin;
                            chosenLine = i1;
                            chosenColumn = j1;
                        }

                        board[i2][j2] = 2;
                    }
                }
                if (otherPlayerWins) {
                    moveWasChosen = true;
                    response.setLine(chosenLine);
                    response.setColumn(chosenColumn);
                    continue;
                }

                board[i1][j1] = 2;
            }
        }
        if (!moveWasChosen) {
            response.setLine(chosenLine);
            response.setColumn(chosenColumn);
        }

        return response;
    }

    private boolean nextToOpponent(int[][] board, int n, int player, int line, int column) {
        for (int k = 0; k < 4; ++k) {
            if (!inInterior(line + moveLine[k], column + moveColumn[k], n, n)) {
                continue;
            }
            if (board[line + moveLine[k]][column + moveColumn[k]] == (1 - player)) {
                return true;
            }
        }
        return false;
    }

    private boolean inInterior(int i, int j, int n, int m) {
        return i >= 0 && i < n && j >= 0 && j < m;
    }

    private boolean playerWins(int[][] board, int boardSize, int player) {
        return countMaxConsecutivePieces(board, boardSize, player) == 5;
    }

    private int countMaxConsecutivePieces(int[][] board, int boardSize, int player) {
        int max = 0;
        int last = 0;
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (board[i][j] != player) {
                    continue;
                }
                last = j;
                while (j < boardSize - 1 && board[i][j] == board[i][j + 1]) {
                    ++j;
                }
                if ((j - last + 1) > max) {
                    max = (j - last + 1);
                }
            }
        }
        for (int j = 0; j < boardSize; ++j) {
            for (int i = 0; i < boardSize; ++i) {
                if (board[i][j] != player) {
                    continue;
                }
                last = i;
                while (i < boardSize - 1 && board[i][j] == board[i + 1][j]) {
                    ++i;
                }
                if ((i - last + 1) > max) {
                    max = (i - last + 1);
                }
            }
        }
        return max;
    }
}
