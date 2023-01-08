package com.example.security.config;

import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.userdetails.User;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Repository;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

@Repository
public class UserDao {
    private final static List<UserDetails> APP_USERS = Arrays.asList
            (new User("adir@gmail.com","password", Collections.singleton(new SimpleGrantedAuthority("ROLE_ADMIN"))),
                    new User("YOSI@gmail.com","password", Collections.singleton(new SimpleGrantedAuthority("ROLE_USER"))));


    public UserDetails findUserByEmail(String email){
        System.out.println(email);
        return APP_USERS.stream().filter(userDetails -> userDetails.getUsername().equals(email)).findFirst()
                .orElseThrow(()-> new UsernameNotFoundException("No user was found"));
    }
}
