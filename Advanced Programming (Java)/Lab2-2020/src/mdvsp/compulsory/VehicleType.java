package mdvsp.compulsory;

public enum VehicleType {
    CAR(0), TRUCK(1), DRONE(2);

    private final int value;
    VehicleType(int value){
        this.value = value;
    }

    public int getValue(){
        return value;
    }
}