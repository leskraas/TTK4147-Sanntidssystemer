#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "../client/miniproject.h"

struct udp_conn UDP;

char IP[] = "127.0.0.1";


int main(void)
{
	printf("Heihei \n");
	char buf[128];
	udp_init_client(&UDP, 9999, IP);

	int i = 0;
	for (i = 0; i < 10; i++){
		strcpy(buf, "START\0");
		udp_send(&UDP, buf, 6);
		clock_nanosleep(5000);
	}

	udp_close(&UDP);
    return 0;
}
