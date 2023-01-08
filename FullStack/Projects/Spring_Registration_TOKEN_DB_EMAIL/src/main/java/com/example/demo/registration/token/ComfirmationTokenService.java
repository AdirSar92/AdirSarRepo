package com.example.demo.registration.token;

import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.util.Optional;

@Service
@AllArgsConstructor
public class ComfirmationTokenService {
    private final ComfirmationTokenRepo comfirmationTokenRepo;
    public void saveConfirmationToken(ComfirmationToken token){
        comfirmationTokenRepo.save(token);
    }

    public Optional<ComfirmationToken> getToken(String token) {
        return comfirmationTokenRepo.findByToken(token);
    }

    public int setConfirmedAt(String token) {
        return comfirmationTokenRepo.updateConfirmedAt(
                token, LocalDateTime.now());
    }
}
