import javax.swing.JFrame;
import javax.swing.SwingUtilities;

public class GUI {


	public static void main(String[] args) {

		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				
				JFrame menu= new MenuFrame("Five in a Row Menu");
				menu.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				menu.setVisible(true);
				
			}
		});
	}
}