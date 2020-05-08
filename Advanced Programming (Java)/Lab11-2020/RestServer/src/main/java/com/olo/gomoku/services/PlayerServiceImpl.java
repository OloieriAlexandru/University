package com.olo.gomoku.services;

import com.olo.gomoku.dtos.PlayerCreateDTO;
import com.olo.gomoku.dtos.PlayerCredentialsDTO;
import com.olo.gomoku.dtos.PlayerGetAllDTO;
import com.olo.gomoku.dtos.PlayerUpdateDTO;
import com.olo.gomoku.exceptions.DuplicatePlayerUsernameException;
import com.olo.gomoku.exceptions.PlayerNotFoundException;
import com.olo.gomoku.models.Player;
import com.olo.gomoku.repositories.PlayerRepository;
import org.modelmapper.ModelMapper;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

@Service
public class PlayerServiceImpl implements PlayerService {
    @Autowired
    private PlayerRepository playerRepository;

    @Autowired
    private ModelMapper modelMapper;

    @Autowired
    private PasswordEncoder bCryptEncoder;

    @Override
    public PlayerCredentialsDTO findByUsername(String username) throws PlayerNotFoundException {
        Player player = playerRepository.findByUsername(username);
        if (player == null) {
            throw new PlayerNotFoundException("No player with username '" + username + "' exists");
        }
        return modelMapper.map(player, PlayerCredentialsDTO.class);
    }

    @Override
    public List<PlayerGetAllDTO> findAll() {
        List<Player> players = (List<Player>) playerRepository.findAll();
        List<PlayerGetAllDTO> resultPlayers = new ArrayList<>();

        for (Player player : players) {
            resultPlayers.add(modelMapper.map(player, PlayerGetAllDTO.class));
        }

        return resultPlayers;
    }

    @Override
    public void create(PlayerCreateDTO newPlayer) throws DuplicatePlayerUsernameException {
        Player player = playerRepository.findByUsername(newPlayer.getUsername());
        if (player != null) {
            throw new DuplicatePlayerUsernameException("A player with the username \"" + newPlayer.getUsername() + "\" already exists!");
        }

        player = modelMapper.map(newPlayer, Player.class);
        player.setPassword(bCryptEncoder.encode(newPlayer.getPassword()));

        playerRepository.save(player);
    }

    @Override
    public void update(Long id, PlayerUpdateDTO updatedPlayer) throws PlayerNotFoundException {
        Optional<Player> playerOpt = playerRepository.findById(id);
        if (!playerOpt.isPresent()) {
            throw new PlayerNotFoundException("No player with id '" + id + "' exists");
        }

        Player player = playerOpt.get();
        BeanUtils.copyProperties(updatedPlayer, player);
        playerRepository.save(player);
    }

    @Override
    public void delete(Long playerId) throws PlayerNotFoundException {
        if (!playerRepository.existsById(playerId)) {
            throw new PlayerNotFoundException("No player with id '" + playerId + "' exists");
        }

        playerRepository.deleteById(playerId);
    }
}
