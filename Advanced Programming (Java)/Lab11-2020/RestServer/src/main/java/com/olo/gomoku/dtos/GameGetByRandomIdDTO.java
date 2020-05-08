package com.olo.gomoku.dtos;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.Date;

@Getter
@Setter
@NoArgsConstructor
public class GameGetByRandomIdDTO {
    private Long id;
    private String name;
    private String randomId;
    private String player1Username;
    private String player2Username;
    private String winner;
    private Date date;
}
