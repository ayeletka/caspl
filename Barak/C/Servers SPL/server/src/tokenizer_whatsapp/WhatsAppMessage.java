package tokenizer_whatsapp;

import java.util.ArrayList;
import java.util.HashMap;

import tokenizer.Message;

public abstract class WhatsAppMessage implements Message<String>{
	protected String httpVersion;
	protected HashMap<String, String> headers;
	protected ArrayList<String> stringHeaders;
	protected String body;
	/**
	 * Abstract Class represents a whatsApp message.
	 * Inherited by {@link WhatsAppRequestMessage}, {@link WhatsAppResponseMessage}.
	 * @param httpVersion
	 */
	public WhatsAppMessage(String httpVersion) {
		this.httpVersion = httpVersion;
		this.body = "";
		headers = new HashMap<String, String>();
		stringHeaders = new ArrayList<String>();
	}

	public void addHeader(String headerName, String headerValue) {
		headers.put(headerName, headerValue);
		stringHeaders.add(headerName + ": " + headerValue);
	}
	
	public void addBody(String body){
		this.body += body;
	}
	
	public String getHttpVersion(){
		return this.httpVersion;
	}
	
	public HashMap<String, String> getHeaders(){
		return this.headers;
	}
	
	public String getBody(){
		return this.body;
	}
}
