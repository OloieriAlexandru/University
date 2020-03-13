package hrproblem;

import common.Hospital;
import common.Resident;

import java.util.*;

// Create a class that describes the HR Problem (or any stable matching problem!). Consider creating the classes Problem, Partition, Element, Matching.
public class Problem {
    private String name;
    private List<Resident> residents = new ArrayList<>();
    private List<Hospital> hospitals = new ArrayList<>();
    private Map<Hospital, Integer> hospitalIndexes = new TreeMap<>();
    private Map<Resident, List<Hospital>> residentsPreferences = new HashMap<>();
    private Map<Hospital, List<Resident>> hospitalPreferences = new TreeMap<>();

    public Problem(String name){
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void addResidents(Resident ... addedResidents){
        residents.addAll(Arrays.asList(addedResidents));
    }

    public void addHospitals(Hospital ... addedHospitals){
        for (int i=0;i<addedHospitals.length;++i){
            hospitalIndexes.put(addedHospitals[i], hospitals.size());
            hospitals.add(addedHospitals[i]);
        }
    }

    public void setHospitalPreferences(Map<Hospital, List<Resident>> hospitalPreferences) {
        this.hospitalPreferences = hospitalPreferences;
    }

    public void setResidentsPreferences(Map<Resident, List<Hospital>> residentsPreferences) {
        this.residentsPreferences = residentsPreferences;
    }

    public List<Hospital> getResidentHospitalsPreferences(Resident resident){
        return residentsPreferences.get(resident);
    }

    public List<Resident> getHospitalResidentsPreferences(Hospital hospital){
        return hospitalPreferences.get(hospital);
    }

    // Implement an algorithm for creating a matching.
    public Matching getMatching(){
        Matching problemMatching = new Matching(this);

        int[] leftMatching = new int[residents.size()];
        Arrays.fill(leftMatching, -1);
        int[] rightMatching = new int[hospitals.size()];
        Arrays.fill(rightMatching, -1);
        int[] usedCapacities = new int[hospitals.size()];
        boolean[] visited = new boolean[residents.size()];

        boolean running = true;
        while (running){
            running = false;
            Arrays.fill(visited, false);
            for (int i=0;i<leftMatching.length;++i){
                if (!visited[i] && leftMatching[i] == -1){
                    if (couple(i, leftMatching, rightMatching, visited, usedCapacities)){
                        running = true;
                    }
                }
            }
        }

        for (int i=0;i<leftMatching.length;++i){
            if (leftMatching[i] != -1){
                problemMatching.addPair(residents.get(i), hospitals.get(leftMatching[i]));
            }
        }
        return problemMatching;
    }

    // Hopcroft–Karp algorithm
    boolean couple(int node, int[] leftMatching, int[] rightMatching, boolean[] visited, int[] usedCapacities){
        if (visited[node]){
            return false;
        }
        visited[node] = true;
        List<Hospital> hospitals = residentsPreferences.get(residents.get(node));
        for (int i=0;i<hospitals.size();++i){
            int hospitalIndex = hospitalIndexes.get(hospitals.get(i));
            if (rightMatching[hospitalIndex] != -1 && usedCapacities[hospitalIndex] >= hospitals.get(i).getCapacity()){
                continue;
            }
            leftMatching[node] = hospitalIndex;
            rightMatching[hospitalIndex] = node;
            ++usedCapacities[hospitalIndex];
            return true;
        }
        for (int i=0;i<hospitals.size();++i){
            int hospitalIndex = hospitalIndexes.get(hospitals.get(i));
            if (couple(rightMatching[hospitalIndex], leftMatching, rightMatching, visited, usedCapacities)){
                leftMatching[node] = hospitalIndex;
                rightMatching[hospitalIndex] = node;
                return true;
            }
        }
        return false;
    }
}
