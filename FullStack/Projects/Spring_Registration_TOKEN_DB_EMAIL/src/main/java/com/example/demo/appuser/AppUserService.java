package com.example.demo.appuser;

import com.example.demo.registration.token.ComfirmationToken;
import com.example.demo.registration.token.ComfirmationTokenService;
import lombok.AllArgsConstructor;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.util.UUID;

@Service
@AllArgsConstructor
public class AppUserService implements UserDetailsService {
    private final static String USER_NOT_FOUND_MSG = "user with email %s not found";
    private final BCryptPasswordEncoder bCryptPasswordEncoder;
    private final AppUserRepo appUserRepo;

    private final ComfirmationTokenService comfirmationTokenService;
    @Override
    public UserDetails loadUserByUsername(String email) throws UsernameNotFoundException {
        return appUserRepo.findByEmail(email).orElseThrow(()->new UsernameNotFoundException(String.format(USER_NOT_FOUND_MSG,email)));
    }

    public  String signUpUser(AppUser appUser){
        boolean userExist = appUserRepo.findByEmail(appUser.getEmail()).isPresent();
        if(userExist){
            // TODO check of attributes are the same and
            // TODO if email not confirmed send confirmation email.
            throw new IllegalStateException("Email alredy taken");
        }
        String encodedPassword = bCryptPasswordEncoder.encode(appUser.getPassword());
        appUser.setPassWord(encodedPassword);
        appUserRepo.save(appUser);
        //token
        String token = UUID.randomUUID().toString();
        ComfirmationToken comfirmationToken = new ComfirmationToken(token, LocalDateTime.now(),LocalDateTime.now().plusMinutes(15),appUser);
        comfirmationTokenService.saveConfirmationToken(comfirmationToken);
        //send email
        return token;
    }

    public int enableAppUser(String email) {
        return appUserRepo.enableAppUser(email);
    }
}
