import java.util.Scanner;

public class Wrapper {
    private int floors;
    private int spotsEachFloor;
    private final ParkingLotManager parkingLotManager;
    private final Scanner input = new Scanner(System.in);

    public Wrapper(int floors, int spotsEachFloor) {
        this.floors = floors;
        this.spotsEachFloor = spotsEachFloor;
        parkingLotManager = new ParkingLotManager(floors,spotsEachFloor);
    }
    // to add mor floors, spots,
    public void showMenue(){
        boolean exit = false;
        while (!exit) {
            parkingLotManager.showMenue();
            System.out.println("press 0 to exit");
            if(0 == input.nextInt()){
                exit = true;
            }
        }

    }

    public static void main(String[] args) {
        Wrapper wrapper = new Wrapper(3,5);
        wrapper.showMenue();
    }

}
