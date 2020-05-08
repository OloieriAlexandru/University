package com.olo.gomoku.services;

import com.olo.gomoku.dtos.GameCreateDTO;
import com.olo.gomoku.dtos.GameGetAllDTO;
import com.olo.gomoku.dtos.GameGetByRandomIdDTO;
import com.olo.gomoku.exceptions.GameNotFoundException;

import java.util.List;

public interface GameService {
    GameGetByRandomIdDTO getByRandomId(String randomId) throws GameNotFoundException;

    List<GameGetAllDTO> findAll();

    GameGetByRandomIdDTO create(GameCreateDTO newGame);

    GameGetByRandomIdDTO update(String randomId, GameGetByRandomIdDTO updatedGame) throws GameNotFoundException;

    void delete(Long gameId) throws GameNotFoundException;
}
