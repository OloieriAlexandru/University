package game.arithmetic;

import game.Player;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public abstract class PlayerArithmeticProgressionGame extends Player {
    protected ArithmeticProgressionGame game;
    protected List<Token> myTokens;
    protected Board board;

    public PlayerArithmeticProgressionGame(ArithmeticProgressionGame game, String name) {
        super(name);
        this.game = game;
        this.board = game.getBoard();
        this.myTokens = new ArrayList<>();
    }

    /**
     * Prints to stdout the best arithmetic progression of this player
     */
    public void printArithmeticProgression() {
        ArithmeticProgression arithmeticProgression = getBestArithmeticProgression();
        int bestScore = arithmeticProgression.getElementsCount();
        int bestRatio = arithmeticProgression.getRatio();
        int start = arithmeticProgression.getStart();
        for (int i = 1; i <= bestScore; ++i) {
            System.out.print(start);
            start += bestRatio;
            if (i + 1 <= bestScore) {
                System.out.print(", ");
            }
        }
        System.out.println();
    }

    /**
     * Prints to stdout the tokens of this player
     */
    public void printMyTokens() {
        for (int i = 0; i < myTokens.size(); ++i) {
            System.out.print(myTokens.get(i).getValue());
            if (i + 1 < myTokens.size()) {
                System.out.print(", ");
            }
        }
        System.out.println();
    }

    /**
     * Checks if this player has won the game
     */
    protected void verifyScore() {
        int myScore = getScore();
        if (myScore == game.getK()) {
            game.announceWinner(this);
        }
    }

    @Override
    protected boolean checkIfTheGameIsRunning() {
        return game.isRunning();
    }

    /**
     * Returns the score of this player
     */
    @Override
    public int getScore() {
        return getBestArithmeticProgression().getElementsCount();
    }

    /**
     * Computes the best arithmetic progression of this player
     */
    private ArithmeticProgression getBestArithmeticProgression() {
        int tokensCount = myTokens.size();
        if (tokensCount == 0) {
            return new ArithmeticProgression(0, 0, 0);
        } else if (tokensCount == 1) {
            return new ArithmeticProgression(1, myTokens.get(0).getValue(), 0);
        }
        Collections.sort(myTokens);
        if (tokensCount == 2) {
            return new ArithmeticProgression(2, myTokens.get(0).getValue(), myTokens.get(1).getValue() - myTokens.get(0).getValue());
        }
        int bestScore = 2, start = 0, bestRatio = 1;
        for (int i = 0; i < tokensCount - 1; ++i) {
            for (int j = i + 1; j < tokensCount; ++j) {
                int ratio = myTokens.get(j).getValue() - myTokens.get(i).getValue();
                int next = myTokens.get(j).getValue() + ratio;
                int currentScore = 2;
                for (int k = j + 1; k < tokensCount; ++k) {
                    if (myTokens.get(k).getValue() == next) {
                        ++currentScore;
                        next += ratio;
                    }
                }
                if (currentScore > bestScore) {
                    bestScore = currentScore;
                    start = myTokens.get(i).getValue();
                    bestRatio = ratio;
                }
            }
        }
        return new ArithmeticProgression(bestScore, start, bestRatio);
    }
}
