package repo.hibernate;

import entity.ChartEntry;

public class ChartEntryRepository extends AbstractRepository<ChartEntry> {
    public ChartEntryRepository() {
        super(ChartEntry.class);
    }
}
