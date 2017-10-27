#include <stdio.h>
#include <string>
#include <client/miniproject.h>

static int port = 9999;
static int ip = 127.0.0.1;

int main()
{
	struct udp_conn udp;


	udp_init_client(udp, port, ip)

	char buf[] = "GET";
	int len = 3;
	udp_send(udp, buf, len);
	udp_receive(udp, buf, 100);
	printf("received msg: %d\n", buf);

	udp_close(udp);
	return NULL;
}


