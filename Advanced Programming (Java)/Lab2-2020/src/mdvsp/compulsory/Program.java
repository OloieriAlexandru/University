package mdvsp.compulsory;

import mdvsp.common.Client;
import mdvsp.common.Depot;

public class Program {
    public static void main(String[] args){
        Depot depot1 = new Depot("D1");
        Depot depot2 = new Depot("D2");

        Vehicle vehicle1 = new Vehicle("V1", VehicleType.CAR);
        Vehicle vehicle2 = new Vehicle("V2", VehicleType.DRONE);
        Vehicle vehicle3 = new Vehicle("V3", VehicleType.TRUCK);

        depot1.addVehicle(vehicle1);
        depot1.addVehicle(vehicle2);
        depot2.addVehicle(vehicle3);

        Client client1 = new Client("C1", 1);
        Client client2 = new Client("C2", 1);
        Client client3 = new Client("C3", 2);
        Client client4 = new Client("C4", 2);
        Client client5 = new Client("C5", 3);

        Problem statementProblem = new Problem("The problem from the statement");

        statementProblem.addDepot(depot1);
        statementProblem.addDepot(depot2);
        statementProblem.addClient(client1);
        statementProblem.addClient(client2);
        statementProblem.addClient(client3);
        statementProblem.addClient(client4);
        statementProblem.addClient(client5);

        System.out.println(statementProblem);
    }
}
