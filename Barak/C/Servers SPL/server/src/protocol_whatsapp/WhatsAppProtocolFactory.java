package protocol_whatsapp;

import database.WhatsAppDatabase;
import protocol.ServerProtocolFactory;
import tokenizer_whatsapp.WhatsAppMessage;

public class WhatsAppProtocolFactory implements ServerProtocolFactory<WhatsAppMessage>{
	private WhatsAppDatabase database;
	public WhatsAppProtocolFactory(){
		this.database = new WhatsAppDatabase();
	}	
	public WhatsAppProtocol create(){
		return new WhatsAppProtocol(database);
	}
}
