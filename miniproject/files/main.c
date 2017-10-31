#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../client/miniproject.h"

void timespec_add_us(struct timespec *t, long us);
void mini(void);
void* udp_listener(void*);

struct udp_conn UDP;

char IP[] = "127.0.0.1";
int PERIOD_NS = 500;
int RUNTIME = 0;
const int Kp = 10;
const int Ki = 800;
const int LENGTH = 128;
char *Y = NULL;

pthread_mutex_t RSS;



int main(void)
{
	char buf[128];
	//strcpy(IP, "127.0.0.1");
	udp_init_client(&UDP, 9999, &IP);


	
	strcpy(buf, "START\0");
	udp_send(&UDP, buf, strlen(buf));

	pthread_t listener;
	pthread_create(&listener, NULL, udp_listener, NULL);
	udp_listener(NULL);
	
	clock_nanosleep(5);
	pthread_t doer;
	pthread_create(&doer, NULL, mini, NULL);
	
	pthread_join(listener, NULL);
	pthread_join(&doer, NULL);
	udp_close(&UDP);
    return 0;
}

void* udp_listener(void* arg){
	char buf[128];
	while(1){
		pthread_mutex_lock(&RSS);
		
		strcpy(buf, "GET_ACK");
		udp_receive(&UDP, buf, LENGTH);
		strtok_r(&buf, ":", &Y);

		pthread_mutex_unlock(&RSS);
	}
}



void mini(){
	
	int ref = 1;
	int integral = NULL;
	char buf[128];

	while(1){
		pthread_mutex_lock(&RSS);
		strcpy(buf, "GET\0");
		udp_send(&UDP, buf, strlen(buf));

		int error = ref - atof(&Y);
		integral = integral + error * PERIOD_NS;
		int u = Kp * error + Ki * integral;

		sprintf(buf, "SET:%d\0", u);
		udp_send(&UDP, buf, (4 + strlen(u)));
		pthread_mutex_unlock(&RSS);


	}
	strcpy(buf, "STOP\0");
	udp_send(&UDP, buf, strlen(buf));
}
