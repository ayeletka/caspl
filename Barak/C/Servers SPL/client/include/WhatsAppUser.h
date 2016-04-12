#ifndef WHATSAPPUSER_H_
#define WHATSAPPUSER_H_
#include "../include/TcpClient.h"
#include "../include/ResponseMessage.h"
#include <iostream>
#include <string>
using namespace std;
class WhatsAppUser {
public:
	WhatsAppUser(string serverAddress, int serverPort);
	void connect();
private:
	string login();
	string logout();
	string sendRequest();
	string sendRequest(int index);
	string sendUserMessage();
	string sendUserMessage(string phoneNumber, string message);
	string sendGroupMessage();
	string sendGroupMessage(string groupName, string message);
	string finalizeRequest(string dataToSend, int selectionNumber);
	string listAllUsers();
	string listGroup(string groupName);
	string listAllGroups();
	string listGroup();
	string addUserToGroup();
	string addUserToGroup(string groupName, string phoneNumber);
	string removeUserFromGroup();
	string removeUserFromGroup(string groupName, string phoneNumber);
	string createGroup();
	string sendManualRequest();
	string createGroup(string groupName, string users);
	ResponseMessage *parseMessage(string messageToParse);
	void listenToServer();
	string _userName;
	string _phone;
	TcpClient _client;
	string _cookie;
	bool loggedIn;
};

#endif
