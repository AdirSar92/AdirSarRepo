package com.example.demo.registration;

import lombok.AllArgsConstructor;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping( path ="api/v1/reg")
@AllArgsConstructor

public class RegistrationController {
     private RegistrationService registrationService;
     @PostMapping()
     public String register(@RequestBody RegistrationRequest request){
          System.out.println(request.getPassWord());
          return registrationService.register(request);

     }

     @GetMapping
     public String hello(){
          return "hello";
     }
}