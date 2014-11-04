
import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JTextArea;
//callback event
//callback object
//Border Layout
//geometry used for circle piece


public class GameFrame extends JFrame {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public static String serverName;
	public static String portNumber;
	public static String aiServerName;
	public static String aiPortNumber;
	public static String password;
	public static String mode;
	public static String difficulty;
	
	public GameFrame(String title) {
		super(title);//calls constructor of parent class JFrame
		this.setSize(800, 600);//sets size of main window

		//Set layout manager
		setLayout(new BorderLayout());
		//setLayout(new GridLayout()); //want a grid for squares
		
		// Create Swing component
		final JTextArea textArea = new JTextArea();
		JButton button = new JButton("Click me!");

		// Add Swing Components to content pane
		Container c = getContentPane();	
		
		//Create Squares for board
		BoardSquare[][] Board = new BoardSquare[15][15];//container for board
		
				for (int i=1;i<=15;++i)
					for (int j=1;j<=15;++j){
						Board[i-1][j-1]= new BoardSquare(i,j);
						Board[i-1][j-1].addMouseListener(mouseListener);
						c.add(Board[i-1][j-1]);
					}
		
		c.add(textArea, BorderLayout.CENTER);
		c.add(button, BorderLayout.SOUTH);
		
		// Add behavior
		button.addActionListener(new ActionListener(){//ActionListener is a callback object of the button
			public void actionPerformed(ActionEvent e) {//override of function actionPerformed
				textArea.append(password);
			}
		});
	}
	
	MouseListener mouseListener=new MouseListener() {
		
		@Override
		public void mouseReleased(MouseEvent arg0) {
			// TODO Auto-generated method stub
			
		}
		
		@Override
		public void mousePressed(MouseEvent arg0) {
			BoardSquare mySquare=(BoardSquare)arg0.getSource();
			mySquare.state=BoardSquare.STATE.BLACK;
	    	validate();
	    	repaint();
		}
		
		@Override
		public void mouseExited(MouseEvent arg0) {
			// TODO Auto-generated method stub
			
		}
		
		@Override
		public void mouseEntered(MouseEvent arg0) {
			// TODO Auto-generated method stub
			
		}
		
		@Override
		public void mouseClicked(MouseEvent arg0) {
			// TODO Auto-generated method stub
			
		}
	};
}
