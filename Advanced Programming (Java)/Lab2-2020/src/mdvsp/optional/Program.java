package mdvsp.optional;

import mdvsp.common.Client;
import mdvsp.common.Depot;

import java.util.ArrayList;

public class Program {
    public static void main(String[] args){
        Depot depot1 = new Depot("D1");
        Depot depot2 = new Depot("D2");

        Vehicle vehicle1 = new Truck("V1");
        Vehicle vehicle2 = new Car("V2");
        Vehicle vehicle3 = new Drone("V3");

        depot1.addVehicle(vehicle1);
        depot1.addVehicle(vehicle2);
        depot2.addVehicle(vehicle3);

        Client client1 = new Client("C1", 1);
        Client client2 = new Client("C2", 1);
        Client client3 = new Client("C3", 2);
        Client client4 = new Client("C4", 2);
        Client client5 = new Client("C5", 3);

        Problem statementProblem = new Problem("MDVSP");

        statementProblem.addDepots(depot1, depot2);
        statementProblem.addClients(client1, client2, client3, client4, client5);

        ArrayList<Vehicle> vehicles = statementProblem.getVehicles();
        System.out.println("The vehicles in all the depots: ");
        for (int i=0;i<vehicles.size();++i){
            System.out.println(vehicles.get(i));
        }

        Solution statementProblemSolution = statementProblem.getSolution();
        System.out.println();
        System.out.println(statementProblemSolution);

        // Bonus:
        if (statementProblem.loadCostMatrixFromFile("statement-cost-matrix.txt")) {
            SolutionBonus statementProblemSolutionBonus = statementProblem.getSolutionBasedOnCostMatrix();
            System.out.println();
            System.out.println(statementProblemSolutionBonus);

            int[] dijkstraShortestPaths = statementProblem.getShortestPathsCostToAllTheClientsFromNode(0, true);
            int[] bellmanFordShortestPaths = statementProblem.getShortestPathsCostToAllTheClientsFromNode(0, false);

            System.out.println("Shortest paths using Dijkstra algorithm:");
            for (int i=statementProblem.getDepotsCount();i<dijkstraShortestPaths.length;++i){
                System.out.print(dijkstraShortestPaths[i]+" ");
            }
            System.out.println();

            System.out.println("Shortest paths using Bellman-Ford algorithm:");
            for (int i=statementProblem.getDepotsCount();i<bellmanFordShortestPaths.length;++i){
                System.out.print(bellmanFordShortestPaths[i]+" ");
            }
            System.out.println();
        }
    }
}
