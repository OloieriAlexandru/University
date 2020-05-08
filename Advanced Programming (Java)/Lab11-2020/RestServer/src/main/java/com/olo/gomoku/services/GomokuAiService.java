package com.olo.gomoku.services;

import com.olo.gomoku.dtos.GomokuAiDTO;
import com.olo.gomoku.dtos.GomokuAiResponseDTO;

public interface GomokuAiService {
    GomokuAiResponseDTO getNextMove(GomokuAiDTO boardState);
}
