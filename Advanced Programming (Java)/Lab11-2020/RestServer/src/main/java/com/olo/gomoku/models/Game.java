package com.olo.gomoku.models;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;
import java.util.Date;

@Getter
@Setter
@NoArgsConstructor
@Entity(name = "games")
public class Game {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(nullable = false)
    private String name;

    @Column(name = "random_id", length = 10)
    private String randomId;

    @Column(name = "player1_username")
    private String player1Username;

    @Column(name = "player2_username")
    private String player2Username;

    private String winner;

    @Column(name = "created_at", nullable = false)
    private Date createdAt;

    public void setDate() {
        createdAt = new Date();
    }
}
