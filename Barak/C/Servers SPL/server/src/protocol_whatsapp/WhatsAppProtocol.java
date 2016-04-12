package protocol_whatsapp;

import java.util.UUID;

import protocol.AsyncServerProtocol;
import protocol.ServerProtocol;
import tokenizer_whatsapp.WhatsAppMessage;
import tokenizer_whatsapp.WhatsAppRequestMessage;
import tokenizer_whatsapp.WhatsAppResponseMessage;
import database.Group;
import database.User;
import database.WhatsAppDatabase;

public class WhatsAppProtocol implements ServerProtocol<WhatsAppMessage>, AsyncServerProtocol<WhatsAppMessage>{
	private WhatsAppDatabase database;

	public WhatsAppProtocol(WhatsAppDatabase database){
		this.database = database;
	}
	/**
	 * Accepts a WhatsAppMessage Object. Checks its correctness,
	 *  acts according to the Uri and server state
	 * and then generates a proper response message.
	 *@return {@link WhatsAppResponseMessage}.
	 */ 
	public WhatsAppMessage processMessage(WhatsAppMessage msg) {
		String uri = ((WhatsAppRequestMessage)msg).getUri();
		//Checks Uri:
		if(!isLegalUri(msg)){
			WhatsAppResponseMessage responseError = new  WhatsAppResponseMessage(msg.getHttpVersion(), 404);
			responseError.addBody("Not Found");
			return responseError;
		}
		// Checks if the User is logged in:
		if (!checkAuthenication(msg) && (!uri.equals("login.jsp"))){
			WhatsAppResponseMessage responseError = new  WhatsAppResponseMessage(msg.getHttpVersion(), 403);
			responseError.addBody("Forbidden");
			return responseError;
		}
		switch (uri) {
		case "login.jsp":
			return processLogin(msg);
		case "logout.jsp":
			return processLogout(msg);
		case "list.jsp":
			return processList(msg);
		case "create_group.jsp":
			return processCreateGroup(msg);
		case "send.jsp":
			return processSend(msg);
		case "add_user.jsp":
			return processAddUser(msg);
		case "remove_user.jsp":
			return processRemoveUser(msg);
		case "queue.jsp":
			return processQueue(msg);
		default:
			WhatsAppResponseMessage responseError = new  WhatsAppResponseMessage(msg.getHttpVersion(), 405);
			responseError.addBody("Method Not Allowed");
			return responseError;
		}
	}
	/**
	 * sends all the user's pending messages.
	 * @param msg - incoming {@link WhatsAppRequestMessage}.
	 * @return - outgoing {@link WhatsAppResponseMessage}.
	 */
	private WhatsAppMessage processQueue(WhatsAppMessage msg) {
		String messages = database.getMessagesByCookie(msg.getHeaders().get("Cookie"));
		WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 200);
		response.addBody(messages);
		return response;
	}
	/**
	 * removes the User from the database(if exists),
	 * otherwise returns a proper error message.
	 * @param msg - incoming {@link WhatsAppRequestMessage}.
	 * @return - outgoing {@link WhatsAppResponseMessage}.
	 */
	private WhatsAppMessage processRemoveUser(WhatsAppMessage msg) {
		String groupName = msg.getHeaders().get("Target");
		String phoneNumber = msg.getHeaders().get("User");
		if (database.containsGroup(groupName)){
			if (database.getGroupByName(groupName).containsUserByPhone(phoneNumber)){
				database.removeUserFromGroup(groupName, phoneNumber);
				WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 200);
				String user = database.getUserByPhoneNumber(phoneNumber).getUserName();
				response.addBody("User [" +  user + "] has been removed from group [" + groupName + "]");
				return response;
			}
			else{
				WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 769);
				response.addBody("Target does not exist");
				return response;
			}
		}
		else{
			WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 769);
			response.addBody("Target does not exist");
			return response;
		}
	}
	/**
	 * adds the user to the desired group in the database (if exists).
	 * @param msg - incoming {@link WhatsAppRequestMessage}.
	 * @return - outgoing {@link WhatsAppResponseMessage}.
	 */
	private WhatsAppMessage processAddUser(WhatsAppMessage msg) {
		String groupName = msg.getHeaders().get("Target");
		String phoneNumber = msg.getHeaders().get("User");
		if ( groupName == null || phoneNumber == null || groupName.equals("") || phoneNumber.equals("") ){
			WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 770);
			response.addBody("Target does not Exist");
			return response;
		}
		if(database.containsPhoneNumber(phoneNumber)){
			if(!database.containsGroup(groupName)){
				User creator = database.getUserByCookie(msg.getHeaders().get("Cookie"));
				database.addGroup(new Group(creator, groupName));
			}
			if (!database.containsUserInGroup(groupName, phoneNumber)){
				database.addUserToGroup(groupName, phoneNumber);
				WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 200);
				String userName = database.getUserByPhoneNumber(phoneNumber).getUserName();
				response.addBody("User [" +  userName + "]@[" + phoneNumber + "] has been added to group [" + groupName + "]");
				return response;
			}
			else{
				WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 142);
				response.addBody("Cannot add user, user already in group");
				return response;
			}
		}
		else{
			WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 242);
			response.addBody("Cannot add user, missing parameters");
			return response;
		}

	}
	/**
	 *send a message to a target User or a target group.
	 * @param msg - incoming {@link WhatsAppRequestMessage}.
	 * @return - outgoing {@link WhatsAppResponseMessage}.
	 */
	private WhatsAppMessage processSend(WhatsAppMessage msg) {
		String messageType = msg.getHeaders().get("Type");
		//Message to single user:
		if(messageType.equals("Direct")){
			String phoneNumber = msg.getHeaders().get("Target");
			String message = msg.getHeaders().get("Content");
			if(database.containsPhoneNumber(phoneNumber)){
				database.addMessageToUser(phoneNumber, message);
				System.out.println("Sending message succesfuly.");
				WhatsAppResponseMessage response = new  WhatsAppResponseMessage(msg.getHttpVersion(), 200);
				response.addBody("Message sent successfuly");
				return response;
			}
			else{
				WhatsAppResponseMessage respond = new WhatsAppResponseMessage(msg.getHttpVersion(), 771);
				System.out.println("Phone number does not exist.");
				respond.addBody("Target does not exist.");
				return respond;
			}
		}
		else if(messageType.equals("Group")){
			String groupName = msg.getHeaders().get("Target");
			String message = msg.getHeaders().get("Content");
			if(database.containsGroup(groupName)){
				database.addMessageToGroup(groupName, message);
				System.out.println("Sending message succesfuly.");
				WhatsAppResponseMessage response = new  WhatsAppResponseMessage(msg.getHttpVersion(), 200);
				response.addBody("Message sent successfuly");
				return response;
			}
			else{
				WhatsAppResponseMessage respond = new WhatsAppResponseMessage(msg.getHttpVersion(), 771);
				System.out.println("Target does not exist.");
				respond.addBody("Target does not exist.");
				return respond;
			}
		}
		return null;
	}
	/**
	 *creates a group with a given name (if not used already in the system).
	 *adds given users to the group (if such exist).
	 * @param msg - incoming {@link WhatsAppRequestMessage}.
	 * @return - outgoing {@link WhatsAppResponseMessage}.
	 */
	private WhatsAppMessage processCreateGroup(WhatsAppMessage msg) {
		User creator = this.database.getUserByCookie(msg.getHeaders().get("Cookie"));
		String groupName = msg.getHeaders().get("GroupName").trim();


		if(database.containsGroup(groupName)){
			WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 511);
			response.addBody("Group name already taken");
			return response;
		}
		String usersString = msg.getHeaders().get("Users").trim();
		String[] usersToAdd = usersString.split(",");
		for(int i = 0; i< usersToAdd.length; i++){
			String userName = usersToAdd[i];
			if(!database.containsUsername(userName)){
				WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 929);
				response.addBody("Cannot create group - unknown user -"+ userName );
				return response;
			}
			if(creator.getUserName().equals(userName)){
				WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 919);
				response.addBody("Cannot create group - don't add yourself twice");
				return response;
			}
			for(int j = i+1; j<usersToAdd.length; j++){
				if(userName.equals(usersToAdd[j])){
					WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 909);
					response.addBody("Cannot create group - Duplicate user detected -" + userName + "-");
					return response;
				}
			}
		}
		Group newGroup = new Group(creator, groupName);
		for(int i = 0; i< usersToAdd.length; i++){
			String userName = usersToAdd[i];
			User tmpUser = database.getUserByUserName(userName);
			newGroup.addUserToGroup(tmpUser);

		}
		database.addGroup(newGroup);
		WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 200);
		response.addBody("Group created successfuly\n" + newGroup.listUsers());
		return response;
	}
	/**
	 *generates and sends a List of either system's users/groups or
	 *a list of a particular group.
	 * @param msg - incoming {@link WhatsAppRequestMessage}.
	 * @return - outgoing {@link WhatsAppResponseMessage}.
	 */
	private WhatsAppMessage processList(WhatsAppMessage msg) {  
		String type = msg.getHeaders().get("List");
		if (type.equals("Users")){
			String usersList = database.generateUsersList();
			WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 200);
			response.addBody("List of users:\n" + usersList);
			return response;
		}
		else if (type.equals("Groups")){
			String groupsList = database.generateGroupsList();
			WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 200);
			response.addBody("List of Groups:\n" + groupsList);
			return response;
		}
		else if(type.equals("Group")){
			String groupName = msg.getHeaders().get("Group_Name").trim();
			if(database.containsGroup(groupName)){
				String usersInGroupList = database.generateUsersInGroupList(groupName);
				WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 200);
				response.addBody(usersInGroupList);
				return response;
			}
			else{
				WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 275);
				response.addBody("Group does not exist");
				return response;
			}
		}
		else {
			WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 273);
			response.addBody("Missing Parameters");
			return response;
		}

	}
	/**
	 *removes user's cookie from database and all the appearances of the user.
	 * @param msg - incoming {@link WhatsAppRequestMessage}.
	 * @return - outgoing {@link WhatsAppResponseMessage}.
	 */
	private WhatsAppMessage processLogout(WhatsAppMessage msg) {
		
		database.removeUserByCookie(((WhatsAppRequestMessage)msg).getCookie());
		WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 200);
		response.addBody("Bye Bye..");
		return response;
	}
	/**
	 *if the request is valid - creates a new cookie for the user and adds it to 
	 *the database, else returns a proper response error.
	 * @param msg - incoming {@link WhatsAppRequestMessage}.
	 * @return - outgoing {@link WhatsAppResponseMessage}.
	 */
	private WhatsAppMessage processLogin(WhatsAppMessage msg) {
		String body = msg.getBody();
		if(!(body.contains("UserName") && body.contains("Phone"))){
			WhatsAppResponseMessage response = new  WhatsAppResponseMessage(msg.getHttpVersion(), 765);
			response.addBody("Error - Wrong parameters");
			return response;
		}
		else{

			String cookie = generateCookie(msg);
			WhatsAppResponseMessage response = new WhatsAppResponseMessage(msg.getHttpVersion(), 200);
			String userName = body.substring(9,body.indexOf('&'));

			//Check if username is taken:

			if(database.containsUsername(userName)){
				WhatsAppResponseMessage responseError = new  WhatsAppResponseMessage(msg.getHttpVersion(), 777);
				responseError.addBody("Error - Username taken");
				return responseError;
			}
			String phoneNumber = body.substring(body.indexOf('=', 10) + 1);
			System.out.println("Phone: " + phoneNumber);
			//Check if the phone number is taken:
			if(database.containsPhoneNumber(phoneNumber)){
				WhatsAppResponseMessage responseError = new  WhatsAppResponseMessage(msg.getHttpVersion(), 787);
				responseError.addBody("Error - Phone number taken");
				return responseError;
			}

			response.addHeader("Set-Cookie", cookie);
			response.addBody("Welcome " + userName + "@" + phoneNumber);

			System.out.println("returning: " + response.getBody());
			System.out.println("Cookie is: " + cookie);

			//add the user to the database:
			database.addUser(userName, phoneNumber, cookie);
			System.out.println("************** New user added**************\nCurrent users are:\n" +" "+ database.usersToString());
			return response;
		}
	}
	/**
	 * checks whether message's Uri supported by WhatsApp Interface.
	 * @param msg
	 * @return true iff The Message's Uri is valid.
	 */
	private boolean isLegalUri(WhatsAppMessage msg){
		String type = ((WhatsAppRequestMessage)msg).getUri();
		return(type.equals("login.jsp")||type.equals("logout.jsp")||type.equals("list.jsp")||type.equals("create_group.jsp")||
				type.equals("send.jsp")||type.equals("add_user.jsp")||type.equals("remove_user.jsp")||type.equals("queue.jsp"));
	}
	/**
	 * generates a unique cookie String for the user logging in.
	 * @param msg
	 * @return String - user's cookie.
	 */
	private String generateCookie(WhatsAppMessage msg){
		String uuid = UUID.randomUUID().toString();
		String cookie = "user_auth=" + uuid;
		return cookie;
	}

	/**
	 * checks whether User's cookie exists in the database.
	 * @param msg
	 * @return true iff The User's cookie exists.
	 */
	private boolean checkAuthenication(WhatsAppMessage msg) {
		String cookie = ((WhatsAppRequestMessage)msg).getCookie();
		return (database.containsCookie(cookie));
	}

	public boolean isEnd(WhatsAppMessage msg) {
		return false;
	}

	public boolean shouldClose() {
		return false;
		//TODO Check!!!!
	}

	public void connectionTerminated() {
	}
}
