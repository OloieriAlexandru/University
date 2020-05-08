package com.olo.gomoku.dtos;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@Getter
@Setter
@NoArgsConstructor
public class PlayerUpdateDTO {
    private Long id;
    private String username;
}
