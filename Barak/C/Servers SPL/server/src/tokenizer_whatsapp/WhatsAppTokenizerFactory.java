package tokenizer_whatsapp;
import tokenizer.TokenizerFactory;

public class WhatsAppTokenizerFactory implements TokenizerFactory<WhatsAppTokenizer>{

	/**
	 * WhatsApp Tokenizer factory, Creates WhatsApp Tokenizers 
	 * with a boolean flag shows whether it creates a reactor
	 * or Thread-per-client Server.
	 */
	public WhatsAppTokenizer create(boolean reactorMethod) {
		WhatsAppTokenizer tokenizer = new WhatsAppTokenizer(reactorMethod);
		return tokenizer;
	}

}
