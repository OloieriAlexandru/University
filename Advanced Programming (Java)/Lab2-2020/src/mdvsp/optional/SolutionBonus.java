package mdvsp.optional;

import java.util.ArrayList;

public class SolutionBonus {
    private ArrayList<Tour> tours = new ArrayList<>();
    private Problem problem = null;
    private boolean complete = true;
    private int totalCost = 0;

    public SolutionBonus(Problem problem){
        this.problem = problem;
    }

    /**
     * Adds a new tour to the solution
     * @param t the newly added tour
     * @param cost the cost of the tour
     */
    public void addTour(Tour t, int cost){
        tours.add(t);
        totalCost += cost;
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
        return getString(complete, problem, tours) + "Total cost: " + totalCost + "\n";
    }

    public static String getString(boolean complete, Problem problem, ArrayList<Tour> tours) {
        StringBuilder stringBuilder = new StringBuilder();

        stringBuilder.append(complete ? "Complete solution\n" : "Incomplete solution\n");
        stringBuilder.append("Solution to: ").append(problem.getName()).append('\n');
        stringBuilder.append("Tours count: ").append(tours.size()).append('\n');

        for (int i = 0; i< tours.size(); ++i){
            stringBuilder.append("Tour ").append(i+1).append(":\n");
            stringBuilder.append(tours.get(i)).append('\n');
        }

        return stringBuilder.toString();
    }
}
