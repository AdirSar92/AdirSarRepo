package com.example.security.controllers;

import com.example.security.config.JwtUtil;
import com.example.security.config.UserDao;
import lombok.RequiredArgsConstructor;
import org.springframework.http.ResponseEntity;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/api/v1/auth")
@RequiredArgsConstructor
public class AuthController {
    private final AuthenticationManager authenticationManager;
    private final JwtUtil jwtUtil;
    private final UserDao userDao;

    @PostMapping("/auth")
    public ResponseEntity<String> auth(@RequestBody AuthRequest request){
        System.out.println("in");
        authenticationManager.authenticate(new UsernamePasswordAuthenticationToken(request.getEmail(),request.getPassword()));
        final UserDetails user = userDao.findUserByEmail((request.getEmail()));
        if(user != null){
            return ResponseEntity.ok(jwtUtil.generateToken(user));
        }
        return   ResponseEntity.status(400).body(" Some error");
    }
}
