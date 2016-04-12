package database;

import java.util.ArrayList;

public class Group {
	private String groupName;
	ArrayList<User> groupUsers;

	public Group (User creator, String groupName){
		this.groupName = groupName;
		this.groupUsers = new ArrayList<User>();
		groupUsers.add(creator);
		creator.addUserToGroup(this);
		System.out.println("Group " + groupName + " Created successfully.\nGroup users are:\n" + groupUsers.toString());
	}
	/**
	 * adds a given user to the group.
	 * @param userToAdd
	 */
	public void addUserToGroup(User userToAdd){
		this.groupUsers.add(userToAdd);
		userToAdd.addUserToGroup(this);
		System.out.println("User " + userToAdd.getUserName() + " Added successfully. \nGroup users are:\n" + groupUsers.toString());
	}
	/**
	 * sends a message to the group's users.
	 * @param userToAdd
	 */
	public void sendMessageToGroup(String message){
		String intro = "[" + groupName + "] -- ";
		for(User tmpUser : groupUsers){
			tmpUser.addMessage(intro+ message+ "--");
		}
	}
	/**
	 * checks whether the group contains a user.
	 * @param phoneNumber
	 * @return true iff the user exists in the group.
	 */
	public boolean containsUserByPhone(String phoneNumber){
		boolean ans = false;
		for(User user : groupUsers){
			if (user.getphoneNumber().equals(phoneNumber)){
				return true;
			}
		}
		return ans;
	}
	public String getGroupName() {
		return this.groupName;
	}
	/**
	 * removes a given user from the group.
	 * @param userToAdd
	 */
	public void removeUser(String phoneNumber) {
		for(User user : groupUsers){
			if (user.getphoneNumber().equals(phoneNumber)){
				groupUsers.remove(user);
				user.removeGroup(this);
				break;
			}
		}
	}
	public int getNumOfUsers(){
		return this.groupUsers.size();
	}
	/**
	 * lists all the users in the group.
	 * @param userToAdd
	 */
	public String listUsers() {
		StringBuilder builder = new StringBuilder();
		builder.append("-[" +  getGroupName() + "] group users-\n");
			for(User user : groupUsers){
				builder.append("[" + user.getUserName() + "]@[" + user.getphoneNumber() + "]\n");
			}
			return builder.toString();
		}
	public boolean containsUserByName(String phoneNumber) {
		boolean ans = false;
		for(User user : groupUsers){
			if (user.getphoneNumber().equals(phoneNumber)){
				return true;
			}
		}
		return ans;
	}
}
