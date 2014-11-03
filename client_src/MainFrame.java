import java.awt.Container;
import java.awt.Event;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JTextArea;

public class MainFrame extends JFrame {
	
	public MainFrame(String title) {
		super(title);

		//Set layout manager
		setLayout(new BorderLayout());

		// Create Swing component
		final JTextArea textArea = new JTextArea();
		JButton button = new JButton("Click me!");

		// Add Swing Components to content pane
		Container c = getContentPane();

		c.add(textArea, BorderLayout.CENTER);
		c.add(button, BorderLayout.SOUTH);

		// Add behavior
		button.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				textArea.append("Hello\n");
			}


		});
	}
}