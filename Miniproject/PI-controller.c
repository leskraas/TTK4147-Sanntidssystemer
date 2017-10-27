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
	udp_send(udp, buf)
}


