#include <stdio.h>
#include "token_manager.h"
#include "multicast_manager.h"

int main()
{
	TokenManager tm("141.223.84.110", 9111);
	MulticastManager mm("239.0.0.1", 9111);

	tm.connectServer();
	printf("%d\n", tm.getToken());
	printf("%d\n", tm.returnToken());
	tm.returnToken();
	tm.getToken();
	tm.getToken();
	tm.returnToken();
	mm.joinMulticastGroup();

	return 0;
}
