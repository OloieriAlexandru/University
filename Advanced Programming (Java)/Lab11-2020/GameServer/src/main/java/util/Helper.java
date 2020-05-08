package util;

import dtos.UserCredentialsDTO;
import dtos.UserToken;
import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.node.ObjectNode;
import org.springframework.http.*;
import org.springframework.web.client.HttpClientErrorException;
import org.springframework.web.client.RestTemplate;

import org.apache.commons.codec.binary.Base64;

import java.io.IOException;
import java.util.Random;

public class Helper {
    public final static String apiURL = "https://127.0.0.1:8443/api/v1/";
    public static String adminJwt;

    private static Random random = new Random();

    public static ResponseEntity<String> post(String url, String body, String jwt) {
        return genericWithBodyRequest(url, body, HttpMethod.POST, jwt);
    }

    public static ResponseEntity<String> put(String url, String body, String jwt) {
        return genericWithBodyRequest(url, body, HttpMethod.PUT, jwt);
    }

    public static ResponseEntity<String> genericWithBodyRequest(String url, String body, HttpMethod method, String jwt) {
        RestTemplate restTemplate = new RestTemplate();

        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);
        if (jwt != null) {
            headers.set("Authorization", "Bearer " + jwt);
        }
        HttpEntity<String> entity = new HttpEntity<>(body, headers);

        return restTemplate.exchange(apiURL + url, method, entity, String.class);
    }

    public static boolean initAdminJwt() {
        try {
            Helper.post("players", new UserCredentialsDTO("admin", "admin").asJSON(), null);
        } catch (HttpClientErrorException ignored) {
        }

        try {
            ResponseEntity<String> resp = Helper.post("auth", new UserCredentialsDTO("admin", "admin").asJSON(), null);
            adminJwt = UserToken.buildFromJson(resp.getBody()).getToken();
        } catch (HttpClientErrorException e) {
            return false;
        }
        return true;
    }

    public static String getUsernameFromJwt(String jwt) {
        String[] split_string = jwt.split("\\.");
        String base64EncodedBody = split_string[1];
        Base64 base64Url = new Base64(true);
        String body = new String(base64Url.decode(base64EncodedBody));

        System.out.println(body);
        ObjectNode node = null;
        try {
            node = new ObjectMapper().readValue(body, ObjectNode.class);
        } catch (IOException e) {
            return "error";
        }
        if (node == null || !node.has("sub")) {
            return "error";
        }
        return node.get("sub").asText();
    }

    public static String getRandomHexString(int length) {
        StringBuilder stringBuilder = new StringBuilder();

        while (stringBuilder.length() < length) {
            stringBuilder.append(Integer.toHexString(random.nextInt()));
        }

        return stringBuilder.toString().substring(0, length);
    }

    public static String[] splitString(String str) {
        return str.trim().split("\\s+", 1);
    }

    public static Pair<Integer, Integer> splitMoveString(String move) {
        String[] aux = move.split(" ");
        return new Pair<>(Integer.parseInt(aux[0]), Integer.parseInt(aux[1]));
    }
}
