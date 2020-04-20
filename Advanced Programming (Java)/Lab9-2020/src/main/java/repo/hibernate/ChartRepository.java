package repo.hibernate;

import entity.Chart;

public class ChartRepository extends AbstractRepository<Chart> {
    public ChartRepository() {
        super(Chart.class);
    }
}
