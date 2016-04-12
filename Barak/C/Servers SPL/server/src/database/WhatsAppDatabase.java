package database;
public class WhatsAppDatabase {
	private UsersDatabase users;
	/**
	 * a class used to contain and organize all of the users/groups database.
	 * all of the methods are synchronized on the users to
	 * make the database multiple-Threads safe.
	 * 
	 */
	public WhatsAppDatabase(){
		users = new UsersDatabase();
	}
	public void addUser(String userName, String phoneNumber, String cookie){
		synchronized (users) {
			users.addUser(userName, phoneNumber, cookie);			
		}
	}
	public boolean containsCookie(String cookie){
		synchronized (users) {
			return users.containsCookie(cookie);
		}
	}
	public boolean containsUsername(String userName){
		synchronized (users) {
			return users.containsUser(userName);
		}
	}
	public User getUserByUserName(String userName){
		synchronized (users) {
			return users.getUserByUserName(userName);
		}
	}
	public boolean containsPhoneNumber(String phoneNumber){
		synchronized (users) {
			return users.containsPhoneNumber(phoneNumber);
		}
	}
	public void removeUserByCookie(String cookie){
		synchronized (users) {
			users.removeUserByCookie(cookie);
		}
	}
	public void addMessageToUser(String phoneNumber, String message){
		synchronized (users) {
			users.addMessageToUser(phoneNumber, message);
		}
	}

	public String usersToString() {
		synchronized (users) {
			return users.toString();			
		}
	}
	public String getMessagesByCookie(String cookie) {
		synchronized (users) {			
			if(users.containsCookie(cookie)){
				return users.getMessagesByCookie(cookie);
			}
			else
				return null;
		}
	}
	public User getUserByCookie(String cookie) {
		synchronized (users) {			
			if(users.containsCookie(cookie)){
				return users.getUserByCookie(cookie);
			}
			else
				return null;
		}
	}

	public boolean containsGroup(String groupName) {
		synchronized (users) {
			return users.containsGroup(groupName);
		}
	}

	public void addMessageToGroup(String groupName, String message) {
		synchronized (users) {
			users.addMessageToGroup(groupName, message);
		}
	}

	public void addGroup(Group newGroup) {
		synchronized (users) {
			users.addGroup(newGroup);
		}
	}

	public String generateUsersList() {
		synchronized (users){
			return users.generateUsersList();
		}

	}
	public String generateGroupsList() {
		synchronized (users){
			return users.generateGroupsList();
		}
	}
	public Group getGroupByName(String groupName){
		synchronized (users){
			return users.getGroupByName(groupName);
		}
	}
	public User getUserByPhoneNumber(String phoneNumber){
		synchronized (users){
			return users.getUserByPhoneNumber(phoneNumber);
		}
	}
	public void removeUserFromGroup(String groupName, String phoneNumber){
		synchronized (users){
			users.removeUserFromGroup(groupName, phoneNumber);
		}
	}
	public String generateUsersInGroupList(String groupName) {
		synchronized (users) {
			return users.generateUsersInGroupList(groupName);
		}		
	}
	public void addUserToGroup(String groupName, String phoneNumber) {
		synchronized (users) {
			User tmpUser = users.getUserByPhoneNumber(phoneNumber);
			users.getGroupByName(groupName).addUserToGroup(tmpUser);
		}
	}
	public boolean containsUserInGroup(String groupName, String phoneNumber) {
		synchronized (users) {
			Group group = getGroupByName(groupName);
			return group.containsUserByName(phoneNumber);
		}
	}
}

