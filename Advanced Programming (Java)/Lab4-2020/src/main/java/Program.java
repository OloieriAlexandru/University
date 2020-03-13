import common.Hospital;
import common.Resident;
import hrproblem.Matching;
import hrproblem.Problem;
import hrproblem.ProblemGenerator;

import java.util.*;

public class Program {
    public static void main(String[] args) {
        // Compulsory part
        // Create all the objects in the example (one by one or using streams).
        Resident r0 = new Resident("R0");
        Resident r1 = new Resident("R1");
        Resident r2 = new Resident("R2");
        Resident r3 = new Resident("R3");

        // Create a list of residents, using an ArrayList implementation. Sort the residents, using a comparator.
        List<Resident> residents = getSortedListOfResidents(r0, r1, r2, r3);

        // Create all the objects in the example (one by one or using streams).
        Hospital h0 = new Hospital("H0", 1);
        Hospital h1 = new Hospital("H1", 2);
        Hospital h2 = new Hospital("H2", 2);

        // Create a set of hospitals, using a TreeSet implementation. Make sure that Hospital objects are comparable.
        Set<Hospital> hospitalSet = getHospitalSet(h0, h1, h2);

        // Create two maps (having different implementations) describing the residents and the hospital preferences and print them on the screen.
        Map<Resident, List<Hospital>> residentListMap = getResidentsPreferences(r0, r1, r2, r3, h0, h1, h2);

        Map<Hospital, List<Resident>> hospitalListMap = getHospitalPreferences(r0, r1, r2, r3, h0, h1, h2);

        // Using Java Stream API, write queries that display the residents who find acceptable H0 and H2, and the hospitals that have R0 as their top preference.
        System.out.println("Residents who find acceptable H0 and H2:");
        residents.stream()
                .filter(r -> residentListMap.get(r).containsAll(Arrays.asList(h0, h2)))
                .forEach(r -> System.out.println(r.getName()));

        System.out.println("Hospitals that have R0 as their top preference:");
        hospitalSet.stream()
                .filter(h -> hospitalListMap.get(h).size() > 0 && hospitalListMap.get(h).get(0).equals(r0))
                .forEach(h -> System.out.println(h.getName()));

        // Optional part
        // Create a class that describes the HR Problem (or any stable matching problem!). Consider creating the classes Problem, Partition, Element, Matching.
        Problem statementProblem = new Problem("HR Statement Problem");
        statementProblem.addResidents(r3, r2, r0, r1);
        statementProblem.addHospitals(h0, h1, h2);
        statementProblem.setResidentsPreferences(residentListMap);
        statementProblem.setHospitalPreferences(hospitalListMap);
        // Implement an algorithm for creating a matching.
        Matching matching = statementProblem.getMatching();
        System.out.print(matching);
        // Verify if the matching produced by your algorithm is stable.
        System.out.println("Stable matching? " + (matching.isStable() ? "yes" : "no"));
        // Use a third-party library in order to generate random fake names for residents and hospitals. Create random instances and test your algorithm.
        for (int i=1;i<=10;++i){
            System.out.println();
            Problem generatedProblem = ProblemGenerator.generateProblem();
            Matching generatedProblemMatching = generatedProblem.getMatching();
            System.out.println("Matching to the random problem " + i + ":");
            System.out.println(generatedProblemMatching);
            System.out.println("Stable matching? " + (generatedProblemMatching.isStable() ? "yes" : "no"));
        }
    }

    private static List<Resident> getSortedListOfResidents(Resident r0, Resident r1, Resident r2, Resident r3){
        List<Resident> residents = new ArrayList<>();
        residents.add(r2);
        residents.add(r1);
        residents.add(r3);
        residents.add(r0);
        residents.sort(Comparator.comparing(Resident::getName));
        System.out.println("All the residents, sorted lexicographically by their name:");
        for (Resident resident : residents) {
            System.out.println(resident);
        }
        return residents;
    }

    private static Set<Hospital> getHospitalSet(Hospital h0, Hospital h1, Hospital h2){
        Set<Hospital> hospitalSet = new TreeSet<>();
        hospitalSet.add(h0);
        hospitalSet.add(h1);
        hospitalSet.add(h2);
        System.out.println("All the hospitals:");
        for (Hospital hospital : hospitalSet) {
            System.out.println(hospital);
        }
        return  hospitalSet;
    }

    private static Map<Resident, List<Hospital>> getResidentsPreferences(Resident r0, Resident r1, Resident r2, Resident r3,
                                                                         Hospital h0, Hospital h1, Hospital h2) {
        Map<Resident, List<Hospital>> residentListMap = new HashMap<>();

        residentListMap.put(r0, new ArrayList<>());
        residentListMap.get(r0).addAll(Arrays.asList(h0, h1, h2));

        residentListMap.put(r1, new ArrayList<>());
        residentListMap.get(r1).addAll(Arrays.asList(h0, h1, h2));

        residentListMap.put(r2, new ArrayList<>());
        residentListMap.get(r2).addAll(Arrays.asList(h0, h1));

        residentListMap.put(r3, new ArrayList<>());
        residentListMap.get(r3).addAll(Arrays.asList(h0, h2));

        System.out.println("Residents preferences:");
        for (Resident resident : residentListMap.keySet()) {
            System.out.print(resident.getName() + ": (");
            ArrayList<Hospital> hospitals = (ArrayList<Hospital>) residentListMap.get(resident);
            for (int i = 0; i < hospitals.size(); ++i) {
                System.out.print(hospitals.get(i).getName());
                if (i + 1 < hospitals.size()) {
                    System.out.print(", ");
                }
            }
            System.out.println(")");
        }

        return residentListMap;
    }

    private static Map<Hospital, List<Resident>> getHospitalPreferences(Resident r0, Resident r1, Resident r2, Resident r3,
                                                                        Hospital h0, Hospital h1, Hospital h2){
        Map<Hospital, List<Resident>> hospitalListMap = new TreeMap<>();

        hospitalListMap.put(h0, new ArrayList<>());
        hospitalListMap.get(h0).addAll(Arrays.asList(r3, r0, r1, r2));

        hospitalListMap.put(h1, new ArrayList<>());
        hospitalListMap.get(h1).addAll(Arrays.asList(r0, r2, r1));

        hospitalListMap.put(h2, new ArrayList<>());
        hospitalListMap.get(h2).addAll(Arrays.asList(r0, r1, r3));

        System.out.println("Hospitals preferences:");
        for (Hospital hospital : hospitalListMap.keySet()) {
            System.out.print(hospital.getName() + ": (");
            ArrayList<Resident> residentsList = (ArrayList<Resident>) hospitalListMap.get(hospital);
            for (int i = 0; i < residentsList.size(); ++i) {
                System.out.print(residentsList.get(i).getName());
                if (i + 1 < residentsList.size()) {
                    System.out.print(", ");
                }
            }
            System.out.println(")");
        }

        return hospitalListMap;
    }
}
