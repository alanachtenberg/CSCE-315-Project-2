import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Container;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextField;


public class MenuFrame extends JFrame{
	public final JFrame menuWindow;

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private static final Insets WEST_INSETS = new Insets(5, 0, 5, 5);
	private static final Insets EAST_INSETS = new Insets(5, 5, 5, 0);
	private GridBagConstraints createGbc(int x, int y) {
	      GridBagConstraints gbc = new GridBagConstraints();
	      gbc.gridx = x;
	      gbc.gridy = y;
	      gbc.gridwidth = 1;
	      gbc.gridheight = 1;

	      gbc.anchor = (x == 0) ? GridBagConstraints.WEST : GridBagConstraints.EAST;
	      gbc.fill = (x == 0) ? GridBagConstraints.BOTH
	            : GridBagConstraints.HORIZONTAL;

	      gbc.insets = (x == 0) ? WEST_INSETS : EAST_INSETS;
	      gbc.weightx = (x == 0) ? 0.1 : 1.0;
	      gbc.weighty = 1.0;
	      return gbc;
	   }
	
	final JTextField 	serverInput;
	final JTextField 	portInput;
	final JTextField 	passInput;
	final JTextField 	aiServerInput;
	final JTextField 	aiPortInput;
	
	public MenuFrame(String title) {
		super(title);//calls constructor of parent class JFrame
		//this.setSize(300, 300);//sets size of main window
		menuWindow = this;
		this.setBounds(325,150, 300, 300);
		//Set layout manager
		GridBagLayout layout= new GridBagLayout();
		layout.rowWeights=new double[]{1,1,1,1,1,1,1};//evenly split up the rows
		layout.columnWeights= new double[]{25,75};
		setLayout(layout);
		GridBagConstraints grid = new GridBagConstraints();
		// Create Swing components
		final JLabel 		serverLabel= new JLabel("Server:");
		serverInput = new JTextField("input server adress here");
		
		final JLabel		portLabel= new JLabel("Port:");
		portInput = new JTextField("input port number here");
		
		final JLabel		passLabel= new JLabel("Password:");
		passInput = new JTextField("input password here");
		
		final JLabel 		aiServerLabel= new JLabel("AI Server:");
		aiServerInput = new JTextField("input AI server adress here");
		
		final JLabel		aiPortLabel= new JLabel("Port:");
		aiPortInput = new JTextField("input AI port number here");
		
		String[] modes= {"HUMAN-AI"};//, "AI-AI"};
		final JLabel	modeLabel = new JLabel("Mode:");
		final JComboBox modeInput = new JComboBox(modes);
		
		String[] difficulties= {"EASY", "MEDIUM", "HARD"};
		final JLabel	diffLabel = new JLabel("Difficulty:");
		final JComboBox diffInput = new JComboBox(difficulties);
		
		
		JButton button = new JButton("Submit");
		
		// Add Swing Components to content pane
		Container c = getContentPane();	
		grid=createGbc(0,0);
		c.add(serverLabel,grid);
		grid=createGbc(1,0);
		c.add(serverInput,grid);
		
		grid=createGbc(0,1);
		c.add(portLabel,grid);
		grid=createGbc(1,1);
		c.add(portInput,grid);
		
		grid=createGbc(0,2);
		c.add(passLabel,grid);
		grid=createGbc(1,2);
		c.add(passInput,grid);
		
		grid=createGbc(0,3);
		c.add(modeLabel,grid);
		grid=createGbc(1,3);
		c.add(modeInput,grid);
		
		grid=createGbc(0,4);
		c.add(diffLabel,grid);
		grid=createGbc(1,4);
		c.add(diffInput,grid);
		
		grid=createGbc(0,5);
		c.add(aiServerLabel,grid);
		grid=createGbc(1,5);
		c.add(aiServerInput,grid);
		
		grid=createGbc(0,6);
		c.add(aiPortLabel,grid);
		grid=createGbc(1,6);
		c.add(aiPortInput,grid);
	
		grid=createGbc(0,7);
		c.add(button, grid);
		
		// Add behavior
		button.addActionListener(new ActionListener(){//ActionListener is a callback object of the button
			public void actionPerformed(ActionEvent e) {//override of function actionPerformed

				menuWindow.setVisible(false);
				
				GameFrame.serverName=serverInput.getText();
				GameFrame.portNumber=portInput.getText();
				GameFrame.aiServerName=aiServerInput.getText();
				GameFrame.aiPortNumber=aiPortInput.getText();
				GameFrame.password=passInput.getText();
				GameFrame.mode=modeInput.getSelectedItem().toString();
				GameFrame.difficulty=diffInput.getSelectedItem().toString();
				String temp=GameFrame.serverName.substring(0,5);
				if(temp.equals("input"))
					System.exit(EXIT_ON_CLOSE);
					
				JFrame game = new GameFrame("Five in a Row!");
				game.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				game.setVisible(true);
			}
		});
	}
	
		



		

	};
	
