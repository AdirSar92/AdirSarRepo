import java.util.*;


public class ParkingLotManager {
    private final List<Floors> floorList ;
    private final Map<Reservetion,Spot> spotMap = new HashMap<>();
    private final Map<Integer,Reservetion> reservationMap = new HashMap<>();
    private final Scanner input = new Scanner(System.in);
    private int totalIncome = 0;

    public ParkingLotManager(int floors, int spots) {
        this.floorList = new ArrayList<>(floors);
        while (floors > 0){
            floorList.add(new Floors(spots));
            --floors;
        }

    }

    public void  showMenue() {
        System.out.println("Welcome");
        System.out.println("for park press 1, un park press 2 ,total income press 3");
        switch (input.nextInt()) {
            case 1:
                Reservetion res = parkVehicleUI();
                if (null == res){
                    System.out.println("the Lot is full");
                }else {
                    showRecite(res);
                }
                break;
            case 2:
               unParkVehicleUI();
              break;
            case 3:
                System.out.println("total income is " +getTotalIncome());

            default:
                System.out.println("Wrong Input");

        }
    }

    public Reservetion parkVehicleUI() {
        System.out.println("enter car ID and size[BIG,MED,SMALL] by comma");
        String str = input.next();
        String[] arr = str.split(",");
        Map<String,Vehicle> map = VehicleFactory.getVehicleMap();
        Vehicle vehicle = map.get()
        Reservetion res = parkVehicle(new Vehicle(Integer.parseInt(arr[0]), arr[1]));
        return res;
    }

    private void unParkVehicleUI() {
        System.out.println("enter car ID  to unpark");

        int id = input.nextInt();
        unPark(id);


    }

    public Reservetion parkVehicle(Vehicle vehicle) {
        Reservetion reservetion = null;
        Spot spotFilled = null;
        for (Floors currFloor : floorList) {
            Spot spot = currFloor.park(vehicle.getSizeOfVehicle());
            if(null != spot){
                spotFilled = spot;
                break;
            }
        }
        if (spotFilled != null ){
            reservetion = new Reservetion(vehicle, spotFilled.getSpotID(), vehicle.getSizeOfVehicle());
            spotMap.put(reservetion,spotFilled);
            reservationMap.put(vehicle.getVehicleID(),reservetion);
        }
        return reservetion;

    }

    private int getTotalIncome() {
        return totalIncome;
    }

    //could ask for reservation and subtract the reservation  MAp
    private void unPark(int carID) {
        Reservetion reservetion  = reservationMap.get(carID);
        System.out.println(reservetion.getVehicleID());
        Spot spot = spotMap.get(reservetion);
        float cost = endPark(reservetion);

        System.out.println("the cost of parking time :"+ reservetion.getOverallTime() + " is " + cost + " NIS");
        System.out.println("enter the amount ...");
        int pay = input.nextInt();
        if(cost < pay){
            freeParking(spot,reservetion,cost);
            System.out.println("His your change " + (pay - cost));
        }
    }

    private float endPark(Reservetion reservation){
        reservation.endParking();
        return calcPayment(reservation);
    }

    private void freeParking(Spot spot,Reservetion reservetion,float cost){
        System.out.println("Thanx Coma again");
        showRecite(reservationMap.get(reservetion.getVehicleID()));
        totalIncome += cost;
        spot.setFree(true);
        reservationMap.remove(reservetion.getVehicleID());
        spotMap.remove(reservetion);
    }

    private float calcPayment(Reservetion reservetion) {
        int secInHour = 3600;
        float cost = (float) reservetion.getOverallTime().getSeconds() / secInHour;
        return cost * reservetion.getSpotSize().costPerHour;
    }

    public void showRecite(Reservetion reservetion){
        System.out.println("Parking Recipe");
        System.out.println("Vehicle ID : " + reservetion.getVehicleID());
        System.out.println("Reservetion ID : " + reservetion.getReserevID());
        System.out.println("Spot ID : " + reservetion.getSpotID());
        System.out.println("start time : " + reservetion.getStartTime());
        if(null != reservetion.getEndTime()){
            System.out.println("end time : " + reservetion.getEndTime());
            System.out.println("overall time : " + reservetion.getOverallTime());
            System.out.println("cost to park : " + calcPayment(reservetion));
        }
    }

}