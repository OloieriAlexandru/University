package game.arithmetic;

import common.Helper;

public class PlayerRandom extends PlayerArithmeticProgressionGame {
    public PlayerRandom(ArithmeticProgressionGame game, String name) {
        super(game, name);
    }

    @Override
    protected void myTurnLogic() {
        int tokensCount = board.getTokensCount();
        if (tokensCount == 0) {
            return;
        }
        System.out.println(name + "'s turn. Random player.");
        Token myToken = board.getKthToken(Helper.generateRandomInt(0, tokensCount - 1));
        board.removeToken(myToken);
        myTokens.add(myToken);

        verifyScore();
    }
}
