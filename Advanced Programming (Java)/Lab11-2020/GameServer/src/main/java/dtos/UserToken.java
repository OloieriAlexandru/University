package dtos;

import org.codehaus.jackson.map.ObjectMapper;

import java.io.IOException;

public class UserToken {
    private String token;

    public UserToken() {

    }

    public UserToken(String token) {
        this.token = token;
    }

    public String getToken() {
        return token;
    }

    public void setToken(String token) {
        this.token = token;
    }

    public static UserToken buildFromJson(String jsonBody) {
        try {
            return new ObjectMapper().readValue(jsonBody, UserToken.class);
        } catch (IOException e) {
            return null;
        }
    }
}
