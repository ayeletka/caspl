package threadPerClient;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;

import protocol.ServerProtocol;
import tokenizer.Tokenizer;

public class ConnectionHandler<T> implements Runnable {

	private BufferedReader in;
	private PrintWriter out;
	Socket clientSocket;
	ServerProtocol<T> protocol;
	Tokenizer<T> tokenizer;

	public ConnectionHandler(Socket acceptedSocket, ServerProtocol<T> p, Tokenizer<T> t)
	{
		in = null;
		out = null;
		clientSocket = acceptedSocket;
		protocol = p;
		tokenizer = t;
		System.out.println("Accepted connection from client!");
		System.out.println("The client is from: " + acceptedSocket.getInetAddress() + ":" + acceptedSocket.getPort());
	}
	/** client's Connection-Handler thread.
	 * once initialized, accepting messages and sends responses
	 * until Client closes connection
	 */
	public void run()
	{
		try {
			initialize();
		}
		catch (IOException e) {
			System.out.println("Error in initializing I/O");
		}
		try {
			process();
		} 
		catch (IOException e) {
			System.out.println("Error in I/O");
		} 
		System.out.println("Connection closed - bye bye...");
		close();
	}
	/**As long as the client is connected
	 *  accepting messages from the
	 * Tokenizer and sends responses accordingly.
	 * @throws IOException
	 */
	public void process() throws IOException
	{
		T msg;
		while ((msg = tokenizer.nextMessage()) != null)
		{
			T response = protocol.processMessage(msg);
			if (response != null)
			{
				out.println(response);
			}
			if (protocol.isEnd(msg)){
				System.out.println("Is end....");
				break;
			}
		}
		System.out.println("The Client ended the session.");
	}
	/**
	 * initializes Client-Server I/O, creates a new Tokenizer
	 * for the Thread.
	 * 
	 * @throws IOException
	 */
	public void initialize() throws IOException
	{
		// Initialize I/O
		in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream(),"UTF-8"));
		out = new PrintWriter(new OutputStreamWriter(clientSocket.getOutputStream(),"UTF-8"), true);
		tokenizer.addInputStream(new InputStreamReader(clientSocket.getInputStream(),"UTF-8"));
		System.out.println("I/O initialized");
	}
	/**
	 *  Closes the connection gracefully.
	 */
	public void close()	{
		try {
			if (tokenizer.isAlive())//Handle this in tokenizer
			{
				in.close();
			}
			if (out != null)
			{
				out.close();
			}
			clientSocket.close();
		}
		catch (IOException e)
		{
			System.out.println("Exception in closing I/O");
		}
	}
}