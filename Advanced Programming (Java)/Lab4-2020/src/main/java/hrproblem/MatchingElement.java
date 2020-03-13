package hrproblem;

import common.Hospital;
import common.Resident;

public class MatchingElement {
    private Resident resident;
    private Hospital hospital;

    public MatchingElement(Resident resident, Hospital hospital){
        this.resident = resident;
        this.hospital = hospital;
    }

    public Hospital getHospital() {
        return hospital;
    }

    public Resident getResident() {
        return resident;
    }

    @Override
    public String toString() {
        return resident.getName() + " - " + hospital.getName();
    }
}
