package tokenizer_whatsapp;
import tokenizer.Message;

public class WhatsAppRequestMessage extends WhatsAppMessage implements Message<String>   {
	private String uri;
	
	/**
	 * WhatsAppMessage sub-class represents request
	 * message from the Client to the Server.
	 * @param httpVersion
	 * @param statusCode
	 */
	public WhatsAppRequestMessage(String httpVersion, String type, String uri){ 
		super(httpVersion);
		this.uri = uri;
	}
	public String getUri(){
		return this.uri;
	}
	
	public String getCookie(){
		if(this.getHeaders().get("Cookie")!=null){
			return this.getHeaders().get("Cookie");
		}
		else
			return "";
	}

}
