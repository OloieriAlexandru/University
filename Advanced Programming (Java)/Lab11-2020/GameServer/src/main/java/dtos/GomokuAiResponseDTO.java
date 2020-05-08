package dtos;

import org.codehaus.jackson.map.ObjectMapper;

import java.io.IOException;

public class GomokuAiResponseDTO {
    private int line;
    private int column;

    public static GomokuAiResponseDTO buildFromJson(String jsonBody) {
        try {
            return new ObjectMapper().readValue(jsonBody, GomokuAiResponseDTO.class);
        } catch (IOException e) {
            return null;
        }
    }

    public GomokuAiResponseDTO() {

    }

    public int getColumn() {
        return column;
    }

    public int getLine() {
        return line;
    }

    public void setColumn(int column) {
        this.column = column;
    }

    public void setLine(int line) {
        this.line = line;
    }
}
