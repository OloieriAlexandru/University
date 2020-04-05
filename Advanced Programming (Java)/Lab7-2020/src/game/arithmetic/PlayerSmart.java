package game.arithmetic;

import common.Helper;

import java.util.Collections;

public class PlayerSmart extends PlayerArithmeticProgressionGame {
    public PlayerSmart(ArithmeticProgressionGame game, String name) {
        super(game, name);
    }

    @Override
    protected void myTurnLogic() {
        int tokensCount = board.getTokensCount();
        if (tokensCount == 0) {
            return;
        }
        System.out.println(name + "'s turn. Smart player.");
        int tokenPosition = getBestTokenPosition();
        if (tokenPosition == -1) {
            tokenPosition = Helper.generateRandomInt(0, board.getTokensCount());
        }
        Token token = board.getKthToken(tokenPosition);
        myTokens.add(token);
        board.removeToken(token);

        verifyScore();
    }

    /**
     * Smart player - it tries to extend its largest arithmetic progression
     */
    private int getBestTokenPosition() {
        int maxPossibleScore = 0, tokenPosition = -1;
        Collections.sort(myTokens);
        for (int i = 0; i < myTokens.size() - 1; ++i) {
            for (int j = i + 1; j < myTokens.size(); ++j) {
                int ratio = myTokens.get(j).getValue() - myTokens.get(i).getValue();
                int next = myTokens.get(j).getValue() + ratio;
                int currentScore = 2;
                for (int k = j + 1; k < myTokens.size(); ++k) {
                    if (myTokens.get(k).getValue() == next) {
                        ++currentScore;
                        next += ratio;
                    }
                }
                int nextPosition = board.getIndexOfTokenValue(next);
                if (nextPosition == -1) {
                    if (currentScore > maxPossibleScore) {
                        maxPossibleScore = currentScore;
                        tokenPosition = -1;
                    }
                } else {
                    ++currentScore;
                    if (currentScore > maxPossibleScore) {
                        maxPossibleScore = currentScore;
                        tokenPosition = nextPosition;
                    }
                }
            }
        }
        return tokenPosition;
    }
}
