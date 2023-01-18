public enum TypeOfVehicle {
    MOTORCYCLE(1000),
    CAR(2000),
    TRUCK(5000);
    final int costToRepair;
    TypeOfVehicle(int cost) {
        this.costToRepair = cost;
    }

    public int getCostToRepair() {
        return costToRepair;
    }
}
