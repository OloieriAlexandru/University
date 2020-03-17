package hrproblem;

import common.Hospital;
import common.Resident;

import java.util.*;

// Create a class that describes the HR Problem (or any stable matching problem!). Consider creating the classes Problem, Partition, Element, Matching.
public class Problem {
    private String name;
    private List<Resident> residents = new ArrayList<>();
    private List<Hospital> hospitals = new ArrayList<>();
    private Map<Resident, Integer> residentIndexes = new HashMap<>();
    private Map<Hospital, Integer> hospitalIndexes = new TreeMap<>();
    private Map<Resident, List<Hospital>> residentsPreferences = new HashMap<>();
    private Map<Hospital, List<Resident>> hospitalPreferences = new TreeMap<>();

    public Problem(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void addResidents(Resident... addedResidents) {
        for (Resident addedResident : addedResidents) {
            residentIndexes.put(addedResident, residents.size());
            residents.add(addedResident);
        }
    }

    public void addHospitals(Hospital... addedHospitals) {
        for (Hospital addedHospital : addedHospitals) {
            hospitalIndexes.put(addedHospital, hospitals.size());
            hospitals.add(addedHospital);
        }
    }

    public void setHospitalPreferences(Map<Hospital, List<Resident>> hospitalPreferences) {
        this.hospitalPreferences = hospitalPreferences;
    }

    public void setResidentsPreferences(Map<Resident, List<Hospital>> residentsPreferences) {
        this.residentsPreferences = residentsPreferences;
    }

    public List<Hospital> getResidentHospitalsPreferences(Resident resident) {
        return residentsPreferences.get(resident);
    }

    public List<Resident> getHospitalResidentsPreferences(Hospital hospital) {
        return hospitalPreferences.get(hospital);
    }

    /**
     * Bonus task: Implement the Gale Shapley algorithm in order to find a stable matching.
     * https://en.wikipedia.org/wiki/Gale%E2%80%93Shapley_algorithm
     * Implementation idea: https://www.geeksforgeeks.org/stable-marriage-problem/
     *
     * @return a stable matching for the problem
     */
    public Matching getStableMatching() {
        List<List<Resident>> hospitalResidents = new ArrayList<>();
        for (int i = 0; i < hospitals.size(); ++i) {
            hospitalResidents.add(new ArrayList<>());
        }
        boolean[] occupiedResident = new boolean[residents.size()];
        int freeResidentsCount = Math.min(occupiedResident.length, computeTotalCapacityOfHospitals());

        while (freeResidentsCount > 0) {
            int which = findFirstFreeResident(occupiedResident);
            if (which == -1) {
                break;
            }
            List<Hospital> hospitals = residentsPreferences.get(residents.get(which));
            for (int i = 0; i < hospitals.size() && !occupiedResident[which]; ++i) {
                Hospital hospital = hospitals.get(i);
                int hospitalIndex = hospitalIndexes.get(hospital);
                if (hospitalResidents.get(hospitalIndex).size() < hospital.getCapacity()) {
                    occupiedResident[which] = true;
                    --freeResidentsCount;
                    hospitalResidents.get(hospitalIndex).add(residents.get(which));
                } else {
                    List<Resident> thisHospitalResidents = hospitalResidents.get(hospitalIndex);
                    for (int j = 0; j < thisHospitalResidents.size(); ++j) {
                        Resident res = thisHospitalResidents.get(j);
                        if (!hospitalPrefersCurrentResident(hospital, residents.get(which), res)) {
                            hospitalResidents.get(hospitalIndex).set(j, residents.get(which));
                            occupiedResident[which] = true;
                            int resIndex = residentIndexes.get(res);
                            occupiedResident[resIndex] = false;
                            break;
                        }
                    }
                }
            }
            occupiedResident[which] = true;
        }
        return constructMatching(hospitalResidents);
    }

    /**
     * A function used in Gale Shapley algorithm
     *
     * @return the sum of capacities of all hospitals
     */
    private int computeTotalCapacityOfHospitals() {
        int result = 0;
        for (Hospital hospital : hospitals) {
            result += hospital.getCapacity();
        }
        return result;
    }

    /**
     * A function used in Gale Shapley algorithm
     * It checks whether the current assigned resident has a higher priority than the free resident
     */
    private boolean hospitalPrefersCurrentResident(Hospital hospital, Resident freeResident, Resident currentResident) {
        List<Resident> hospitalPref = hospitalPreferences.get(hospital);
        for (Resident resident : hospitalPref) {
            if (resident.equals(currentResident)) {
                return true;
            }
            if (resident.equals(freeResident)) {
                return false;
            }
        }
        return true;
    }

    /**
     * A function used in Gale Shapley algorithm
     *
     * @param occupiedResident a boolean array, occupiedResident[i] = false if the i'th resident is free
     * @return the index of the first free resident
     */
    private int findFirstFreeResident(boolean[] occupiedResident) {
        for (int i = 0; i < occupiedResident.length; ++i) {
            if (!occupiedResident[i]) {
                return i;
            }
        }
        return -1;
    }

    /**
     * A function which constructs a matching based on the result of Gale Shapley algorithm
     *
     * @param hospitalResidents the result of Gale Shapley algorithm
     * @return a stable matching for the problem
     */
    private Matching constructMatching(List<List<Resident>> hospitalResidents) {
        Matching matching = new Matching(this, true);

        for (int i = 0; i < hospitalResidents.size(); ++i) {
            for (int j = 0; j < hospitalResidents.get(i).size(); ++j) {
                matching.addPair(hospitalResidents.get(i).get(j), hospitals.get(i));
            }
        }

        return matching;
    }


    /**
     * Optional, 2nd task
     * For creating the matching the Hopcroft Karp algorithm is implemented
     * https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm
     *
     * @return a matching for the problem
     */
    public Matching getMatching() {
        Matching problemMatching = new Matching(this, false);

        int[] leftMatching = new int[residents.size()];
        Arrays.fill(leftMatching, -1);
        int[] rightMatching = new int[hospitals.size()];
        Arrays.fill(rightMatching, -1);
        int[] usedCapacities = new int[hospitals.size()];
        boolean[] visited = new boolean[residents.size()];

        boolean running = true;
        while (running) {
            running = false;
            Arrays.fill(visited, false);
            for (int i = 0; i < leftMatching.length; ++i) {
                if (!visited[i] && leftMatching[i] == -1) {
                    if (couple(i, leftMatching, rightMatching, visited, usedCapacities)) {
                        running = true;
                    }
                }
            }
        }

        for (int i = 0; i < leftMatching.length; ++i) {
            if (leftMatching[i] != -1) {
                problemMatching.addPair(residents.get(i), hospitals.get(leftMatching[i]));
            }
        }
        return problemMatching;
    }

    /**
     * A helper function for the Hopcroft Karp algorithm
     */
    boolean couple(int node, int[] leftMatching, int[] rightMatching, boolean[] visited, int[] usedCapacities) {
        if (visited[node]) {
            return false;
        }
        visited[node] = true;
        List<Hospital> hospitals = residentsPreferences.get(residents.get(node));
        for (Hospital value : hospitals) {
            int hospitalIndex = hospitalIndexes.get(value);
            if (rightMatching[hospitalIndex] != -1 && usedCapacities[hospitalIndex] >= value.getCapacity()) {
                continue;
            }
            leftMatching[node] = hospitalIndex;
            rightMatching[hospitalIndex] = node;
            ++usedCapacities[hospitalIndex];
            return true;
        }
        for (Hospital hospital : hospitals) {
            int hospitalIndex = hospitalIndexes.get(hospital);
            if (couple(rightMatching[hospitalIndex], leftMatching, rightMatching, visited, usedCapacities)) {
                leftMatching[node] = hospitalIndex;
                rightMatching[hospitalIndex] = node;
                return true;
            }
        }
        return false;
    }
}
