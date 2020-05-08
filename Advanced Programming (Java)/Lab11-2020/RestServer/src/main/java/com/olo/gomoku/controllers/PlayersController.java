package com.olo.gomoku.controllers;

import com.olo.gomoku.dtos.PlayerCreateDTO;
import com.olo.gomoku.dtos.PlayerGetAllDTO;
import com.olo.gomoku.dtos.PlayerUpdateDTO;
import com.olo.gomoku.exceptions.DuplicatePlayerUsernameException;
import com.olo.gomoku.exceptions.PlayerNotFoundException;
import com.olo.gomoku.services.PlayerService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/api/v1/players")
public class PlayersController {
    @Autowired
    private PlayerService playerService;

    @GetMapping
    public ResponseEntity<List<PlayerGetAllDTO>> getAll() {
        return new ResponseEntity<>(playerService.findAll(), HttpStatus.OK);
    }

    @PostMapping
    public ResponseEntity<?> create(@RequestBody PlayerCreateDTO player) throws DuplicatePlayerUsernameException {
        playerService.create(player);
        return ResponseEntity.ok().build();
    }

    @PutMapping(value = "{id}")
    public ResponseEntity<?> update(@PathVariable Long id, @RequestBody PlayerUpdateDTO player) throws PlayerNotFoundException {
        playerService.update(id, player);
        return ResponseEntity.ok().build();
    }

    @DeleteMapping(value = "{id}")
    public ResponseEntity<?> delete(@PathVariable Long id) throws PlayerNotFoundException {
        playerService.delete(id);
        return ResponseEntity.noContent().build();
    }
}
