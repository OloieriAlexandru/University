package com.olo.gomoku.repositories;

import com.olo.gomoku.models.Game;
import org.springframework.data.repository.CrudRepository;

public interface GameRepository extends CrudRepository<Game, Long> {
    Game findByRandomId(String randomId);
}
