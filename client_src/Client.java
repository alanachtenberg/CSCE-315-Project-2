
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


/**
 *
 * @author jacobstone
 */
import java.io.*;
import java.net.*;
import java.util.*;
public class Client {
	
	private BufferedReader 	in;
	private PrintWriter 	out;
	
	public Client(){
	}
	
	public Client(String server, int port){
	try{
		System.out.println("Connecting to '" + server + "' on port '" + port + "'");
		Socket socket = new Socket(server, port);
		in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		out = new PrintWriter(socket.getOutputStream(), true);	
		
	}
	catch(Exception e){
		System.out.println(e.getMessage());
		e.printStackTrace();
	}
	}
	
	public String read(){
		try {
			String resp;
			resp=in.readLine();
			return resp;
		} catch (IOException e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
		return null;
	}
	
	public void write(String msg){
		out.println(msg);
	}
    

//	public static void main(String args[]){
//
//	    try{
//	    	Client myClient= new Client("10.202.120.48",2323);
//	    	while(true){
//	    	System.out.println(myClient.read());
//	    	Scanner IP = new Scanner(System.in);
//			String userMsg = IP.nextLine();
//			myClient.write(userMsg);
//	    	}
//	    	
//        }catch (UnknownHostException err) {
//			err.printStackTrace();
//		} catch (IOException err) {
//			err.printStackTrace();
//		}
//	}
}

