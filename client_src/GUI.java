import javax.swing.JFrame;
import javax.swing.SwingUtilities;

public class GUI {


	public static void main(String[] args) {

		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				JFrame frame = new MainFrame("Five in a Row!");
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				frame.setVisible(true);
			}
		});
	}
}