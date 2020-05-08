package com.olo.gomoku.services;

import com.olo.gomoku.dtos.PlayerCredentialsDTO;
import com.olo.gomoku.exceptions.PlayerNotFoundException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.User;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;

import java.util.ArrayList;

@Service
public class JwtUserDetailsService implements UserDetailsService {
    @Autowired
    private PlayerService playerService;

    @Override
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
        PlayerCredentialsDTO playerCredentials = null;
        try {
            playerCredentials = playerService.findByUsername(username);
        } catch (PlayerNotFoundException e) {
            throw new UsernameNotFoundException("Player not found with username: " + username);
        }
        
        return new User(playerCredentials.getUsername(), playerCredentials.getPassword(), new ArrayList<>());
    }
}
