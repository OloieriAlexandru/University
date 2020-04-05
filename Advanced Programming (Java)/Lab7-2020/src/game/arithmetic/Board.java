package game.arithmetic;

import java.util.ArrayList;
import java.util.List;

public class Board {
    private List<Token> tokens;

    public Board(int n) {
        tokens = new ArrayList<>();
        for (int i = 1; i <= n; ++i) {
            tokens.add(new Token(i));
        }
    }

    /**
     * Returns the number of remaining tokens
     */
    public int getTokensCount() {
        synchronized (this) {
            return tokens.size();
        }
    }

    /**
     * Returns the k'th token
     */
    public Token getKthToken(int k) {
        synchronized (this) {
            return tokens.get(k);
        }
    }

    /**
     * Removes a token from the board
     */
    public void removeToken(Token token) {
        synchronized (this) {
            int rm = -1;
            for (int i = 0; i < tokens.size(); ++i) {
                if (tokens.get(i).equals(token)) {
                    rm = i;
                    break;
                }
            }
            if (rm == -1) {
                return;
            }
            tokens.remove(rm);
        }
    }

    /**
     * Binary searches the position of the token with the value = (param)value
     */
    public int getIndexOfTokenValue(int value) {
        int left = 0;
        int right = tokens.size() - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (tokens.get(mid).getValue() == value) {
                return mid;
            }
            if (tokens.get(mid).getValue() > value) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        return -1;
    }

    /**
     * Displays the value of all the remaining tokens
     */
    public void printTokens() {
        for (int i = 0; i < tokens.size(); ++i) {
            System.out.print(tokens.get(i).getValue());
            if (i + 1 < tokens.size()) {
                System.out.print(", ");
            }
        }
        System.out.println();
    }
}
