package mdvsp.compulsory;

import mdvsp.common.BaseVehicle;

public class Vehicle extends BaseVehicle {
    private String name;
    private VehicleType type;
    private static String[] typeToName = {"Car", "Truck", "Drone"};

    public Vehicle(String vehicleName, VehicleType vehicleType){
        name = vehicleName;
        type = vehicleType;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public VehicleType getType() {
        return type;
    }

    public void setType(VehicleType type) {
        this.type = type;
    }

    @Override
    public String toString(){
        return "Vehicle: " + name + ", type: " + typeToName[type.getValue()];
    }
}
