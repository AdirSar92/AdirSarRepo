package iotinfrastructure.datatables;

import java.sql.Date;

public class CreditCardInfo {
    private int ID;
    private String cardNumber;
    private Date expirationDate;
    private String CVV;

    public CreditCardInfo(String cardNumber, Date expirationDate, String CVV) {
        this.cardNumber = cardNumber;
        this.expirationDate = expirationDate;
        this.CVV = CVV;
    }

    @Override
    public String toString() {
        return cardNumber + "~" + expirationDate + "~" + CVV;
    }

    public void setID(int ID) {
        this.ID = ID;
    }

    public String getCardNumber() {
        return cardNumber;
    }

    public Date getExpirationDate() {
        return expirationDate;
    }

    public String getCVV() {
        return CVV;
    }

    public int getID() {
        return ID;
    }
}
