package org.example;

import java.util.Date;

public class Customer {
    private final Account account;
    private long id;
    private Date dateOfBirth;

    public Customer(Account account, long id, Date dateOfBirth) {
        this.account = account;
        this.id = id;
        this.dateOfBirth = dateOfBirth;
    }


    public Account getAccount() {
        return account;
    }

    public void transferCash(float cash){

    }
    public void depositeCheck(int number,float num){

    }
    public float withdrawlCash(float cash){
        float balance = getAccount().checkBalance();
        if(balance > cash){
            account.setMoney(balance - cash);
            return cash;
        }
        System.out.println("atm -- not enough funds");
        return 0;

    }

}
