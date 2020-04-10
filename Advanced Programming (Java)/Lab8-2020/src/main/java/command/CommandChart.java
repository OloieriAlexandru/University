package command;

import database.connection.Database;
import database.dao.*;
import javafx.util.Pair;
import model.*;

import org.apache.velocity.Template;
import org.apache.velocity.VelocityContext;
import org.apache.velocity.app.VelocityEngine;
import org.apache.velocity.runtime.log.NullLogChute;

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.*;

/**
 * Optional, 4th and 5th tasks
 */
public class CommandChart {
    /**
     * There starts the execution of the HTML Report creation
     */
    public void executeHTML() {
        StringWriter writer = getHTMLReportWriter();

        String fileName = writeHTMLReportToFile(writer);
        if (fileName != null) {
            System.out.println("Report created! The file name is: " + fileName);
        } else {
            System.out.println("Failed to create report!");
        }
    }

    /**
     * Returns a StringWriter which can write the report to a stream
     */
    private StringWriter getHTMLReportWriter() {
        VelocityEngine ve = new VelocityEngine();
        ve.setProperty("runtime.log.logsystem.class", NullLogChute.class.getName());
        ve.init();
        Template t = ve.getTemplate("./templates/report.vm");

        VelocityContext context = getVelocityContext();

        StringWriter writer = new StringWriter();
        t.merge(context, writer);
        return writer;
    }

    /**
     * Returns a Velocity specific object
     */
    private VelocityContext getVelocityContext() {
        VelocityContext context = new VelocityContext();
        List<Pair<Double, Artist>> artistsRank = getRanking();
        List<Pair<String, Artist>> artistsRankStr = new ArrayList<>();

        for (Pair<Double, Artist> artistPair : artistsRank) {
            artistsRankStr.add(new Pair<>(String.format("%.3f", artistPair.getKey()), artistPair.getValue()));
        }

        context.put("artists", artistsRankStr);

        return context;
    }

    /**
     * Creates the HTML report file
     */
    private String writeHTMLReportToFile(StringWriter writer) {
        String fileName = "report.html";

        try {
            FileWriter fileWriter = new FileWriter(fileName);
            PrintWriter printWriter = new PrintWriter(fileWriter);

            printWriter.println(writer.toString());

            printWriter.close();
        } catch (IOException e) {
            System.out.println(e.getMessage());
            return null;
        }

        return fileName;
    }

    /**
     * There starts the execution of the Chart stdout printing
     */
    public void execute() {
        List<Pair<Double, Artist>> artistsRank = getRanking();

        System.out.println("Artists ranking:");
        for (int i = 0; i < artistsRank.size(); ++i) {
            System.out.print((i + 1) + ". " + artistsRank.get(i).getValue().getName() + ", points: ");
            System.out.format("%.3f\n", artistsRank.get(i).getKey());
        }
    }

    /**
     * Returns the ranking of the artists
     */
    private List<Pair<Double, Artist>> getRanking() {
        Database database = Database.getInstance();

        ArtistController artistController = new ArtistController(database);
        AlbumController albumController = new AlbumController(database);
        ChartController chartController = new ChartController(database);

        List<Artist> artists = artistController.findAll();
        Map<Integer, Artist> artistMap = new HashMap<>();
        Map<Artist, Double> artistsPoints = new HashMap<>();
        for (Artist artist : artists) {
            artistMap.put(artist.getId(), artist);
            artistsPoints.put(artist, 0.0);
        }

        List<Album> albums = albumController.findAll();
        Map<Integer, Album> albumMap = new HashMap<>();
        for (Album album : albums) {
            albumMap.put(album.getId(), album);
        }

        List<Chart> charts = chartController.findAll(albumMap);

        return computeRanking(charts, artistMap, artistsPoints);
    }

    /**
     * Computes the score of each artist and returns an array containing the artists in descending order of their score
     */
    private List<Pair<Double, Artist>> computeRanking(List<Chart> charts, Map<Integer, Artist> artistMap, Map<Artist, Double> artistsPoints) {
        for (Chart chart : charts) {
            List<Album> chartAlbums = chart.getAlbums();

            for (int i = 0; i < chartAlbums.size(); ++i) {
                double points = computePoints(i);
                Artist artist = artistMap.get(chartAlbums.get(i).getArtistId());

                artistsPoints.put(artist, artistsPoints.get(artist) + points);
            }
        }

        List<Pair<Double, Artist>> artistsRank = new ArrayList<>();
        for (Map.Entry<Artist, Double> entry : artistsPoints.entrySet()) {
            artistsRank.add(new Pair<>(entry.getValue(), entry.getKey()));
        }

        artistsRank.sort((a1, a2) -> -Double.compare(a1.getKey(), a2.getKey()));
        return artistsRank;
    }

    /**
     * Computes the number of points an artist will get for being in a specific position in a chart
     */
    private double computePoints(int position) {
        if (position < 3) {
            return 3.0 - position;
        }
        int sub = (position - 2) / 10 + 1;
        if (sub > 8) {
            sub = 8;
        }
        return 1.0 - sub * 0.1;
    }
}
