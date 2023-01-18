

    public class Spot implements ParkAble {
        private static int spotIDGlobal = 1;
        private final int spotID;
        private final SpotSize spotSize;
        boolean isFree = true;

        Spot(SpotSize spotSize) {
            this.spotID = CreateSpotID();
            this.spotSize = spotSize;
        }

        private int CreateSpotID() {
            int num = spotIDGlobal;
            spotIDGlobal++;
            return num;
        }

        @Override
        public Spot park(SpotSize spotSize) {
            if(isFree() && getSpotSize() == spotSize) {
                setFree(false);
                return this;
            }
            return null;
        }


        public int getSpotID() {
            return spotID;
        }

        public SpotSize getSpotSize() {
            return spotSize;
        }

        public boolean isFree() {
            return isFree;
        }

        public void setFree(boolean free) {
            isFree = free;
        }


    }
