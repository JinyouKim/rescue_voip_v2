#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include "common.h"

typedef struct {
	unsigned int type;
	unsigned int dataLength;
	char *data;
}Message;

class TokenManager {
public:
	TokenManager(char *serverIP, unsigned int serverPort);

	int getToken();
	int returnToken();
	int connectServer();
private:
	void serialize(Message* message, char* data, unsigned int messageSize);
	void deserialize(char* data, Message* message);
private:
	char *serverIP;
	unsigned int serverPort;
	const size_t MAX_BUFFER_SIZE = 255;
	int sockfd;
	char* buffer;
	bool hasToken;

};
