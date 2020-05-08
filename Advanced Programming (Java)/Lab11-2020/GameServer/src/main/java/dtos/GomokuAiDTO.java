package dtos;

import org.codehaus.jackson.map.ObjectMapper;

import java.io.IOException;

public class GomokuAiDTO {
    private int aiPieceNumber;
    private int boardSize;
    private int[][] board;

    public GomokuAiDTO() {
        
    }

    public GomokuAiDTO(int aiPieceNumber, int boardSize, int[][] board) {
        this.aiPieceNumber = aiPieceNumber;
        this.boardSize = boardSize;
        this.board = board;
    }

    public String asJSON() {
        ObjectMapper objectMapper = new ObjectMapper();
        try {
            return objectMapper.writeValueAsString(this);
        } catch (IOException e) {
            return "error";
        }
    }

    public int[][] getBoard() {
        return board;
    }

    public int getAiPieceNumber() {
        return aiPieceNumber;
    }

    public int getBoardSize() {
        return boardSize;
    }

    public void setAiPieceNumber(int aiPieceNumber) {
        this.aiPieceNumber = aiPieceNumber;
    }

    public void setBoard(int[][] board) {
        this.board = board;
    }

    public void setBoardSize(int boardSize) {
        this.boardSize = boardSize;
    }
}
