package dtos;

import org.codehaus.jackson.map.ObjectMapper;

import java.io.IOException;
import java.util.Date;

public class GameUpdateDTO {
    private Long id;
    private String name;
    private String randomId;
    private String player1Username;
    private String player2Username;
    private String winner;
    private Date date;

    public GameUpdateDTO() {

    }

    public static GameUpdateDTO buildFromJson(String jsonBody) {
        try {
            return new ObjectMapper().readValue(jsonBody, GameUpdateDTO.class);
        } catch (IOException e) {
            return null;
        }
    }

    public String asJSON() {
        ObjectMapper objectMapper = new ObjectMapper();
        try {
            return objectMapper.writeValueAsString(this);
        } catch (IOException e) {
            return "error";
        }
    }

    public String getPlayer1Username() {
        return player1Username;
    }

    public String getPlayer2Username() {
        return player2Username;
    }

    public String getRandomId() {
        return randomId;
    }

    public String getName() {
        return name;
    }

    public Long getId() {
        return id;
    }

    public Date getDate() {
        return date;
    }

    public String getWinner() {
        return winner;
    }

    public void setRandomId(String randomId) {
        this.randomId = randomId;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public void setWinner(String winner) {
        this.winner = winner;
    }

    public void setPlayer1Username(String player1Username) {
        this.player1Username = player1Username;
    }

    public void setPlayer2Username(String player2Username) {
        this.player2Username = player2Username;
    }
}
