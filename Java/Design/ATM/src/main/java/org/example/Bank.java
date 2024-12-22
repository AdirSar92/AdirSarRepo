package org.example;

import java.util.*;

public class Bank {
    private final Map<UUID,ATM> atmList = new HashMap();
    private final Map<Long,Customer> customerMap = new HashMap<Long, Customer>();

    private final String nameOfBank;
    private final int bankID;


    public Bank(String nameOfBank, int bankID) {
        this.nameOfBank = nameOfBank;
        this.bankID = bankID;
    }

    public boolean isValidTransaction(Long customerID,UUID accountID, float amount,int pinNumber){
        boolean isValid = false;
        Account account = customerMap.get(customerID).getAccount();
        if(account.getAccNumber() == accountID && account.checkBalance() > amount && account.getPinCode() == pinNumber) {
            isValid = true;
        }
    }
}
