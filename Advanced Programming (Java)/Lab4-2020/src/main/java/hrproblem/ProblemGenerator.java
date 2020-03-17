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

    /**
     * A static function which generates a random HR problem
     * The number of residents will be in [6, 12] interval
     * The number of hospitals will be in [10,20] interval
     *
     * @return the random generated problem
     */
    public static Problem generateProblem() {
        Faker faker = new Faker();
        Problem randomProblem = new Problem(faker.app().name());

        int residentsCount = RandomGenerator.intBetween(minimumNumberOfResidents, maximumNumberOfResidents);
        int hospitalsCount = RandomGenerator.intBetween(minimumNumberOfHospitals, maximumNumberOfHospitals);

        Resident[] residents = new Resident[residentsCount];
        for (int i = 0; i < residentsCount; ++i) {
            residents[i] = new Resident(faker.name().fullName());
        }

        Hospital[] hospitals = new Hospital[hospitalsCount];
        for (int i = 0; i < hospitalsCount; ++i) {
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

    /**
     * A function which generates, for each resident, a list of hospitals (his preferences)
     *
     * @param residents
     * @param hospitals
     * @param faker     an instance of Faker class
     * @return a Map of type <Resident, Hospitals>
     */
    private static Map<Resident, List<Hospital>> generateResidentsPreferences(Resident[] residents, Hospital[] hospitals, Faker faker) {
        Map<Resident, List<Hospital>> residentListMap = new HashMap<>();

        for (Resident resident : residents) {
            List<Hospital> thisResidentPreferences = new ArrayList<>();

            int hospitalsCount = RandomGenerator.intBetween(1, hospitals.length);
            List<Integer> permutation = RandomGenerator.getPermutationOfKNumbers(hospitalsCount);
            for (int j = 0; j < hospitalsCount; ++j) {
                thisResidentPreferences.add(hospitals[permutation.get(j)]);
            }

            residentListMap.put(resident, thisResidentPreferences);
        }

        return residentListMap;
    }

    /**
     * A function which generates, for each hospital, a list of residents (its preferences)
     *
     * @param residents
     * @param hospitals
     * @param faker     an instance of Faker class
     * @return a Map of type <Hospital, Residents>
     */
    private static Map<Hospital, List<Resident>> generateHospitalsPreferences(Resident[] residents, Hospital[] hospitals, Faker faker) {
        Map<Hospital, List<Resident>> hospitalListMap = new TreeMap<>();

        for (Hospital hospital : hospitals) {
            List<Resident> thisHospitalPreferences = new ArrayList<>();

            int residentsCount = RandomGenerator.intBetween(1, residents.length);
            List<Integer> permutation = RandomGenerator.getPermutationOfKNumbers(residentsCount);
            for (int j = 0; j < residentsCount; ++j) {
                thisHospitalPreferences.add(residents[permutation.get(j)]);
            }

            hospitalListMap.put(hospital, thisHospitalPreferences);
        }

        return hospitalListMap;
    }
}
