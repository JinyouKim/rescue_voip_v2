#include "token_manager.h"

TokenManager::TokenManager(char* _serverIP, unsigned int _serverPort) {
	serverIP = _serverIP;
	serverPort = _serverPort;
	hasToken = false;
	buffer = (char *)malloc(MAX_BUFFER_SIZE);
}

int TokenManager::connectServer() {
	struct sockaddr_in serverAddress;

	int ret;
	socklen_t length;

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printError("socket()");
		return -1;
	}

	bzero((char *) &serverAddress, sizeof(serverAddress));

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(serverIP);
	serverAddress.sin_port = htons(serverPort);

	printf("Connecting to server ......\n");

	if ((ret = connect(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress))) < 0) {
		if (errno != EINPROGRESS)
			return -1;
	}

	if (ret == 0) {
		printf("Connect Success!\n");
		return 0;
	}
		
}

int TokenManager::getToken() {
	if (hasToken) {
		printf("Token exists already\n");
		return -1;
	}
	int ret;
	int response = 0;
	Message *message = (Message *)malloc(sizeof(Message));
	message->type = 0;
	message->dataLength = 0;

	unsigned int messageLength = sizeof(unsigned int) * 2 + message->dataLength;
	
	serialize(message, buffer, messageLength);

	if (write(sockfd, buffer, messageLength) < 0) {
		printError("write()");
		return -1;
	}

	if ((ret = read(sockfd, buffer, MAX_BUFFER_SIZE)) < 0) {
		printError("read()");
		return -1;
	}

	else if (ret == 0)
		return -1;

	else {
		deserialize(buffer, message);
		
		// token is passed
		if (message->type == 2) {
			ret = 1;
			hasToken = true;

		}
		// token is not passed
		else if (message->type == 3) {
			ret = 0;
			hasToken = false;
		}

		free(message);
		return ret;
	}
	
}

int TokenManager::returnToken() {

	if (!hasToken) {
		printf("Token does not exist\n");
		return -1;
	}

	int ret;
	Message *message = (Message *)malloc(sizeof(Message));
	message->type = 1;
	message->dataLength = 0;

	unsigned int messageLength = sizeof(unsigned int) * 2 + message->dataLength;
	
	serialize(message, buffer, messageLength);

	if (write(sockfd, buffer, messageLength) < 0) {
		printError("write()");
		return -1;
	}

	if ((ret = read(sockfd, buffer, MAX_BUFFER_SIZE)) < 0) {
		printError("read()");
		return -1;
	}

	else if (ret == 0)
		return -1;

	else {
		deserialize(buffer, message);
		
		// token is passed
		if (message->type == 1) {
			printf("Token is returned to server\n");
			hasToken = false;
		}

		free(message);
		return 0;
	}
	
}
void TokenManager::serialize(Message* message, char *data, unsigned int messageSize) {
	unsigned int *q = (unsigned int*)data;
	*q = message->type; q++;
	*q = message->dataLength; q++;
	
	char *p = (char *)q;
	unsigned int i = 0;
	while (i < message->dataLength) {
		*p = message->data[i];
		p++;
		i++;
	}
}

void TokenManager::deserialize(char *data, Message* message) {
	memcpy(&message->type, data, sizeof(unsigned int)); data = data + sizeof(unsigned int);
	memcpy(&message->dataLength, data, sizeof(unsigned int)); data = data + sizeof(unsigned int);
	memcpy(message->data, data, message->dataLength);
}
