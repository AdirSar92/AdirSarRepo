package iotinfrastructure.datatables;

public class BankInfo {
    private int ID;
    private int bankNumber;
    private int bankBranch;
    private int accountNumber;

    public BankInfo(int bankNumber, int bankBranch, int accountNumber) {
        this.bankNumber = bankNumber;
        this.bankBranch = bankBranch;
        this.accountNumber = accountNumber;
    }

    @Override
    public String toString() {
        return bankNumber + "~" + bankBranch + "~" + accountNumber;
    }

    public int getID() {
        return ID;
    }

    public int getBankNumber() {
        return bankNumber;
    }

    public int getBankBranch() {
        return bankBranch;
    }

    public int getAccountNumber() {
        return accountNumber;
    }

    public void setID(int ID) {
        this.ID = ID;
    }

}
