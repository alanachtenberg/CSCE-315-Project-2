package client;

import javax.swing.*;
import java.awt.*;

public class ClientGUI {
    private static void createAndShowGUI() {
        //Create and set up the window.
        JFrame frame = new JFrame("Gomaku - 5 In A Row");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(600, 600);

        //Add the ubiquitous "Hello World" label.
        JPanel board = new JPanel();
        board.setSize(400, 400);
        board.set
        frame.getContentPane().add(board);

        //Display the window.
        frame.setVisible(true);
    }

    public static void main(String[] args) {
        //Schedule a job for the event-dispatching thread:
        //creating and showing this application's GUI.
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                createAndShowGUI();
            }
        });
    }
}
