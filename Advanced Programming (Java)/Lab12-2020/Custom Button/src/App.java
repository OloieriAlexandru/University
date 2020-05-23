import view.CircleButton;

import javax.swing.*;
import java.awt.*;

public class App {
    public static void main(String[] args) {
        JFrame frame = new JFrame();
        JButton button = new CircleButton("Text");

        frame.setPreferredSize(new Dimension(500, 500));
        frame.pack();
        frame.add(button);
        frame.setVisible(true);
    }
}
