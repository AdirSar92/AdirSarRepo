package org.example;

import java.util.UUID;

public  class Account {
    private float money;
    private final UUID accNumber = UUID.randomUUID();
    private String owner;
    private final int pinCode;

    public Account(float money, String owner, int pinCode) {
        this.money = money;
        this.owner = owner;
        this.pinCode = pinCode;
    }

    public float getMoney() {
        return money;
    }

    public void setMoney(float money) {
        this.money = money;
    }

    public UUID getAccNumber() {
        return accNumber;
    }

    public String getOwner() {
        return owner;
    }

    public void setOwner(String owner) {
        this.owner = owner;
    }

    public float checkBalance(){
        return getMoney();
    }

    public int getPinCode() {
        return pinCode;
    }
}
