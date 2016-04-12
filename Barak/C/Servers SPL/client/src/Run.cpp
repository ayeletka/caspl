
#include <string>
#include <boost/lexical_cast.hpp>


#include "../include/WhatsAppUser.h"
using namespace std;
int main(int argc , char *argv[]){
	if(argv[1]!=0){
	string serverAddress = argv[1];
	string sServerPort = argv[2];
	cout << sServerPort << endl;
	cout << serverAddress << endl;
	int serverPort = boost::lexical_cast<int>(sServerPort);

	WhatsAppUser *bobcat = new WhatsAppUser(serverAddress, serverPort);
	bobcat->connect();
	delete bobcat;
	}
	else{
		cout << "Wrong input parameters. Please use: -Address-  -Port-" << endl;
	}
	return 0;
}


