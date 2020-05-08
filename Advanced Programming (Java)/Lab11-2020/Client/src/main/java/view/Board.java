package view;

import controller.Controller;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;

public class Board extends JPanel {
    public static final int BOARD_SIDE_PX = 803;
    private BufferedImage boardImage;
    private Graphics2D graphics2D;
    private int size;

    public Board(Controller controller, int size) {
        this.size = size;

        addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                controller.notifyMouseClicked(e.getX(), e.getY());
            }
        });
    }

    public void init() {
        resetImageAndGraphics();
        drawBoard();
        repaint();
    }

    @Override
    public void paint(Graphics g) {
        super.paintComponent(g);

        Graphics2D g2 = (Graphics2D) g;
        g2.drawImage(boardImage, 0, 0, this);
    }

    /**
     * Draws a piece which is positioned on board at (boardX, boardY)
     */
    public void drawPiece(int boardX, int boardY, int player) {
        int sz = (BOARD_SIDE_PX - size * 2) / size;
        int more = (BOARD_SIDE_PX - sz * size) / 2;

        int x = more + sz * boardX + sz / 2;
        int y = more + sz * boardY + sz / 2;

        Circle circle = new Circle(x, y, (sz * 2) / 3);
        if (player == 0) {
            graphics2D.setColor(Color.ORANGE);
        } else {
            graphics2D.setColor(Color.CYAN);
        }
        graphics2D.fill(circle);
        repaint();
    }

    /**
     * The initial drawing of the board
     */
    private void drawBoard() {
        int sz = (BOARD_SIDE_PX - size * 2) / size;
        int more = (BOARD_SIDE_PX - sz * size) / 2;
        graphics2D.setColor(Color.BLACK);
        for (int i = 0; i <= size; ++i) {
            graphics2D.drawLine(i * sz + more, more, i * sz + more, BOARD_SIDE_PX - more);
            graphics2D.drawLine(more, i * sz + more, BOARD_SIDE_PX - more, i * sz + more);
        }
    }

    private void resetImageAndGraphics() {
        boardImage = new BufferedImage(BOARD_SIDE_PX, BOARD_SIDE_PX, BufferedImage.TYPE_INT_ARGB);
        graphics2D = boardImage.createGraphics();

        graphics2D.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        graphics2D.setStroke(new BasicStroke((2)));
    }
}
