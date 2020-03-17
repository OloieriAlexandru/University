package hrproblem;

import common.Hospital;
import common.Resident;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Matching {
    private boolean stable;
    private Problem problem = null;
    private List<MatchingElement> matchingPairs = new ArrayList<>();
    private Map<Resident, Hospital> residentHospitalMap = new HashMap<>();

    public Matching(Problem problem, boolean stable) {
        this.stable = stable;
        this.problem = problem;
    }

    /**
     * A function which can be called to add a pair(Resident, Hospital) to the matching
     *
     * @param resident
     * @param hospital
     */
    public void addPair(Resident resident, Hospital hospital) {
        this.matchingPairs.add(new MatchingElement(resident, hospital));
        residentHospitalMap.put(resident, hospital);
    }

    /**
     * A function which checks if the matching is stable
     * Optional, 4th task
     *
     * @return true if the matching is stable, false otherwise
     */
    public boolean isStable() {
        for (int i = 0; i < matchingPairs.size(); ++i) {
            Hospital hospital = matchingPairs.get(i).getHospital();
            Resident resident = matchingPairs.get(i).getResident();
            List<Resident> hospitalPreferences = problem.getHospitalResidentsPreferences(hospital);
            int idx1;
            for (idx1 = hospitalPreferences.size() - 1; idx1 >= 0; --idx1) {
                if (hospitalPreferences.get(idx1).equals(resident)) {
                    break;
                }
            }
            if (idx1 == 0) {
                continue;
            }
            for (idx1 = idx1 - 1; idx1 >= 0; --idx1) {
                // hospital[i] prefers hospitalPreferences.get(idx1) over resident
                if (!residentHospitalMap.containsKey(hospitalPreferences.get(idx1))) {
                    continue;
                }
                List<Hospital> residentPreferences = problem.getResidentHospitalsPreferences(hospitalPreferences.get(idx1));
                // residentPreferences are the preferences of the current resident
                for (int j = 0; j < residentPreferences.size(); ++j) {
                    if (residentPreferences.get(j).equals(residentHospitalMap.get(hospitalPreferences.get(idx1)))) {
                        break;
                    }
                    if (residentPreferences.get(j).equals(hospital)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();

        stringBuilder.append(stable ? "Stable matching for \"" : "Matching for \"").append(problem.getName()).append("\" problem\n");
        for (int i = 0; i < matchingPairs.size(); ++i) {
            stringBuilder.append(i + 1).append(". ");
            stringBuilder.append(matchingPairs.get(i));
            if (i + 1 < matchingPairs.size()) {
                stringBuilder.append('\n');
            }
        }

        return stringBuilder.toString();
    }
}
