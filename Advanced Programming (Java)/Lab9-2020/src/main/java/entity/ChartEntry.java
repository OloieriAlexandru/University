package entity;

import javax.persistence.*;
import java.io.Serializable;

@Entity
@Table(name = "charts_albums")
public class ChartEntry implements Serializable {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id")
    private int id;

    @Column(name = "rank", nullable = false)
    private int rank;

    @Column(name = "chart_id", nullable = false)
    private int chartId;

    @Column(name = "album_id", nullable = false)
    private int albumId;

    public ChartEntry() {
    }

    public ChartEntry(int rank, int chartId, int albumId) {
        this.rank = rank;
        this.chartId = chartId;
        this.albumId = albumId;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getRank() {
        return rank;
    }

    public void setRank(int rank) {
        this.rank = rank;
    }

    public int getChartId() {
        return chartId;
    }

    public void setChartId(int chartId) {
        this.chartId = chartId;
    }

    public int getAlbumId() {
        return albumId;
    }

    public void setAlbumId(int albumId) {
        this.albumId = albumId;
    }
}
