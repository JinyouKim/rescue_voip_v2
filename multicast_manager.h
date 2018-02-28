#ifndef _MULTICAST_MANAGER_H_
#define _MULTICAST_MANAGER_H_
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

class MulticastManager {
	private:
		char *groupIP;
		uint16_t port;
	public:
		MulticastManager(char *groupIP, uint16_t port);
		int joinMulticastGroup();
		int send(char *message, unsigned int length);
		int recv(char *message, unsigned int length);

	private:
		int sockfd;
		struct sockaddr_in sendAddress, recvAddress;
		socklen_t addressLength;
		


};

#endif
