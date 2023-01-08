package com.example.security.config;

import lombok.RequiredArgsConstructor;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.AuthenticationProvider;
import org.springframework.security.authentication.dao.DaoAuthenticationProvider;
import org.springframework.security.config.annotation.authentication.configuration.AuthenticationConfiguration;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;

import org.springframework.security.config.http.SessionCreationPolicy;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.userdetails.User;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.security.crypto.password.NoOpPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.security.web.authentication.UsernamePasswordAuthenticationFilter;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

@EnableWebSecurity
@Configuration
@RequiredArgsConstructor
public class SecurityConfig  {
    private final UserDao userDao;




    private final JwaAthFilter jwaAthFilter;
    @Bean

    public SecurityFilterChain defaultSecurityFilterChain(HttpSecurity http) throws Exception {
        System.out.println("http");
        http
                .csrf().disable()
                .authorizeHttpRequests()
                .requestMatchers("/api/v1/auth/auth")
                .permitAll()
                .and().authorizeHttpRequests().requestMatchers("/api/v1/greetings/*").permitAll()
                .anyRequest()
                .authenticated()
                .and()
                .sessionManagement()
                .sessionCreationPolicy(SessionCreationPolicy.STATELESS)
                .and()
                .authenticationProvider(authProvider())
                .addFilterBefore(jwaAthFilter, UsernamePasswordAuthenticationFilter.class)
                .httpBasic();
        return http.build();
    }
@Bean
    public AuthenticationProvider authProvider() {
        final DaoAuthenticationProvider authenticationProvider = new DaoAuthenticationProvider();
        authenticationProvider.setUserDetailsService(userDetailsService());
        authenticationProvider.setPasswordEncoder(passwordEncoder());
        return authenticationProvider;
    }
@Bean
    public PasswordEncoder passwordEncoder() {
        return NoOpPasswordEncoder.getInstance();
    }

    @Bean
    public UserDetailsService userDetailsService() {
        return  new UserDetailsService() {
            @Override
            public UserDetails loadUserByUsername(String email) throws UsernameNotFoundException {
              return userDao.findUserByEmail(email);
            }
        };
    }

    @Bean
    public AuthenticationManager authenticationManager(AuthenticationConfiguration config) throws Exception {
        return config.getAuthenticationManager();
    }

}
