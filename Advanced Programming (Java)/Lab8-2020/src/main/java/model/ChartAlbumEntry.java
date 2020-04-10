package model;

public class ChartAlbumEntry {
    private int rank;
    private int chartId;
    private int albumId;

    public ChartAlbumEntry(int rank, int chartId, int albumId) {
        this.chartId = chartId;
        this.albumId = albumId;
    }

    public int getChartId() {
        return chartId;
    }

    public int getAlbumId() {
        return albumId;
    }
}
