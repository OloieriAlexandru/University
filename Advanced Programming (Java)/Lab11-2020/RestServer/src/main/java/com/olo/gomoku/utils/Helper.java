package com.olo.gomoku.utils;

import com.olo.gomoku.dtos.GomokuAiResponseDTO;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Helper {
    private static List<Integer> permutation = new ArrayList<>();

    static {
        for (int i = 0; i < 81; ++i) {
            permutation.add(i);
        }
    }

    public static GomokuAiResponseDTO getRandomNextMove(int[][] board, int boardSize) {
        shufflePermutation(permutation);

        for (Integer move : permutation) {
            int line = move / boardSize;
            int column = move % boardSize;
            if (board[line][column] == 2) {
                return new GomokuAiResponseDTO(line, column);
            }
        }

        return new GomokuAiResponseDTO(boardSize / 2, boardSize / 2);
    }

    private static void shufflePermutation(List<Integer> permutation) {
        Collections.shuffle(permutation);
    }
}
