package com.olo.gomoku.services;

import com.olo.gomoku.dtos.PlayerCreateDTO;
import com.olo.gomoku.dtos.PlayerCredentialsDTO;
import com.olo.gomoku.dtos.PlayerGetAllDTO;
import com.olo.gomoku.dtos.PlayerUpdateDTO;
import com.olo.gomoku.exceptions.DuplicatePlayerUsernameException;
import com.olo.gomoku.exceptions.PlayerNotFoundException;

import java.util.List;

public interface PlayerService {
    PlayerCredentialsDTO findByUsername(String username) throws PlayerNotFoundException;

    List<PlayerGetAllDTO> findAll();

    void create(PlayerCreateDTO newPlayer) throws DuplicatePlayerUsernameException;

    void update(Long id, PlayerUpdateDTO updatedPlayer) throws PlayerNotFoundException;

    void delete(Long playerId) throws PlayerNotFoundException;
}
