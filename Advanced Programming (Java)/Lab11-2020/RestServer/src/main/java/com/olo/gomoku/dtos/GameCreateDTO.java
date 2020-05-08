package com.olo.gomoku.dtos;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@Getter
@Setter
@NoArgsConstructor
public class GameCreateDTO {
    private String name;
    private String randomId;
}
