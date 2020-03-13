package mdvsp.optional;

import javafx.util.Pair;
import mdvsp.common.BaseVehicle;
import mdvsp.common.Client;
import mdvsp.common.Depot;
import mdvsp.graph.DirectedGraph;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;

public class Problem {
    private String name;
    private ArrayList<Depot> depots = new ArrayList<>();
    private ArrayList<Client> clients = new ArrayList<>();
    private DirectedGraph graph = null;
    private int[][] costMatrix = null;

    public Problem(){
        name = "No name problem";
    }

    public Problem(String problemName){ name = problemName; }

    /**
     * Adds new depots to the problem
     * @param newDepots an array containing the newly added depots
     */
    public void addDepots(Depot ... newDepots){
        for (Depot d:newDepots){
            boolean add = true;
            for (int i=0;i<depots.size();++i){
                if (depots.get(i).equals(d)){
                    add = false;
                    break;
                }
            }
            if (!add){
                continue;
            }
            depots.add(d);
        }
    }

    /**
     * Adds new clients to the problem
     * @param newClients an array containing the newly added clients
     */
    public void addClients(Client ... newClients){
        for (Client c:newClients){
            boolean add = true;
            for (int i=0;i<clients.size();++i){
                if (clients.get(i).equals(c)){
                    add = false;
                    break;
                }
            }
            if (!add){
                continue;
            }
            clients.add(c);
        }
    }

    /**
     * Loads into memory the cost matrix of the problem
     * @param fileName the name of the file which contains the cost matrix
     * @return true if the cost matrix was loaded successfully, false otherwise
     */
    public boolean loadCostMatrixFromFile(String fileName){
        BufferedReader bufferedReader;
        int lineCount = 0;
        costMatrix = new int[clients.size()+depots.size()][clients.size()+depots.size()];
        try{
            bufferedReader = new BufferedReader(new FileReader(fileName));
            String line = bufferedReader.readLine();
            while (line != null && lineCount < clients.size() + depots.size()){
                if (line.length() == 0){
                    break;
                }
                String[] values = line.split("\\s+");
                if (values.length != clients.size() + depots.size()){
                    System.out.println("The "+lineCount+"th line of the matrix doesn't have the valid number of columns!" +
                            "Expected " + clients.size()+depots.size()+", found "+values.length);
                    return false;
                }
                for (int i=0;i<values.length;++i){
                    costMatrix[lineCount][i] = Integer.parseInt(values[i]);
                }
                ++lineCount;
                line = bufferedReader.readLine();
            }
            bufferedReader.close();;
        } catch (IOException e){
            System.out.println("Error when reading the matrix from the file " + fileName);
            return false;
        }
        if (lineCount < clients.size() + depots.size()){
            System.out.println("The cost matrix has too few lines!");
            return false;
        }
        buildGraph();
        return true;
    }

    /**
     * Returns a 'SolutionBonus' object for this problem
     * @return an object describing the solution based on the cost matrix for this problem
     */
    public SolutionBonus getSolutionBasedOnCostMatrix() {
        if (costMatrix == null || costMatrix.length != depots.size() + clients.size()){
            return null;
        }
        SolutionBonus solution = new SolutionBonus(this);
        boolean[] selectedClients = new boolean[clients.size()];
        int selectedClientsCount = 0, currentDepot = 0, currentVehicle = 0;

        while (selectedClientsCount < clients.size() && currentDepot < depots.size()){
            Vehicle currentTourVehicle = (Vehicle)depots.get(currentDepot).getKthVehicle(currentVehicle);
            Pair<Tour,Integer> tourCost = getNextTourBasedOnCostMatrix(currentTourVehicle, currentDepot, selectedClients);
            if (tourCost.getKey().getClientsCount() != 0){
                selectedClientsCount += tourCost.getKey().getClientsCount();
                solution.addTour(tourCost.getKey(), tourCost.getValue());
            }

            ++currentVehicle;
            if (currentVehicle == depots.get(currentDepot).getVehiclesCount()){
                ++currentDepot;
                currentVehicle = 0;
            }
        }

        if (selectedClientsCount != clients.size()){
            solution.setComplete(false);
        }
        return solution;
    }

    public String getName() {
        return name;
    }

    public int getDepotsCount() {
        return depots.size();
    }

    public ArrayList<Client> getClients(){
        return clients;
    }

    /**
     * Sets a new name for the problem
     * @param name the new name of the problem
     */
    public void setName(String name) {
        this.name = name;
    }

    /**
     * Returns all the vehicles in this problem
     * @return an array containing all the vehicles from the depots
     */
    public ArrayList<Vehicle> getVehicles(){
        ArrayList<Vehicle> allVehicles = new ArrayList<>();

        for (int i=0;i<depots.size();++i){
            ArrayList<BaseVehicle> thisDepotVehicles = depots.get(i).getVehicles();
            for (BaseVehicle baseVehicle:thisDepotVehicles){
                allVehicles.add((Vehicle)baseVehicle);
            }
        }

        return allVehicles;
    }

    /**
     * Returns a 'Solution' object for this problem
     * @return an object describing the solution for the problem
     */
    public Solution getSolution(){
        Solution solution = new Solution(this);
        boolean[] selectedClients = new boolean[clients.size()];
        int selectedClientsCount = 0, currentDepot = 0, currentVehicle = 0;

        if (!clientsAreSortedBasedOnVisitingTimes()){
            sortClientsBasedOnVisitingTimes();
        }

        while (selectedClientsCount < clients.size() && currentDepot < depots.size()){
            Vehicle currentTourVehicle = (Vehicle)depots.get(currentDepot).getKthVehicle(currentVehicle);
            Tour newTour = getNextTour(currentTourVehicle, selectedClients);
            selectedClientsCount += newTour.getClientsCount();
            solution.addTour(newTour);

            ++currentVehicle;
            if (currentVehicle == depots.get(currentDepot).getVehiclesCount()){
                ++currentDepot;
                currentVehicle = 0;
            }
        }

        if (selectedClientsCount != clients.size()){
            solution.setComplete(false);
        }
        return solution;
    }

    public int[] getShortestPathsCostToAllTheClientsFromNode(int node, boolean nLog){
        return graph.getShortestPathsFromNodeK(node, nLog);
    }

    /**
     * Creates a new tour, taking into account all the clients that weren't already selected
     * @param tourVehicle the vehicle used for the tour that will be created
     * @param selectedClients an array indicating which clients were already selected in a tour
     * @return a new tour
     */
    private Tour getNextTour(Vehicle tourVehicle, boolean[] selectedClients){
        Tour tour = new Tour(tourVehicle);
        int lastSelectedTime = -1;
        for (int i=0;i<clients.size();++i){
            if (selectedClients[i] || lastSelectedTime >= clients.get(i).getVisitingTime()){
                continue;
            }
            selectedClients[i] = true;
            lastSelectedTime = clients.get(i).getVisitingTime();
            tour.addClient(clients.get(i), true);
        }
        return tour;
    }

    /**
     * Creates a new tour, taking into account all the clients that weren't already selected, returning also the cost of the tour
     * @param tourVehicle the vehicle used for the tour
     * @param depotIndex the id of the depot where the vehicle is located
     * @param selectedClients an array indicating which clients were already selected in a tour
     * @return a pair consisting of the new tour and its cost
     */
    private Pair<Tour,Integer> getNextTourBasedOnCostMatrix(Vehicle tourVehicle, int depotIndex, boolean[] selectedClients){
        Tour tour = new Tour(tourVehicle);
        int cost = 0, lastChosenClient = -1;

        for (int i=0;i<clients.size();++i){
            if (!selectedClients[i] && costMatrix[depotIndex][i+depots.size()] > 0){
                lastChosenClient = i;
                cost += costMatrix[depotIndex][i+depots.size()];
                selectedClients[i] = true;
                tour.addClient(clients.get(i), false);
                break;
            }
        }
        if (lastChosenClient != -1){
            boolean found = true;
            while (found){
                found = false;
                for (int i=0;i<clients.size();++i){
                    if (!selectedClients[i] && costMatrix[lastChosenClient+depots.size()][i+depots.size()] > 0){
                        found = true;
                        cost += costMatrix[lastChosenClient+depots.size()][i+depots.size()];
                        lastChosenClient = i;
                        selectedClients[i] = true;
                        tour.addClient(clients.get(i), false);
                        break;
                    }
                }
            }
        }
        return new Pair<>(tour, cost);
    }

    /**
     * Checks whether the clients array is sorted in ascending order
     * @return true if the arrays is sorted, false otherwise
     */
    private boolean clientsAreSortedBasedOnVisitingTimes(){
        for (int i=0;i<clients.size()-1;++i){
            if (clients.get(i).getVisitingTime() > clients.get(i+1).getVisitingTime()){
                return false;
            }
        }
        return true;
    }

    /**
     * Sorts the clients array in ascending order, based on their visiting times
     */
    private void sortClientsBasedOnVisitingTimes(){
        clients.sort(Comparator.comparingInt(Client::getVisitingTime));
    }

    /**
     * Builds the directed graph associated with the cost matrix
     */
    private void buildGraph(){
        graph = new DirectedGraph(costMatrix.length);
        for (int i=0;i<costMatrix.length;++i){
            for (int j=0;j<costMatrix.length;++j){
                if (i == j || costMatrix[i][j] == -1){
                    continue;
                }
                graph.addWeightedEdge(i, j, costMatrix[i][j]);
            }
        }
    }

    @Override
    public String toString(){
        return getString(name, depots, clients);
    }

    public static String getString(String name, ArrayList<Depot> depots, ArrayList<Client> clients) {
        StringBuilder stringBuilder = new StringBuilder();

        stringBuilder.append(name).append('\n');

        if (depots.size() > 0){
            stringBuilder.append("Depots:\n");
            for (Depot depot: depots){
                stringBuilder.append('\t').append(depot.toString()).append('\n');
            }
        } else {
            stringBuilder.append("No depots\n");
        }

        if (clients.size() > 0) {
            stringBuilder.append("Clients:\n");
            for (Client client: clients){
                stringBuilder.append('\t').append(client.toString()).append('\n');
            }
        } else {
            stringBuilder.append("No clients");
        }

        return stringBuilder.toString();
    }
}
