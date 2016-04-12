package database;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

public class User {
	private String userName;
	private String phoneNumber;
	private String cookie;
	private ArrayList<String> messages;
	private ArrayList<Group> myGroups;
	public User(String userName, String phoneNumber, String cookie){
		this.userName = userName;
		this.phoneNumber = phoneNumber;
		this.cookie = cookie;
		this.messages = new ArrayList<String>();
		this.myGroups = new ArrayList<Group>();
	}
	public String getUserName(){
		return this.userName;
	}
	public String getphoneNumber(){
		return this.phoneNumber;
	}
	public String getCookie(){
		return this.cookie;
	}
	public String toString(){
		String str = "";
		str += userName;
		str += ", " + phoneNumber;
		str += ", " + cookie + "\n";
		return str;
	}
	public void addMessage(String message) {
		SimpleDateFormat sdfDate = new SimpleDateFormat("yyyy-MM-dd HH-mm-ss");
		Date now = new Date();
		String strDate = sdfDate.format(now);
		String intro = "Message -- " + strDate + "\n";
		messages.add(intro + message);
		System.out.println(intro + message);
	}
	public ArrayList<String> getMessages() {
		return this.messages;
	}
	public void addUserToGroup(Group group) {
		this.myGroups.add(group);
		this.addMessage("You have been added to group- " + group.getGroupName());
		
	}
	public void removeGroup(Group group) {
		myGroups.remove(group);		
	}
	public ArrayList<Group> getGroups() {
		return this.myGroups;
	}
}
