package main;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashSet;
import java.util.Set;

import static java.lang.Character.isLetter;

public class Program {
    public static void main(String[] args){
        Date createdDate = new java.util.Date();

        compulsory();
        optional(args);

        Date now = new java.util.Date();
        double seconds = ((now.getTime() - createdDate.getTime()) / 1000.0);

        System.out.println("\n\nThe running time of the application (seconds): " + seconds);
    }

    private static void optional(String[] args){
        System.out.println("\nOptional\n");

        if (!validateArguments(args)){
            return;
        }

        String alphabet = getAlphabet(args);
        int n = Integer.parseInt(args[0]);
        int k = Integer.parseInt(args[1]);

        System.out.print("words = { ");
        String[] words = new String[n];
        for (int i=0;i<words.length;++i){
            words[i] = generateRandomWord(k, alphabet);
            System.out.print(words[i]);
            if (i + 1 != words.length){
                System.out.print(", ");
            } else {
                System.out.println(" };");
            }
            if (i > 0 && (i+1) % 10 == 0){
                System.out.println();
                if (i + 1 != words.length){
                    System.out.print('\t');
                }
            }
        }

        Graph graph = new Graph(words);
        System.out.println("Adjacency matrix of the graph:");
        System.out.println(graph.getAdjacencyMatrix());
        System.out.println();

        graph.printNodesWithMostNeighbors();
        graph.printNodesWithLeastNeighbors();
        if (graph.allNodesHaveTheSameNumberOfNeighbors()){
            System.out.println("All the nodes have the same number of neighbors!");
        } else {
            System.out.println("Not all the nodes have the same number of neighbors!");
        }

        System.out.println();

        ArrayList<ArrayList<String>> connectedComponents = graph.getConnectedComponens();
        if (connectedComponents.size() == 1){
            System.out.println("The generated graph is connected!");
        } else {
            System.out.println("The generated graph is not connected!");
        }

        System.out.println("Connected components:");
        for (int i=0;i<connectedComponents.size();++i){
            System.out.print((i+1)+". ");
            for (int j=0;j<connectedComponents.get(i).size();++j){
                System.out.print(connectedComponents.get(i).get(j)+" ");
            }
            System.out.println();
        }
    }

    private static boolean validateArguments(String[] args){
        if (args.length < 3){
            System.out.println("You have to provide at least 3 arguments! (n k letter1)");
            return false;
        }
        if (!tryParseInt(args[0]) || Integer.parseInt(args[0]) <= 0){
            System.out.println("n has to be a positive integer!");
            return false;
        }
        if (!tryParseInt(args[1]) || Integer.parseInt(args[1]) <= 0){
            System.out.println("k has to be a positive integer!");
            return false;
        }
        for (int i=2;i<args.length;++i){
            for (int j=0;j<args[i].length();++j){
                if (!isLetter(args[i].charAt(j))){
                    System.out.println("The alphabet has to contain only letters!");
                    return false;
                }
            }
        }
        System.out.println("Valid arguments!\n");
        return true;
    }

    private static String getAlphabet(String[] args){
        StringBuilder stringBuilder = new StringBuilder();
        Set<Character> chars = new HashSet<>();

        for (int i=2;i<args.length;++i){
            for (int j=0;j<args[i].length();++j){
                if (!chars.contains(args[i].charAt(j))){
                    chars.add(args[i].charAt(j));
                    stringBuilder.append(args[i].charAt(j));
                }
            }
        }

        return stringBuilder.toString();
    }

    private static String generateRandomWord(int length, String alphabet){
        StringBuilder stringBuilder = new StringBuilder();

        for (int i=1;i<=length;++i){
            stringBuilder.append(alphabet.charAt(((int)(Math.random()*1000000))%alphabet.length()));
        }

        return stringBuilder.toString();
    }

    private static boolean tryParseInt(String num){
        int start = 0;
        if (!(num.charAt(0) >= '0' && num.charAt(0) <= '9')){
            if (num.charAt(0) != '-' || (num.charAt(0) == '-' && num.length() == 1)){
                return false;
            } else {
                start = 1;
            }
        }
        for (int i=start;i<num.length();++i){
            if (!(num.charAt(i) >= '0' && num.charAt(i) <= '9')){
                return false;
            }
        }
        return true;
    }

    private static void compulsory() {
        System.out.println("\nCompulsory\n");
        String[] languages = {"C", "C++", "C#", "Python", "Go", "Rust", "JavaScript", "PHP", "Swift", "Java"};
        int n = (int) (Math.random() * 1_000_000);

        n *= 3;
        n += 0b10101;
        n += 0xFF;
        n *= 6;
        System.out.println("n = " + n);

        while (n >= 10){
            n = sumOfDigits(n);
        }

        System.out.println("Willy-nilly, this semester I will learn " + languages[n]);
    }

    private static int sumOfDigits(int x){
        if (x <= 9){
            return x;
        }
        int res = 0;
        while (x > 0){
            res += x % 10;
            x /= 10;
        }
        return res;
    }
}
