package util;

import entity.Album;
import entity.Artist;

import java.util.Collections;
import java.util.List;
import java.util.Scanner;

public class Helper {
    public static boolean stringIsUInt(String str) {
        if (str.length() > 9 || str.length() == 0) {
            return false;
        }
        for (int i = 0; i < str.length(); ++i) {
            if (!(str.charAt(i) >= '0' && str.charAt(i) <= '9')) {
                return false;
            }
        }
        return true;
    }

    /**
     * A function used by shells
     * Reads a line from stdin
     */
    public static String readLine(Scanner scanner, String message) {
        String line;
        while (true) {
            if (message != null) {
                System.out.print(message);
            }
            line = scanner.nextLine();
            if (line.length() == 0) {
                continue;
            }
            break;
        }
        return line.trim();
    }

    /**
     * Prints information about an artist that was added to db
     */
    public static void printDatabaseArtistInsertEvent(Artist artist) {
        System.out.println("Artist " + artist + " inserted in the database!");
    }

    /**
     * Prints information about an album that was added to db
     */
    public static void printDatabaseAlbumInsertEvent(Album album) {
        System.out.println("Album " + album + " inserted in the database!");
    }

    /**
     * Random shuffles a permutation
     */
    public static void shufflePermutation(List<Integer> permutation) {
        Collections.shuffle(permutation);
    }
}
