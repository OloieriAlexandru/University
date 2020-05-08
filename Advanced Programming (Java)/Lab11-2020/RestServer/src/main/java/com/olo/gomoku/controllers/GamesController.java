package com.olo.gomoku.controllers;

import com.olo.gomoku.dtos.*;
import com.olo.gomoku.exceptions.GameNotFoundException;
import com.olo.gomoku.services.GameService;
import com.olo.gomoku.services.GomokuAiService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/api/v1/games")
public class GamesController {
    @Autowired
    private GameService gameService;

    @Autowired
    private GomokuAiService gomokuAiService;

    @GetMapping
    public ResponseEntity<List<GameGetAllDTO>> getAll() {
        return new ResponseEntity<>(gameService.findAll(), HttpStatus.OK);
    }

    @GetMapping(value = "{randomId}")
    public ResponseEntity<?> getByRandomId(@PathVariable String randomId) throws GameNotFoundException {
        GameGetByRandomIdDTO game = gameService.getByRandomId(randomId);
        return ResponseEntity.ok(game);
    }

    @PostMapping
    public ResponseEntity<?> create(@RequestBody GameCreateDTO game) {
        GameGetByRandomIdDTO createdGame = gameService.create(game);
        return ResponseEntity.ok(createdGame);
    }

    @PutMapping(value = "{randomId}")
    public ResponseEntity<?> update(@PathVariable String randomId, @RequestBody GameGetByRandomIdDTO updatedGame) throws GameNotFoundException {
        gameService.update(randomId, updatedGame);
        return ResponseEntity.ok().build();
    }

    @DeleteMapping(value = "{id}")
    public ResponseEntity<?> delete(@PathVariable Long id) throws GameNotFoundException {
        gameService.delete(id);
        return ResponseEntity.noContent().build();
    }

    @PostMapping(value = "/ai")
    public ResponseEntity<GomokuAiResponseDTO> getNextMove(@RequestBody GomokuAiDTO boardState) {
        return ResponseEntity.ok(gomokuAiService.getNextMove(boardState));
    }
}
