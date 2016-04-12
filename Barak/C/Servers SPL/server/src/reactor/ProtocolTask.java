package reactor;

import java.nio.ByteBuffer;
import protocol.*;
import tokenizer_whatsapp.WhatsAppMessage;
import tokenizer_whatsapp.WhatsAppResponseMessage;
import tokenizer_whatsapp.WhatsAppTokenizer;

/**
 * This class supplies some data to the protocol, which then processes the data,
 * possibly returning a reply. This class is implemented as an executor task.
 * 
 */
public class ProtocolTask<T> implements Runnable {

	private final ServerProtocol<T> _protocol;
	private final WhatsAppTokenizer _tokenizer;
	private final ConnectionHandler<T> _handler;

	public ProtocolTask(final ServerProtocol<T> protocol, final WhatsAppTokenizer tokenizer, final ConnectionHandler<T> h) {
		this._protocol = protocol;
		this._tokenizer = tokenizer;
		this._handler = h;
	}
	/**
	 * reactor Thread.
	 * as long as there are complete messages, gets them and
	 * sends back {@link WhatsAppResponseMessage}.  
	 * 
	 * we synchronize on ourselves, in case we are executed by several threads
	 * from the thread pool. 
	 */

	public synchronized void run() {
      // go over all complete messages and process them.
      while (_tokenizer.hasMessage()) {
         WhatsAppMessage msg = _tokenizer.nextMessage();
         @SuppressWarnings("unchecked")
		T response = this._protocol.processMessage((T) msg);
         if (response != null) {
            ByteBuffer bytes = _tokenizer.getBytesForMessage((WhatsAppResponseMessage) response);
               this._handler.addOutData(bytes);
         }
      }
	}

	public void addBytes(ByteBuffer b) {
		
		_tokenizer.addBytes(b);
	}
}
