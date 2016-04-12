package tokenizer_whatsapp;
import tokenizer.Message;

public class WhatsAppResponseMessage extends WhatsAppMessage implements Message<String> {
	private int statusCode;
	/**
	 * WhatsAppMessage sub-class represents response
	 * message from the Server to the Client.
	 * @param httpVersion
	 * @param statusCode
	 */
	public WhatsAppResponseMessage(String httpVersion, int statusCode) {
		super(httpVersion);
		this.statusCode = statusCode;

	}

	public String toString(){
		StringBuilder builder = new StringBuilder();
		builder.append(httpVersion + " " + statusCode + "\n");
		for(String header: stringHeaders){
			builder.append(header + "\n");
		}
		builder.append(body + "\n$");
		return builder.toString();
	}
	
}
