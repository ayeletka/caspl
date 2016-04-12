#ifndef RESPONSEMESSAGE_H_
#define RESPONSEMESSAGE_H_
#include <map>
#include <string>
#include <vector>
using namespace std;

class ResponseMessage{
public:
	ResponseMessage (string httpVersion, string code);
	void addHeader(string headerName, string headerValue);
	void addBody(string body);
	string toString();
	string getCode();
	string getHeaderValue(string header);
	string getBody();
private:
	string _httpVersion;
	string _code;
	map<string, string> _headersMap;
	vector<string> _headersVector;
	string _body;

};




#endif
