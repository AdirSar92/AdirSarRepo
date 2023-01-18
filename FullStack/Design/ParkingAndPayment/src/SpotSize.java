public enum SpotSize {
    BIG(10),
    MED(7),

    SMALL(5);
    SpotSize(int costPerHour) {
        this.costPerHour = costPerHour;
    }

    final int costPerHour;

    public SpotSize increaseSize(SpotSize spotSize){
        if(SpotSize.MED == spotSize) return SpotSize.BIG;
        if(SpotSize.SMALL == spotSize) return SpotSize.MED;
        return  null;
    }
}
