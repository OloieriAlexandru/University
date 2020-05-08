package dtos;

import org.codehaus.jackson.map.ObjectMapper;

import java.io.IOException;

public class GameCreateDTO {
    private String name;
    private String randomId;

    public GameCreateDTO() {

    }

    public String asJSON() {
        ObjectMapper objectMapper = new ObjectMapper();
        try {
            return objectMapper.writeValueAsString(this);
        } catch (IOException e) {
            return "error";
        }
    }

    public GameCreateDTO(String name, String randomId) {
        this.name = name;
        this.randomId = randomId;
    }

    public String getName() {
        return name;
    }

    public String getRandomId() {
        return randomId;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setRandomId(String randomId) {
        this.randomId = randomId;
    }
}
