package threadPerClient;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.SocketTimeoutException;
import java.util.concurrent.atomic.AtomicBoolean;

import protocol.ServerProtocolFactory;
import protocol_whatsapp.WhatsAppProtocolFactory;
import tokenizer.TokenizerFactory;
import tokenizer_whatsapp.WhatsAppTokenizerFactory;
import database.WhatsAppDatabase;

public class MultipleClientProtocolServer<T> implements Runnable {
	private final int SERVER_ACCEPT_TIMOUT = 4000;
	private ServerSocket serverSocket;
	private int listenPort;
	private ServerProtocolFactory<T> _protocolFactory;
	private TokenizerFactory<T> _tokenizerFactory;
	private AtomicBoolean shouldStop;
	public WhatsAppDatabase database;
	public MultipleClientProtocolServer(int port, ServerProtocolFactory<T> whatsAppProtocolFactory,
			TokenizerFactory<T> whatsAppTokenizerFactory) {
		serverSocket = null;
		listenPort = port;
		_protocolFactory = whatsAppProtocolFactory;
		_tokenizerFactory =whatsAppTokenizerFactory;	
		database = new WhatsAppDatabase();
		this.shouldStop = new AtomicBoolean(false);
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	/**
	 * server's thread run. 
	 * opens a server socket on the given port and listens on it.
	 * runs until received "exit" from the user on the Exit-Thread .
	 */
	public void run()
	{
		try {
			serverSocket = new ServerSocket(listenPort);
			System.out.println("-Thread Per Client Server is Listening...-");
		}
		catch (IOException e) {
			System.out.println("Cannot listen on port " + listenPort);
		}
		Thread stopper = new Thread(new ExitThread(this.shouldStop));
		stopper.start();
		while (!shouldStop.get()){
			try {
				ConnectionHandler newConnection = null;
				serverSocket.setSoTimeout(SERVER_ACCEPT_TIMOUT);
				newConnection = new ConnectionHandler(serverSocket.accept(), _protocolFactory.create(),_tokenizerFactory.create(false));
				new Thread(newConnection).start();  
				  
			}
			catch (IOException e)
			{
				if (!(e instanceof SocketTimeoutException))
					System.out.println("Failed to accept on port " + listenPort);
			}
		}
		System.out.println("exiting...");
	}
	// Closes the connection
	public void close() throws IOException
	{
		serverSocket.close();
	}
/**
 * Main function for the Thread-per-client.
 * creates the server thread.
 * 
 * @param args args[0] - The server's port.
 * @throws IOException
 */
	
	
	public static void main(String[] args) throws IOException
	{
		// Get port
		int port = Integer.decode(args[0]).intValue();

		@SuppressWarnings({ "rawtypes", "unchecked" })
		MultipleClientProtocolServer server = new MultipleClientProtocolServer(port, new WhatsAppProtocolFactory(), new WhatsAppTokenizerFactory());
		Thread serverThread = new Thread(server);
		serverThread.start();
		try {
			serverThread.join();
		}
		catch (InterruptedException e)
		{
			System.out.println("Server stopped");
		}
		System.out.println("Server Stopped Succsessfuly.");
	}
}
