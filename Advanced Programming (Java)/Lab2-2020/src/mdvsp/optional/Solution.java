package mdvsp.optional;

import java.util.ArrayList;

public class Solution {
    private ArrayList<Tour> tours = new ArrayList<>();
    private Problem problem = null;
    private boolean complete = true;

    public Solution(Problem problem){
        this.problem = problem;
    }

    /**
     * Adds a new tour to the solution
     * @param t the newly added tour
     */
    public void addTour(Tour t){
        tours.add(t);
    }

    /**
     * Sets whether in the solution all the clients from the problem are visited
     * @param complete true if all the clients are visited, false otherwise
     */
    public void setComplete(boolean complete){
        this.complete = complete;
    }

    @Override
    public String toString() {
        return SolutionBonus.getString(complete, problem, tours);
    }
}
