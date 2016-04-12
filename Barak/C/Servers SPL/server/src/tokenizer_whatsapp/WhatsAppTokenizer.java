package tokenizer_whatsapp;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import tokenizer.Tokenizer;
public class WhatsAppTokenizer implements Tokenizer<WhatsAppMessage>{
	private boolean reactorMethod;
	private InputStreamReader inputStreamReader;
	private String messageAccumulator;
	private boolean hasMessage;
	private ArrayList<String> messages;
	public WhatsAppTokenizer(boolean reactorMethod){
		messageAccumulator = "";
		this.reactorMethod = reactorMethod;
		hasMessage = false;
		messages = new ArrayList<String>();
	}
	/**
	 * Gets the message input stream from the Connection Handler,
	 * reads a message and turns it into String.
	 * @return accepted message as String.
	 */
	private String parseMessage(){
		int c;
		StringBuilder builder = new StringBuilder();
		try {
			while((c = inputStreamReader.read())!=-1){
				if(c == '$'){
					return builder.toString();
				}
				else{
					builder.append(Character.toChars(c));
				}
			}
		} catch (IOException e1) {e1.printStackTrace();	}
		return null;
	}
	/**
	 * returns a message from the list of parsed messages.
	 * @return
	 */
	private String parseStringMessage(){
		String tmpMessage = messages.remove(0);
		if (messages.isEmpty()){
			hasMessage = false;
		}
		return tmpMessage;
	}
	/**
	 * Parses messages for both Thread-per-client server and reactor.
	 * separates message into lines and creates Request-Message Object,
	 *@return {@link WhatsAppRequestMessage}
	 */
	public WhatsAppMessage nextMessage() {
		WhatsAppMessage message;
		String stringMessage;
		if(reactorMethod){
			stringMessage = parseStringMessage();
		}
		else{
			stringMessage = parseMessage();
		}
		if(stringMessage == null){
			return null;
		}
		//Cut the message to lines:
		String[] messageLines = stringMessage.split("\n");
		String messagePropertiesLine = messageLines[0];

		//Get properties from the first line:
		String[] messageProperties = messagePropertiesLine.split("\\s+");
		String type = messageProperties[0];
		String uri = messageProperties[1].substring(1);
		String httpVersion = messageProperties[2];

		message = new WhatsAppRequestMessage(httpVersion, type, uri);
		//Add headers:
		int bodyLineNumber = 0;
		for(int i = 1; i<messageLines.length; i++){
			int semicolumnLocation = messageLines[i].indexOf(':');
			if(semicolumnLocation>0){
				String headerName = messageLines[i].substring(0, semicolumnLocation).trim();
				String headerValue = messageLines[i].substring(semicolumnLocation + 1).trim();
				message.addHeader(headerName, headerValue);
			}
			bodyLineNumber = i;
		}
		String body = "";
		if(messageLines.length > bodyLineNumber){
			body = messageLines[bodyLineNumber];
		}
		message.addBody(body);
		return message;
	}

	public boolean isAlive() {
		//TODO Check what the ??
		return false;
	}


	public void addInputStream(InputStreamReader inputStreamReader) {
		this.inputStreamReader = inputStreamReader;
	}

	public boolean hasMessage(){
		return hasMessage;
	}
	/**
	 * streams bytes to the response message buffer.
	 * @param responseMessage
	 * @return ByteBuffer containing bytes from response message.
	 */
	public ByteBuffer getBytesForMessage(WhatsAppResponseMessage responseMessage){
		String response = responseMessage.toString();
		byte[] tmpByteArray = null;
		try {
			tmpByteArray = response.getBytes("UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		ByteBuffer buffer = ByteBuffer.wrap(tmpByteArray);
		return buffer;

	}
	/**
	 * Gets a byteBuffer from the reactor, concatenates given bytes into
	 * messageAccumalator until reaches a delimiter.
	 * @param ByteBuffer
	 */
	public void addBytes(ByteBuffer b){
		byte[] tmpArray = b.array();
		String input = "";
		try {
			input = new String(tmpArray,"UTF-8");
		} 
		catch (UnsupportedEncodingException e) {e.printStackTrace();}
		while (input.contains("$")){
			String tmpMessage = messageAccumulator + input.substring(0,input.indexOf('$')- 1);
			messages.add(tmpMessage);
			hasMessage = true;	
			input = input.substring(input.indexOf('$') + 1);
			messageAccumulator = "";
		}
		messageAccumulator += input;
	}
}
