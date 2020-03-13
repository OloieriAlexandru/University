package hrproblem;

import com.github.javafaker.Faker;
import common.Hospital;
import common.RandomGenerator;
import common.Resident;

import java.util.*;

public class ProblemGenerator {
    private static int minimumNumberOfResidents = 6;
    private static int maximumNumberOfResidents = 12;
    private static int minimumNumberOfHospitals = 10;
    private static int maximumNumberOfHospitals = 20;

    public static Problem generateProblem(){
        Faker faker = new Faker();
        Problem randomProblem = new Problem(faker.app().name());

        int residentsCount = RandomGenerator.intBetween(minimumNumberOfResidents, maximumNumberOfResidents);
        int hospitalsCount = RandomGenerator.intBetween(minimumNumberOfHospitals, maximumNumberOfHospitals);

        Resident[] residents = new Resident[residentsCount];
        for (int i=0;i<residentsCount;++i){
            residents[i] = new Resident(faker.name().fullName());
        }

        Hospital[] hospitals = new Hospital[hospitalsCount];
        for (int i=0;i<hospitalsCount;++i){
            hospitals[i] = new Hospital(faker.name().lastName(), RandomGenerator.intBetween(1, 4));
        }

        randomProblem.addResidents(residents);
        randomProblem.addHospitals(hospitals);

        Map<Resident, List<Hospital>> residentListMap = generateResidentsPreferences(residents, hospitals, faker);
        Map<Hospital, List<Resident>> hospitalListMap = generateHospitalsPreferences(residents, hospitals, faker);

        randomProblem.setResidentsPreferences(residentListMap);
        randomProblem.setHospitalPreferences(hospitalListMap);

        return randomProblem;
    }

    private static Map<Resident, List<Hospital>> generateResidentsPreferences(Resident[] residents, Hospital[] hospitals, Faker faker){
        Map<Resident, List<Hospital>> residentListMap = new HashMap<>();

        for (int i=0;i<residents.length;++i){
            List<Hospital> thisResidentPreferences = new ArrayList<>();

            int hospitalsCount = RandomGenerator.intBetween(1, hospitals.length);
            List<Integer> permutation = RandomGenerator.getPermutationOfKNumbers(hospitalsCount);
            for (int j=0;j<hospitalsCount;++j){
                thisResidentPreferences.add(hospitals[permutation.get(j)]);
            }

            residentListMap.put(residents[i], thisResidentPreferences);
        }

        return residentListMap;
    }

    private static Map<Hospital, List<Resident>> generateHospitalsPreferences(Resident[] residents, Hospital[] hospitals, Faker faker){
        Map<Hospital, List<Resident>> hospitalListMap = new TreeMap<>();

        for (int i=0;i<hospitals.length;++i){
            List<Resident> thisHospitalPreferences = new ArrayList<>();

            int residentsCount = RandomGenerator.intBetween(1, residents.length);
            List<Integer> permutation = RandomGenerator.getPermutationOfKNumbers(residentsCount);
            for (int j=0;j<residentsCount;++j){
                thisHospitalPreferences.add(residents[permutation.get(j)]);
            }

            hospitalListMap.put(hospitals[i], thisHospitalPreferences);
        }

        return hospitalListMap;
    }
}
