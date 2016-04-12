# All Targets

all: client

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
client: bin/Run.o bin/ResponseMessage.o bin/TcpClient.o bin/WhatsAppUser.o 
	@echo 'Building target: client'
	g++ -o bin/client  bin/Run.o bin/ResponseMessage.o bin/TcpClient.o bin/WhatsAppUser.o -lboost_system -lboost_system  -lboost_filesystem  -lboost_thread -L/usr/lib 
	@echo 'Finished building target: client' 
	@echo ' '

bin/ResponseMessage.o: src/ResponseMessage.cpp
	g++ -g -Wall -Weffc++ -c -o bin/ResponseMessage.o src/ResponseMessage.cpp
	
bin/TcpClient.o: src/TcpClient.cpp
	g++ -g -Wall -Weffc++ -c -o bin/TcpClient.o src/TcpClient.cpp

bin/WhatsAppUser.o: src/WhatsAppUser.cpp
	g++ -g -Wall -Weffc++ -c -o  bin/WhatsAppUser.o src/WhatsAppUser.cpp  
		
bin/Run.o: src/Run.cpp
	g++ -g -Wall -Weffc++ -c -o bin/Run.o src/Run.cpp 
	

clean: 
	rm -f bin/*
