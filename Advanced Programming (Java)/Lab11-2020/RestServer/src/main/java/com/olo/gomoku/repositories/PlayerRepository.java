package com.olo.gomoku.repositories;

import com.olo.gomoku.models.Player;
import org.springframework.data.repository.CrudRepository;

public interface PlayerRepository extends CrudRepository<Player, Long> {
    Player findByUsername(String username);
}
