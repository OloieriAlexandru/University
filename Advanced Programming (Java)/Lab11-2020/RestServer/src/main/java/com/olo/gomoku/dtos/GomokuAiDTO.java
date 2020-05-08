package com.olo.gomoku.dtos;

import lombok.NoArgsConstructor;
import lombok.Setter;

@Setter
@NoArgsConstructor
public class GomokuAiDTO {
    private int aiPieceNumber;
    private int boardSize;
    private int[][] board;

    public int[][] getBoard() {
        return board;
    }

    public int getAiPieceNumber() {
        return aiPieceNumber;
    }

    public int getBoardSize() {
        return boardSize;
    }
}
