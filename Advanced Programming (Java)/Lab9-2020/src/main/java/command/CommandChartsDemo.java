package command;

import com.github.javafaker.Faker;
import entity.Album;
import entity.Chart;
import entity.ChartEntry;
import repo.hibernate.AlbumRepository;
import repo.hibernate.ChartEntryRepository;
import repo.hibernate.ChartRepository;
import util.Helper;

import java.util.ArrayList;
import java.util.List;

public class CommandChartsDemo {
    /**
     * Tests the functionality of "ChartRepository"
     */
    public void execute() {
        AlbumRepository albumRepository = new AlbumRepository();
        ChartRepository chartRepository = new ChartRepository();
        ChartEntryRepository chartEntryRepository = new ChartEntryRepository();

        List<Album> albums = albumRepository.findAll();
        List<Integer> permutation = generatePermutation(albums.size());

        Faker faker = new Faker();

        int numberOfCharts = 5;
        for (int i = 1; i <= numberOfCharts; ++i) {
            generateChart(albums, permutation, chartRepository, chartEntryRepository, faker);
        }
        System.out.println("Generated " + numberOfCharts + " charts!");
    }

    /**
     * Generates a random chart
     */
    private void generateChart(List<Album> albums, List<Integer> permutation, ChartRepository chartRepository, ChartEntryRepository chartEntryRepository, Faker faker) {
        Helper.shufflePermutation(permutation);

        int minNumberOfAlbumsInChart = 5;
        int maxNumberOfAlbumsInChart = 10;

        int numberOfAlbumsInThisChart = faker.random().nextInt(minNumberOfAlbumsInChart, maxNumberOfAlbumsInChart);
        Helper.shufflePermutation(permutation);

        List<Album> thisChartAlbums = new ArrayList<>();
        for (int j = 0; j < numberOfAlbumsInThisChart; ++j) {
            thisChartAlbums.add(albums.get(permutation.get(j)));
        }

        Chart chart = chartRepository.create(new Chart(faker.company().name()));

        for (int i = 0; i < thisChartAlbums.size(); ++i) {
            chartEntryRepository.create(new ChartEntry(i + 1, chart.getId(), thisChartAlbums.get(i).getId()));
        }
    }

    /**
     * Generates the identity permutation
     */
    private List<Integer> generatePermutation(int numberOfElements) {
        List<Integer> permutation = new ArrayList<>();

        for (int i = 0; i < numberOfElements; ++i) {
            permutation.add(i);
        }

        return permutation;
    }
}
