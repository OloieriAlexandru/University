package com.olo.gomoku.exceptions;

import com.olo.gomoku.dtos.GenericErrorDTO;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.RestControllerAdvice;

@RestControllerAdvice
public class OloExceptionAdvice {
    @ExceptionHandler(value = GenericNotFoundException.class)
    public ResponseEntity<?> handleGenericNotFoundException(GenericNotFoundException e) {
        GenericErrorDTO error = new GenericErrorDTO(e.getMessage());
        return new ResponseEntity<>(error, HttpStatus.NOT_FOUND);
    }

    @ExceptionHandler(value = DuplicatePlayerUsernameException.class)
    public ResponseEntity<?> handleDuplicateUsernameException(DuplicatePlayerUsernameException e) {
        GenericErrorDTO error = new GenericErrorDTO(e.getMessage());
        return new ResponseEntity<>(error, HttpStatus.BAD_REQUEST);
    }
}
