package game.arithmetic;

import common.Helper;

import java.util.Scanner;

public class PlayerManual extends PlayerArithmeticProgressionGame {
    public PlayerManual(ArithmeticProgressionGame game, String name) {
        super(game, name);
    }

    @Override
    protected void myTurnLogic() {
        int tokensCount = board.getTokensCount();
        if (tokensCount == 0) {
            return;
        }
        System.out.print(name + "'s turn. Manual player. Available tokens: ");
        board.printTokens();
        System.out.print("Your tokens: ");
        printMyTokens();

        Token token = board.getKthToken(readTokenPosition());

        myTokens.add(token);
        board.removeToken(token);

        verifyScore();
    }

    /**
     * Manual player - a user has to choose a token
     * This function reads that token from stdin
     */
    private int readTokenPosition() {
        Scanner scanner = new Scanner(System.in);
        int position = 0;
        while (true) {
            int token = Helper.readIntOption(scanner, "Choose a token: ");
            position = board.getIndexOfTokenValue(token);
            if (position == -1) {
                System.out.println("Wrong token!");
                continue;
            }
            break;
        }
        return position;
    }
}
