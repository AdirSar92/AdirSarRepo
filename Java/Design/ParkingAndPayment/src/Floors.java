import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class Floors implements ParkAble {
    private final List<Spot> spotList;
    int numOfFree;

    public Floors(int numOfFree) {
        this.numOfFree = numOfFree;
        this.spotList = new ArrayList<>(numOfFree);
        while(numOfFree > 0){
            spotList.add(new Spot(SpotSize.MED));
            --numOfFree;
        }

    }

    @Override
    public Spot park(SpotSize spotSize) {
        for (Spot spot : spotList) {
            if(null != spot.park(spotSize)){
                return  spot;
            }
        }
        if(spotSize != SpotSize.BIG){
            park(SpotSize.BIG.increaseSize(spotSize));
        }
        return null;
    }


}
