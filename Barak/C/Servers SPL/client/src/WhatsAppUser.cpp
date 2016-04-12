
#include "../include/WhatsAppUser.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>
using namespace std;
const int MESSAGE_CHECK_INTERVAL = 1000;

WhatsAppUser::WhatsAppUser(string serverAddress, int serverPort):_userName(""), _phone(""),_client(),_cookie(""), loggedIn(0){
	_client.Initialize(serverAddress, serverPort);
}

/**
 * Listens to the server for incoming messages, while the bool listen to sever is true.
 */
void WhatsAppUser::listenToServer(){
	cout << "Starting to listen.... " << endl;
	while(loggedIn){
		string messageToSend = "GET /queue.jsp HTTP/1.1\nCookie: " + _cookie + "\n$";
		string receivedMessage = "";
		//If request failed:
		if(!_client.send_data(messageToSend)){
			loggedIn = false;
			break;
		}
		receivedMessage = _client.receive(1024);
		if(receivedMessage != "recv failed"){
			ResponseMessage *response = parseMessage(receivedMessage);
			string body = response->getBody();
			boost::trim(body);
			if(body != "No new messages"){
				cout << response->getBody()<< endl;
			}

		}
		boost::posix_time::milliseconds myTime(MESSAGE_CHECK_INTERVAL);
		boost::this_thread::sleep(myTime);
	}
}

/**
 * Connects to the server, by the tcpClient.
 */
void WhatsAppUser::connect(){
	if(_client.connectToServer()){
		string status = "";
		cout << "Welcome to the client app!\nPlease select input method:\n1.Menu interface\n2.Text command input" << endl;
		string selection = "";
		cin >> selection;
		cin.ignore();

		//Menu entry
		if(selection == "1"){
			string username;
			string phone;
			cout << "Enter username:" << endl;
			cin >> username;
			_userName = username;
			cout << "Enter phone number:" << endl;
			cin >> phone;
			_phone = phone;
			while(status != "exit"){
				status = sendRequest();
			}
		}

		//Manual entry
		else if(selection == "2"){
			while (status != "exit"){
				status = sendManualRequest();
			}
		}
		else{
			cout << "Wrong input... Try again\n" << endl;
		}

	}
}

/**
 * Send a request entered mannualy.
 */
string WhatsAppUser::sendManualRequest(){
	int selectionNumber = 0;
	string dataToSend = "";
	bool isLegalMessage = false;
	while (!isLegalMessage){
		isLegalMessage = true;
		cout << "Please write your request\n" << endl;
		string input = "";
		cin.clear();
		getline(cin, input);

		vector<string> inputWords;
		boost::split(inputWords, input, boost::is_any_of(" "));
		boost::to_lower(inputWords.at(0));

		if(inputWords.at(0) == "login"){
			_userName = inputWords.at(1);
			_phone = inputWords.at(2);
			selectionNumber = 1;
			dataToSend = login();

		}
		else if(inputWords.at(0) == "logout"){
			dataToSend = logout();
			selectionNumber = 10;
		}
		else if(inputWords.at(0) == "list"){
			boost::to_lower(inputWords.at(1));
			if(inputWords.at(1) == "users"){
				dataToSend = listAllUsers();
			}
			else if(inputWords.at(1) == "groups"){
				dataToSend = listAllGroups();

			}
			else if(inputWords.at(1) == "group"){
				string groupName = inputWords.at(2);
				dataToSend = listGroup(inputWords.at(3));
			}
		}
		else if(inputWords.at(0) == "send"){
			boost::to_lower(inputWords.at(1));

			if(inputWords.at(1) == "group"){
				string groupName = inputWords.at(2);
				string message = inputWords.at(3);
				dataToSend = sendGroupMessage(groupName, message);
			}
			else if(inputWords.at(1) == "user"){
				cout << "Case User Detected:" <<  endl;
				string phone = inputWords.at(2);
				string message = "";
				for (size_t st = 3; st < inputWords.size(); st++){
					message  += inputWords.at(st) + " ";
				}
				cout << "Sending message:" << message << endl;
				dataToSend = sendUserMessage(phone, message);
			}
		}
		else if(inputWords.at(0) == "add"){
			string groupName = inputWords.at(1);
			string phone = inputWords.at(2);
			dataToSend = addUserToGroup(groupName, phone);
		}
		else if(inputWords.at(0) == "remove"){
			string groupName = inputWords.at(1);
			string phone = inputWords.at(2);
			dataToSend = removeUserFromGroup(groupName, phone);
		}
		else if(inputWords.at(0) == "create"){
			string groupName = inputWords.at(2);
			string users = inputWords.at(3);
			dataToSend = createGroup(groupName, users);

		}
		else if(inputWords.at(0) == "exit"){
			cout << "Exiting..." << endl;
			if(loggedIn){
				finalizeRequest(logout(),10);
			}
			return "exit";
		}

		else{
			isLegalMessage = false;
			cout << "Incorrect command, sent aborted."<< endl;
		}

	}
	finalizeRequest(dataToSend, selectionNumber);
	return dataToSend;

}

/**
 * Log out of the server.
 */
string WhatsAppUser::logout(){
	string messageToSend = "GET /logout.jsp HTTP/1.1\nCookie: " + _cookie + "\n$";
	return messageToSend;
}

/*
 * Sends a message to a user.
 */
string WhatsAppUser::sendUserMessage(){
	string phoneNumber, message;
	cout << "Please enter The User's phone number:";
	cin >> phoneNumber;
	cin.ignore();
	cout << "Please Enter a Message:";
	getline(cin, message) ;
	cout << message;
	return sendUserMessage(phoneNumber, message);
}


string WhatsAppUser::sendUserMessage(string phoneNumber, string message){
	return ("POST  /send.jsp HTTP/1.1\nCookie: " + _cookie + "\nType: Direct\nTarget: " + phoneNumber + "\nContent: " + message + "\n$");
}

/*
 * Sends a group message.
 */
string WhatsAppUser::sendGroupMessage(){
	string groupName, message;
	cout << "Please enter The Group's Name:";
	cin >> groupName;
	cin.ignore();
	cout << "Please Enter a Message:";
	getline(cin, message) ;
	cout << message;
	return sendGroupMessage(groupName, message);
}

string WhatsAppUser::sendGroupMessage(string groupName, string message){

	return ("POST  /send.jsp HTTP/1.1\nCookie: " + _cookie + "\nType: Group\nTarget: " + groupName + "\nContent: " + message + "\n$");
}

string WhatsAppUser::listAllUsers(){
	return "POST /list.jsp HTTP/1.1\nCookie: " + _cookie +"\nList: Users\n$";
}

string WhatsAppUser::listAllGroups(){
	return "POST /list.jsp HTTP/1.1\nCookie: " + _cookie +"\nList: Groups\n$";
}

string WhatsAppUser::listGroup(){
	string groupName;
	cout<< "Please Enter The Specified Group Name:";
	cin >> groupName;
	return	listGroup(groupName);
}
string WhatsAppUser::listGroup(string groupName){
	return  "POST /list.jsp HTTP/1.1\nCookie: " + _cookie +"\nList: Group\nGroup_Name: " + groupName +"\n$";
}

/**
 * Adds a given user to a group.
 */
string WhatsAppUser::addUserToGroup(){
	string groupName;
	string phoneNumber;
	cout<< "Please Enter The Specified Group Name:";
	cin >> groupName;
	cout<< "Please Enter The phone number to Add:";
	cin >> phoneNumber;
	return addUserToGroup(groupName, phoneNumber);
}

string WhatsAppUser::addUserToGroup(string groupName, string phoneNumber){
	return "POST /add_user.jsp HTTP/1.1\nCookie: " + _cookie +"\nTarget: " + groupName + "\nUser: " + phoneNumber +"\n$";
}

/**
 * Removes a user from a group
 */
string WhatsAppUser::removeUserFromGroup(){
	string groupName;
	string phoneNumber;
	cout<< "Please Enter The Specified Group Name:";
	cin >> groupName;
	cout<< "Please Enter The phone number to remove:";
	cin >> phoneNumber;
	return "POST /remove_user.jsp HTTP/1.1\nCookie: " + _cookie +"\nTarget: " + groupName + "\nUser: " + phoneNumber +"\n$";
}
string WhatsAppUser::removeUserFromGroup(string groupName, string phoneNumber){
	return "POST /remove_user.jsp HTTP/1.1\nCookie: " + _cookie +"\nTarget: " + groupName + "\nUser: " + phoneNumber +"\n$";
}

string WhatsAppUser::login (){
	cout << "Logging in..." << endl;
	string dataToSend = "POST /login.jsp HTTP/1.1\nUserName=" + _userName + "&Phone=" + _phone+"\n$";
	return dataToSend;
}

/*
 * Creates a new group
 */
string WhatsAppUser::createGroup(){
	string groupName;
	string users;
	cout<< "Please Enter The Specified Group Name:";
	cin >> groupName;
	cin.clear();
	cout<< "Please Enter users to add (separated by ','):";
	cin.ignore();
	getline(cin, users);
	return "POST /create_group.jsp HTTP/1.1\nCookie: " + _cookie + "\nGroupName: " + groupName + "\nUsers: " + users +"\n$";
}

string WhatsAppUser::createGroup(string groupName, string users){
	return "POST /create_group.jsp HTTP/1.1\nCookie: " + _cookie + "\nGroupName: " + groupName + "\nUsers: " + users +"\n$";
}

string WhatsAppUser::sendRequest(int index){
	string dataToSend = "";
	switch(index){
	//login:
	case 1:
		dataToSend = login();
		break;
		//send user a message:
	case 2:
		dataToSend = sendUserMessage();
		cout << dataToSend << endl;
		break;
		//Send a message to a group:
	case 3:
		dataToSend = sendGroupMessage();
		break;
		//List all the users:
	case 4:
		dataToSend = listAllUsers();
		break;
		//List all the groups:
	case 5:
		dataToSend = listAllGroups();
		break;
		//List all the users in a group:
	case 6:
		dataToSend = listGroup();
		break;
	case 7:
		dataToSend = addUserToGroup();
		break;
	case 8:
		dataToSend = removeUserFromGroup();
		break;
	case 9:
		dataToSend = createGroup();
		break;
	case 10:
		dataToSend = logout();
		break;
	case 11:
		if(loggedIn){
			dataToSend = logout();
			finalizeRequest(dataToSend, 10);
		}
		return "exit";
		break;
	}

	finalizeRequest(dataToSend, index);
	return dataToSend;
}

/**
 * Sends a given request to the tcp client to send to the server.
 */
string WhatsAppUser::finalizeRequest(string dataToSend, int selectionNumber){
	cout << dataToSend<< endl;
	//If there is a connection problem:
	if(!_client.send_data(dataToSend)){
		loggedIn = false;
		return "exit";
	}
	string responseString = "";
	while(responseString == ""){
		responseString = _client.receive(1024);
	}
	ResponseMessage *responseMessage = parseMessage(responseString);
	cout << responseMessage->toString() << endl;
	if(responseMessage->getCode() == "200"){
		//Logged in:
		if(selectionNumber == 1){
			_cookie = responseMessage->getHeaderValue("Set-Cookie");
			loggedIn = true;
			boost::thread listen(&WhatsAppUser::listenToServer, this);
			listen.detach();
		}
		cout << "The Response Code is 200, Operation executed Successfully!\n\n" << endl;
		if (selectionNumber==10){
			cout<< "Disconnected from server."<< endl;
			loggedIn = false;
		}
	}
	delete responseMessage;
	return "done";
}

/**
 * Runs the request selection menu.
 */
string WhatsAppUser::sendRequest(){
	int requestNumber;
	cout<< "Please select your request number:\n1. Login\n2. Send Message to User\n3. Send Message to Group\n4. List All Users\n5. List All Groups\n6. List Users in a Specific Group\n7. Add User to a Group\n8. Remove From Group\n9. Create Group\n10. Logout\n11. Exit\n";
	cin >> requestNumber;
	cin.clear();
	cin.ignore();
	if(requestNumber < 1 || requestNumber > 11){
		cout << "------------------------------\nSelection error, Please try again:\n------------------------------" << endl;
		return "error";
	}
	else{
		return sendRequest(requestNumber);
	}
}

/**
 * Parses the message and creates a new response message object.
 */
ResponseMessage *WhatsAppUser::parseMessage(string messageToParse){
	messageToParse = messageToParse.substr(0, messageToParse.find('$') - 1);
	vector<string> messageByLines;
	boost::split(messageByLines, messageToParse, boost::is_any_of("\n"));

	size_t spaceLocation = messageByLines[0].find(' ');
	string httpVersion = messageByLines[0].substr(0, spaceLocation);
	string code = messageByLines[0].substr(spaceLocation + 1);
	ResponseMessage *response = new ResponseMessage(httpVersion, code);
	for(size_t i = 1; i< messageByLines.size(); i++){
		size_t seperator = (messageByLines[i].find(':'));
		if(seperator != string::npos){
			string tmpHeader = messageByLines[i].substr(0, seperator);
			string tmpHeaderValue = messageByLines[i].substr(seperator + 1);
			boost::trim(tmpHeader);
			boost::trim(tmpHeaderValue);
			response->addHeader(tmpHeader, tmpHeaderValue);
		}
		else{
			response->addBody(messageByLines[i]);
		}
	}

	return response;


}

