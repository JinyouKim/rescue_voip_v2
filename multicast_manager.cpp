#include "multicast_manager.h"

MulticastManager::MulticastManager(char *_groupIP, uint16_t _port) {
	groupIP = _groupIP;
	port = _port;
	
}

int MulticastManager::joinMulticastGroup() {
	struct ip_mreq mreq;
	/* set up socket */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printError("socket()");
		return -1;		
	}

	bzero((char *)&sendAddress, sizeof(sendAddress));
	bzero((char *)&recvAddress, sizeof(recvAddress));
	sendAddress.sin_family = recvAddress.sin_family = AF_INET;
	sendAddress.sin_addr.s_addr = inet_addr(groupIP);
	recvAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	sendAddress.sin_port = recvAddress.sin_port = htons(port);
	addressLength = sizeof(sendAddress);

	if (bind(sockfd, (struct sockaddr *) &recvAddress, sizeof(recvAddress)) < 0) {
		printError("bind()");
		return -1;
	}
	mreq.imr_multiaddr.s_addr = inet_addr(groupIP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	/* join multicast group */
	if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
		printError("setsocketopt()");
		return -1;
	}
	return 0;
}

int MulticastManager::send(char *message, unsigned int length) {
	int cnt = sendto(sockfd, message, length, 0, (struct sockaddr *) &sendAddress, addressLength);
	return cnt;
}

int MulticastManager::recv(char *message, unsigned int length) {
	int cnt = recvfrom(sockfd, message, length, 0, (struct sockaddr *) &recvAddress, &addressLength);
	return cnt;
}
