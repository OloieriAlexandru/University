package mdvsp.common;

import java.util.ArrayList;
import java.util.Objects;

public class Depot {
    private ArrayList<BaseVehicle> vehicles = new ArrayList<>();
    private String name;

    public Depot(String depotName){
        name = depotName;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getVehiclesCount(){
        return vehicles.size();
    }

    public BaseVehicle getKthVehicle(int k){
        if (k < 0 || k >= vehicles.size()){
            return null;
        }
        return vehicles.get(k);
    }

    public void addVehicle(BaseVehicle vehicle){
        for (int i=0;i<vehicles.size();++i){
            if (vehicles.get(i).equals(vehicle)){
                return;
            }
        }
        vehicles.add(vehicle);
    }

    public ArrayList<BaseVehicle> getVehicles(){
        return vehicles;
    }

    @Override
    public String toString(){
        StringBuilder stringBuilder = new StringBuilder();

        stringBuilder.append("Depot: ").append(name).append('\n');
        if (vehicles.size() == 0){
            stringBuilder.append("The depot has no vehicles!");
        } else {
            stringBuilder.append("\tThe vehicles in this depot:\n");
            for (int i=0;i<vehicles.size();++i){
                stringBuilder.append("\t\t").append(vehicles.get(i).toString());
                if (i + 1 < vehicles.size()){
                    stringBuilder.append('\n');
                }
            }
        }

        return stringBuilder.toString();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Depot depot = (Depot) o;
        return Objects.equals(vehicles, depot.vehicles) &&
                Objects.equals(name, depot.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(vehicles, name);
    }
}
