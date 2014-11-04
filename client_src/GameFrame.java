
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.IOException;

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
	private BoardSquare[][] Board = new BoardSquare[15][15];//container for board
	private Client client;
	
	public GameFrame(String title) {
		super(title);//calls constructor of parent class JFrame
		this.setSize(510, 510);//sets size of main window

		//Set layout manager
		setLayout(new BorderLayout());
		//setLayout(new GridLayout()); //want a grid for squares
		
		// Create Swing component
		final JTextArea textArea = new JTextArea(); //background for correct printing
		textArea.setBackground(new Color(0xFFEB7D));
		textArea.disable();

		// Add Swing Components to content pane
		Container c = getContentPane();	
		
		//Create Squares for board
				for (int i=1;i<=15;++i)
					for (int j=1;j<=15;++j){
						Board[i-1][j-1]= new BoardSquare(i,j);
						Board[i-1][j-1].addMouseListener(mouseListener);
						c.add(Board[i-1][j-1]);
					}
		
		c.add(textArea, BorderLayout.CENTER);
		
		
		
		InitializeServer();
		
	}
	
	void InitializeServer(){ 
	try {
		client = new Client(serverName,Integer.parseInt(portNumber));
		
		System.out.println(client.read());//discard Password
		client.write(password);//input password to server
		System.out.println(client.read());//discard Welcome
		client.write(mode);//input mode, human-ai or ai-ai
		System.out.println(client.read());//discard OK
		client.write(difficulty);//input difficulty, easy medium or hard
		System.out.println(client.read());//discard OK
	}
	catch(Exception E){
		System.out.println(E.getMessage());
		E.printStackTrace();
	}
	}
	
	MouseListener mouseListener=new MouseListener() {
		
		@Override
		public void mouseReleased(MouseEvent arg0) {
			// TODO Auto-generated method stub
			
		}
		
		@Override
		public void mousePressed(MouseEvent arg0) {
			BoardSquare mySquare=(BoardSquare)arg0.getSource();
			
			try {
				
				client.write(String.format("%x%x", mySquare.x, mySquare.y));
				String returnedMessage = client.read();
				if (returnedMessage.substring(0,1)==" "){//game is won
					
				}
					
				System.out.println(returnedMessage);
				String[] moves = returnedMessage.split(";");
				for(String move : moves) {
					//System.out.println(move);
					String[] pos = move.split(",");
					int x = Integer.parseInt(pos[0]);
					int y = Integer.parseInt(pos[1]);
					BoardSquare.STATE color;
					if(pos[2].equals("BLACK"))
						color = BoardSquare.STATE.BLACK;
					else if(pos[2].equals("WHITE"))
						color = BoardSquare.STATE.WHITE;
					else
						color = BoardSquare.STATE.EMPTY;
					
					BoardSquare square = Board[x-1][y-1];
					square.state=color;
					square.validate();
					square.repaint();
				}
			}
			catch(Exception e) {
				System.out.println("Freaking errors, man\n");
			}
			
			
			
			
			
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
