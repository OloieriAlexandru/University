package mdvsp.compulsory;

import mdvsp.common.Client;
import mdvsp.common.Depot;

import java.util.ArrayList;

import static mdvsp.optional.Problem.getString;

public class Problem {
    private String name;
    private ArrayList<Depot> depots = new ArrayList<>();
    private ArrayList<Client> clients = new ArrayList<>();

    public Problem(){
        name = "No name problem";
    }

    public Problem(String problemName){
        name = problemName;
    }

    public void addDepot(Depot depot){
        depots.add(depot);
    }

    public void addClient(Client client){
        clients.add(client);
    }

    @Override
    public String toString(){
        return getString(name, depots, clients);
    }
}
