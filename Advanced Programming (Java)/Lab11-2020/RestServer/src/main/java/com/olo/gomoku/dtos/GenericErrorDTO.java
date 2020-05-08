package com.olo.gomoku.dtos;

import lombok.Getter;
import lombok.Setter;

import java.time.LocalDateTime;

@Getter
@Setter
public class GenericErrorDTO {
    private String message;
    private LocalDateTime timestamp;

    public GenericErrorDTO(String message) {
        this.message = message;
        this.timestamp = LocalDateTime.now();
    }
}
