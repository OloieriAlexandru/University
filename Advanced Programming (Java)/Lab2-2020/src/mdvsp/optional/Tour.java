package mdvsp.optional;

import mdvsp.common.Client;
import mdvsp.common.Depot;

import java.util.ArrayList;

public class Tour {
    private Vehicle vehicle = null;
    private ArrayList<Client> clients = new ArrayList<>();

    public Tour(Vehicle vehicle){
        this.vehicle = vehicle;
    }

    /**
     * Adds a new client to the tour
     * @param newClient the newly added client
     * @param check true if the function is called when creating a normal solution, false if it is called when creating a bonus solution
     */
    public void addClient(Client newClient, boolean check){
        if (!check){
            clients.add(newClient);
        } else {
            if (clients.size() == 0 || clients.get(clients.size()-1).getVisitingTime() < newClient.getVisitingTime()){
                clients.add(newClient);
            }
        }
    }

    /**
     * Returns the number of clients visited in this tour
     * @return an integer, the number of clients
     */
    public int getClientsCount(){
        return clients.size();
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();

        stringBuilder.append("Visited clients: ");
        for (int i=0;i<clients.size();++i){
            stringBuilder.append(clients.get(i).getName()).append('(').append(clients.get(i).getVisitingTime()).append(')');
            if (i+1<clients.size()){
                stringBuilder.append(", ");
            }
        }
        stringBuilder.append("; Used vehicle: ").append(vehicle.getName());

        return stringBuilder.toString();
    }
}
