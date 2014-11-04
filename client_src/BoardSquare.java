import javax.swing.JPanel;//Panel, will be used as board square
import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

public class BoardSquare extends JPanel {
		private static final long serialVersionUID = 1L;
		
		public int x, y;//position of square
		STATE state;
		public enum STATE {
			BLACK(Color.BLACK), 
			WHITE(Color.WHITE), 
			EMPTY(null);
			
			Color color;
			STATE(Color c) {
				if (c==null)
					color=new Color(0,0,0,0);//sets color to transparent
				else
					color= c;//else we have white or black
			}
		
		}
		
	    public BoardSquare(int x ,int y){
	    	this.x=x;
	    	this.y=y;
	    	this.setSize(30, 30);
	    	this.setLocation(x*30,y*30);
	    	this.setBackground(new Color(0xFFEB7D));//set tan background on square
	    	state=STATE.EMPTY;
	    }

	    protected void paintComponent(Graphics g){
	        super.paintComponent(g);
	        g.setColor(Color.BLACK);
	        g.drawLine(0, 15, 30, 15);
	        g.drawLine(15,0,15,30);
	        g.setColor(state.color);
	        g.fillOval(5, 5, 20, 20);
	    }
	}
