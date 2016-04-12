package reactor;

import java.util.concurrent.ExecutorService;
import java.nio.channels.Selector;
import protocol.*;
import protocol_whatsapp.WhatsAppProtocolFactory;
import tokenizer_whatsapp.WhatsAppTokenizerFactory;

/**
 * a simple data structure that hold information about the reactor, including getter methods
 */
public class ReactorData<T> {

    private final ExecutorService _executor;
    private final Selector _selector;
    private final WhatsAppProtocolFactory _protocolMaker;
    private final WhatsAppTokenizerFactory _tokenizerMaker;
    
    public ExecutorService getExecutor() {
        return _executor;
    }

    public Selector getSelector() {
        return _selector;
    }
    /**
     * class contains all of the reactor's inner Objects.
     * @param _executor
     * @param _selector
     * @param protocol
     * @param tokenizer
     */
	public ReactorData(ExecutorService _executor, Selector _selector, WhatsAppProtocolFactory protocol, WhatsAppTokenizerFactory tokenizer) {
		this._executor = _executor;
		this._selector = _selector;
		this._protocolMaker = protocol;
		this._tokenizerMaker = tokenizer;
	}

	@SuppressWarnings("rawtypes")
	public ServerProtocolFactory getProtocolMaker() {
		return _protocolMaker;
	}

	public WhatsAppTokenizerFactory getTokenizerMaker() {
		return _tokenizerMaker;
	}

}
