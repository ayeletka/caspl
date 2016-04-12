package database;

import java.util.ArrayList;
public class UsersDatabase {
	private ArrayList<User> users;
	private ArrayList<Group> groups;
	public UsersDatabase(){
		this.users = new ArrayList<User>();
		this.groups = new ArrayList<Group>();
	}

	public synchronized boolean containsGroup(String groupName){
		for(Group group: groups){
			if(group.getGroupName().equals(groupName))
				return true;
		}
		return false;
	}
	/**
	 * creates a new User and inserts it to the database.
	 * @param userName
	 * @param phoneNumber
	 * @param cookie
	 */
	public synchronized void addUser(String userName, String phoneNumber, String cookie){
		users.add(new User(userName, phoneNumber, cookie));
	}
	public synchronized boolean containsUser(String userName){
		for(User user: users){
			if(user.getUserName().equals(userName))
				return true;
		}
		return false;
	}
	public synchronized User getUserByPhoneNumber(String phoneNumber){
		for(User user: users){
			if(user.getphoneNumber().equals(phoneNumber))
				return user;
		}
		return null;
	}
	/** 
	 * Sends a message to a specific user.
	 * @param phoneNumber - users phone number.
	 *@param message - a message needed to be sent. 
	 */
	public synchronized void addMessageToUser(String phoneNumber, String message){
		if(containsPhoneNumber(phoneNumber)){
			User user = getUserByPhoneNumber(phoneNumber);
			user.addMessage(message);
		}
	}

	public synchronized boolean containsPhoneNumber(String phoneNumber){
		for(User user: users){
			if(user.getphoneNumber().equals(phoneNumber))
				return true;
		}
		return false;
	}
	public synchronized boolean containsCookie(String cookie){
		for(User user: users){
			if(user.getCookie().equals(cookie))
				return true;
		}
		return false;
	}
	public synchronized String toString(){
		return this.users.toString();
	}
	/** 
	 * removes a specific User from the system by its phone number.
	 * @param cookie
	 * @return
	 */
	public synchronized void removeUserByCookie(String cookie) {
		for(User user: users){
			if(user.getCookie().equals(cookie)){
				//Remove the user from all of it's groups:
				while(!user.getGroups().isEmpty()){
				    System.out.println("Removing user " + user.getUserName() + " from group " + user.getGroups().get(0));
					user.getGroups().get(0).removeUser(user.getphoneNumber());
					System.out.println("Removed Successfully!!");
				
				}
				/*
				for(Group group : user.getGroups()){
					
				}
				*/
				System.out.println("Removing User from system:");
				users.remove(user);
				System.out.println("User: " + user.getUserName() + "Erased from Database.");
				System.out.println("Current users are:\n " + users);
				break;
			}
		}
	}
	/** 
	 * returns all of specific user's messages.
	 * @param cookie
	 * @return
	 */
	public synchronized String getMessagesByCookie(String cookie){
		StringBuilder builder = new StringBuilder();
		ArrayList<String> messages = getUserByCookie(cookie).getMessages();
		if(!messages.isEmpty()){
			for(String message: messages){
				builder.append(message);
			}
			messages.clear();
			return builder.toString();
		}
		//No new messages:
		else{
			return "No new messages";
		}
	}
	public synchronized User getUserByCookie(String cookie) {
		for(User user: users){
			if(user.getCookie().equals(cookie)){
				return user;
			}
		}
		return null;
	}
	public synchronized User getUserByUserName(String userName) {
		for(User user: users){
			if(user.getUserName().equals(userName)){
				return user;
			}
		}
		return null;
	}
	/**
	 * sends message to all of particular group's users.
	 * @param groupName
	 * @param message
	 */
	public synchronized void addMessageToGroup(String groupName, String message) {
		for(Group group: groups){
			if(group.getGroupName().equals(groupName)){
				group.sendMessageToGroup(message);
			}
		}		
	}

	public synchronized void addGroup(Group newGroup) {
		groups.add(newGroup);
	}
	/**
	 * lists all of the users in the system.
	 * @return String - system's users.
	 */
	public synchronized String generateUsersList() {
		StringBuilder sb = new StringBuilder();
		for(User user : users){
			sb.append("[" + user.getUserName() + "]@[" + user.getphoneNumber() + "]\n");
		}
		return sb.toString();
	}
	/**
	 * lists all of the groups in the system.
	 * @return String - system's groups.
	 */
	public synchronized String generateGroupsList() {
		StringBuilder sb = new StringBuilder();
		for(Group group : groups){
			sb.append(group.listUsers() + "\n");
		}
		return sb.toString();
	}

	public synchronized Group getGroupByName(String groupName) {
		for(Group group: groups){
			if(group.getGroupName().equals(groupName)){
				return group;
			}

		}		
		return null;
	}
	/**
	 * removes a specific user from a given group.
	 * @param groupName to delete from
	 * @param phoneNumber user's number to delete.
	 */
	public synchronized void removeUserFromGroup(String groupName, String phoneNumber) {
		Group group = getGroupByName(groupName);
		group.removeUser(phoneNumber);
		if (group.getNumOfUsers() == 0){
			groups.remove(group);
		}
	}

	public synchronized String generateUsersInGroupList(String groupName) {
		Group group = getGroupByName(groupName);
		return group.listUsers();
	}
}