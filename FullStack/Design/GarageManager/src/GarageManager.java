
import jdk.nashorn.api.scripting.JSObject;

import java.beans.XMLEncoder;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;


//***didnt understand the the req' about the below O(N) get Sortedlist , i got to iterate through all nodes to get the list;
//private final Map<Integer,List<Vehicle>> vehicleMap = new HashMap<>(); Possible
//private final List<Vehicle> list = new ArrayList<>();

//
public class GarageManager {
    public static void main(String[] args) throws FileNotFoundException {
        GarageManager garageManager = new GarageManager();
        garageManager.showMenue();
    }
    private boolean isRunning = true;
    private final Map<Integer,Vehicle> vehicleMap = new HashMap<>();
    //private final XMLEncoder encoder ;
    //private final FileOutputStream fos ;
    Scanner input = new Scanner(System.in);
    private static int totalIncome  = 0;

    public GarageManager() throws FileNotFoundException {
        //fos = new FileOutputStream(new File("./c/temp/vehicles.xml"));
        //encoder = new XMLEncoder(fos);
    }

    // I can do Factory of Commands instead of the switch case, No time..
    // Map <Inger, Function>..

    public void showMenue(){
        System.out.println("Welcome, please enter num of service? [1-insert," +
                "2-change status,3-get total income,4-delete vehicle,5-get list of cars[sorted in  years]");
        System.out.println("If you want to exit the program press any other key");

        while(isRunning){
            int numPressed = input.nextInt();
            switch (numPressed){
                case 1:
                    addVehicleUI();
                    break;
                case 2:
                    changeStatusUI();
                    break;
                case 3:
                    System.out.println(getProfitSoFar());
                    break;
                case 4:
                    deleteVehicleUI();
                case 5:
                    showListOfVehicleUI();
                default:
                    isRunning = false;
                    exit();
            }
        }

    }

    private void addVehicleUI(){
        System.out.println("enter type of vehicle [Car,MotorCycle,Truck]");
        System.out.println("enter details of vehicle and separate them by comma");
        System.out.println("type of vehicle,licensePlateID,description,yearOfManufacture," +
                "weightLimit(optional for trucks),twoSeated(optional for motorcycle Y OR N\n");

        String inputFromUser = input.next();
        String [] data = inputFromUser.split(",");
        switch (data[0]){
            case "Car":
                vehicleMap.put(Integer.parseInt(data[3]),
                        new Car(Integer.parseInt(data[1]),data[2],Integer.parseInt(data[3])));

                break;
            case "MotorCycle":
                boolean isTwoSit = "Y" == data[5];
                Vehicle motorCycle = new MotorCycle((Integer.parseInt(data[1])),data[2],Integer.parseInt(data[3]),isTwoSit);
                insertVehicle(Integer.parseInt(data[1]),motorCycle);
                break;

            case "Truck":
                vehicleMap.put(Integer.parseInt(data[3]),
                        new Truck(Integer.parseInt(data[1]),data[2],Integer.parseInt(data[3]),Integer.parseInt(data[4])));
                break;
            default:
                System.out.println("Wrong input ");
                break;
        }
    }

    private void saveToFile(Vehicle vehicle){
        //encoder.writeObject(vehicle);
    }


    private void insertVehicle(int licensePLateID,Vehicle vehicle){
        vehicleMap.put(licensePLateID,vehicle);
        sortVehiclesByeYear(vehicle);
        saveToFile(vehicle);
        //encoder.close();
    }

    private void sortVehiclesByeYear(Vehicle vehicle){
        /*
        if(listOfVehicles.isEmpty()) listOfVehicles.add(vehicle);

        for (Vehicle current:listOfVehicles) {
            if(current.getYearOfManufacture() > vehicle.getYearOfManufacture()){
            insetToList()
               //add to to specific Location of list;
               //break;
            }

        }
         */
    }


    private void changeStatusUI(){

        System.out.println("enter Id of car");
        int  ID = input.nextInt();
        System.out.println("enter Status");
        String status = input.next();
        Status enumStatus = null;
        switch (status){
            case "NEW":
                enumStatus = Status.NEW;
                break;
            case "FIXED":
                enumStatus = Status.FIXED;
                break;
            case "INPROCESS":
                enumStatus = Status.INPROCESS;
                break;
            case "REALESE":
                enumStatus = Status.REALESE;
                deleteVehicle(ID);
                break;
            default:
                System.out.println("Wrong input");
                break;

        }
        changeStatus(ID,enumStatus);
    }

    private void changeStatus(int licensePLateID,Status status){
        vehicleMap.get(licensePLateID).setStatus(status);
    }

    private void deleteVehicleUI(){}

    private void deleteVehicle(int licensePLateID){
        totalIncome += vehicleMap.get(licensePLateID).getCost();
        //change to release in log
        vehicleMap.remove(licensePLateID);
    }

    private int getProfitSoFar() {
        return totalIncome;
    }

    private List<Vehicle> showListOfVehicleUI(){
        // if (listOfVehicle.isEmpty()) return null;
       // return listOfVehicle
        return  null;
    };


    private void exit(){
        System.out.println("Bye Bye");
    };
}