#include <stdio.h>
#include "client/miniproject.h"

static int port = 9999;
static char ip[] = "127.0.0.1";

int main()
{
	struct udp_conn udp;


	udp_init_client(&udp, port, ip);

	int len = 6;
	char buf[6] = "START";

	udp_send(&udp, buf, len);

	len = 4;
	//buf[4] = "GET";
	
	udp_send(&udp, "GET", len);
	//buf = "";
	udp_receive(&udp, buf, 100);
	printf("received msg: %s\n", buf);

	udp_close(&udp);
	return 0;
}


