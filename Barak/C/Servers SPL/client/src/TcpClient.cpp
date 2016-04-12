#include "../include/TcpClient.h"
#include<iostream>    //cout
#include<string>  //string
#include<string.h>    //strlen
#include<stdio.h> //printf
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h> //hostent

using namespace std;
TcpClient::TcpClient():_address(""), _port(0), _socket(-1), _server(){}

TcpClient::TcpClient(string address, int port):_address(address), _port(port), _socket(-1), _server(){}

void TcpClient::Initialize(string serverAddress, int serverPort){
	_address = serverAddress;
	_port = serverPort;
}

/**
    Connect to a host on a certain port number
*/
bool TcpClient::connectToServer(){
    //create socket if it is not already created
    if(_socket == -1)
    {
        //Create socket
        _socket = socket(AF_INET , SOCK_STREAM , 0);
        if (_socket == -1)
        {
            perror("Could not create socket");
        }
        cout<<"Socket created\n";
    }
    _server.sin_addr.s_addr = inet_addr(_address.c_str() );
    _server.sin_family = AF_INET;
    _server.sin_port = htons(_port );
    //Connect to remote server
    if(connect(_socket , (struct sockaddr *)&_server , sizeof(_server)) < 0){
        perror("connect failed. Error");
        return 1;
    }
    cout<<"Connected\n";
    return true;
}

/**
    Send data to the connected host
*/
bool TcpClient::send_data(string data)
{
    if( send(_socket , data.c_str() , strlen( data.c_str() ) , 0) < 0)
    {
        perror("Send failed : ");
        return false;
    }
    return true;
}

/**
    Receive data from the connected host
*/
string TcpClient::receive(int size=512)
{
	char buffer[size];
    string reply;
    //Receive a reply from the server
    if(recv(_socket, buffer, sizeof(buffer),0)<0){
        puts("recv failed");
    }
    reply = buffer;
    return reply;
}

