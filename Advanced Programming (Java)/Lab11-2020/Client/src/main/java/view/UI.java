package view;

import controller.Controller;

import javax.swing.*;
import java.awt.*;

public class UI extends JFrame {
    private Board board;
    private InfoPanel infoPanel;

    public UI(Controller controller, int player, int size) {
        super("Gomoku");
        board = new Board(controller, size);
        board.init();
        infoPanel = new InfoPanel();
        infoPanel.init(player);

        init();
    }

    /**
     * Draws a "player"'s piece
     */
    public void drawPiece(int boardX, int boardY, int player) {
        board.drawPiece(boardX, boardY, player);
    }

    /**
     * Creates the layout of the UI
     */
    private void init() {
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        setLayout(new GridBagLayout());
        setPreferredSize(new Dimension(1280, Board.BOARD_SIDE_PX + 36));

        GridBagConstraints gridBagConstraints = new GridBagConstraints();
        gridBagConstraints.fill = GridBagConstraints.BOTH;
        gridBagConstraints.gridx = 0;
        gridBagConstraints.gridy = 0;
        gridBagConstraints.weightx = 5;
        gridBagConstraints.weighty = 1;
        add(board, gridBagConstraints);

        gridBagConstraints.gridx = 1;
        gridBagConstraints.weightx = 3;
        add(infoPanel, gridBagConstraints);

        pack();
    }
}
