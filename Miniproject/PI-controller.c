#include <stdio.h>
#include <string.h>
#include "client/miniproject.h"

static int port = 9999;
static char ip[] = "127.0.0.1";

int main(void)
{
	struct udp_conn udp;


	udp_init_client(&udp, port, ip);

	int len = 6;
	char buf[6] = "START";

	udp_send(&udp, buf, len);

	len = 4;
	//buf[4] = "GET";
	
	udp_send(&udp, "GET", len);

	char msg_receive[100];
	//printf("er her1\n");


	udp_receive(&udp, msg_receive, 100);

	unsigned int  i;
	char *temp;
	printf("%lu\n", strlen(msg_receive));
	for (i = 0; i < strlen(msg_receive)-8; i++){
		printf("dette går bra\n");
		printf("%\n", &temp[0]);  
	} 
	printf("received msg: %s\n", temp);
	printf("%s\n", msg_receive);


	udp_send(&udp, "STOP", 5);

	udp_close(&udp);
	return 0;
}


