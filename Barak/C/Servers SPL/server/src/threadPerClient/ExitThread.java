package threadPerClient;

import java.util.Scanner;
import java.util.concurrent.atomic.AtomicBoolean;

import reactor.Reactor;
import tokenizer_whatsapp.WhatsAppMessage;

public class ExitThread implements Runnable{
	private AtomicBoolean shouldStop;
	private Reactor<WhatsAppMessage> reactorToStop;
	public ExitThread(AtomicBoolean shouldStop){
		this.shouldStop = shouldStop;
	}
	
	public ExitThread(AtomicBoolean shouldStop, Reactor<WhatsAppMessage> toStop){
		this.shouldStop = shouldStop;
		this.reactorToStop = toStop;
	}
	/**
	 * Receives text from the user,
	 * when receives "exit" starts shut-down proccess.
	 */
	public void run() {
		while (!shouldStop.get()){
			Scanner sc = new Scanner(System.in);
			String input = sc.nextLine();
			if(input.equals("exit")){
				System.out.println("Exit command detected...");
				shouldStop.set(true);
				if(reactorToStop!= null){
					reactorToStop.stopReactor();
				}
			}
			sc.close();
		}
	}
}
