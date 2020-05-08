package com.olo.gomoku.services;

import com.olo.gomoku.dtos.GameCreateDTO;
import com.olo.gomoku.dtos.GameGetAllDTO;
import com.olo.gomoku.dtos.GameGetByRandomIdDTO;
import com.olo.gomoku.exceptions.GameNotFoundException;
import com.olo.gomoku.models.Game;
import com.olo.gomoku.repositories.GameRepository;
import org.modelmapper.ModelMapper;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

@Service
public class GameServiceImpl implements GameService {
    @Autowired
    private GameRepository gameRepository;

    @Autowired
    private ModelMapper modelMapper;

    @Override
    public GameGetByRandomIdDTO getByRandomId(String randomId) throws GameNotFoundException {
        Game game = gameRepository.findByRandomId(randomId);
        if (game == null) {
            throw new GameNotFoundException("No game with randomId '" + randomId + "' exists");
        }
        return modelMapper.map(game, GameGetByRandomIdDTO.class);
    }

    @Override
    public List<GameGetAllDTO> findAll() {
        List<Game> games = (List<Game>) gameRepository.findAll();
        List<GameGetAllDTO> resultGames = new ArrayList<>();

        for (Game game : games) {
            resultGames.add(modelMapper.map(game, GameGetAllDTO.class));
        }

        return resultGames;
    }

    @Override
    public GameGetByRandomIdDTO create(GameCreateDTO newGame) {
        Game game = new Game();
        BeanUtils.copyProperties(newGame, game);
        game.setDate();

        gameRepository.save(game);
        return modelMapper.map(game, GameGetByRandomIdDTO.class);
    }

    @Override
    public GameGetByRandomIdDTO update(String randomId, GameGetByRandomIdDTO updatedGame) throws GameNotFoundException {
        Game game = gameRepository.findByRandomId(randomId);
        if (game == null) {
            throw new GameNotFoundException("No game with randomId '" + randomId + "' exists");
        }
        BeanUtils.copyProperties(updatedGame, game);
        gameRepository.save(game);
        return updatedGame;
    }

    @Override
    public void delete(Long gameId) throws GameNotFoundException {
        if (!gameRepository.existsById(gameId)) {
            throw new GameNotFoundException("No game with id '" + gameId + "' exists");
        }

        gameRepository.deleteById(gameId);
    }
}
