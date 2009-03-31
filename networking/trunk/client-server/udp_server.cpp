#include "server.h"
#define MAX_LEN 1024
#define BUF_LEN 64

int serialize_player(DPlaya *player, char *buf, size_t buflen);
int unserialize_player(char *buf, DPlaya *player);

void start_udp_server() {
	int sd, port = 8000, n, j;
	socklen_t client_len;
	char udpbuf[MAX_LEN];
	struct sockaddr_in udpserver, udpclient;
	DPlaya p1, p2;
	
	printf("starting udp server..\n");
	fflush(stdout);

	if((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("Socket Creation");
		exit(EXIT_FAILURE);
	}

	bzero((char *)&udpserver, sizeof(udpserver));
	udpserver.sin_family = AF_INET;
	udpserver.sin_port = htons(port);
	udpserver.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sd, (struct sockaddr *)&udpserver, sizeof(udpserver)) == -1) {
			perror("Binding Socket");
			exit(EXIT_FAILURE);
	}

	for(;;) {
		client_len = sizeof(udpclient);
		if((n = recvfrom(sd, udpbuf, MAX_LEN, 0, (struct sockaddr *)&udpclient, &client_len)) < 0) {
			perror("Recv From");
			exit(EXIT_FAILURE);
		}

		for(j=0; udpbuf[j] != -1; j++);
		udpbuf[j] = '\0';

		printf("len: %d - buf: %s\n", strlen(udpbuf), udpbuf);

		unserialize_player(udpbuf, &p1);

		printf("%d\n", p1.getX());
		printf("%d\n", p1.getY());
		printf("%d\n", p1.getNumBombs());
		printf("%d\n", p1.getDroppedBombs());
		printf("%d\n", p1.getDPlayaID());

		serialize_player(&p1, udpbuf, BUF_LEN);

		if(strcmp(udpbuf, "quit\n")==0) {
			close(sd);
			printf("closing udp server..\n");
			return;
		} else {
			if(sendto(sd, udpbuf, n, 0, (struct sockaddr *)&udpclient, client_len) < 0) {
				perror("Send To");
				exit(EXIT_FAILURE);
			}
		}
	}
}

#define BUF_LEN 64

/******************************************************************************
 *  Function:    serialize_player
 * 
 *  Date:        March 30, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   serialize_player(DPlaya *player, char *buf, int buflen)
 *		            DPlaya *player: Pointer to the DPlaya object to be serialized
 *                  char *buf: buffer to put the serialized form of the player into
 *					int buflen: the length of buf
 * 
 *  Returns:     0 on success or (-1) on failure.
 * 
 *  Description: Serialize player's data members into buf to be passed across the network.
 *
 *****************************************************************************/
int serialize_player(DPlaya *player, char *buf, size_t buflen) {
	if(buflen < sizeof(DPlaya)) {
		return -1;
	}
	
	memcpy(buf, player, sizeof(DPlaya));
	return 0;
}

/******************************************************************************
 *  Function:    unserialize_player
 * 
 *  Date:        March 30, 2009
 *
 *  Revisions:   
 * 
 *  Designer:    David Young
 *  Programmer:  David Young
 * 
 *  Interface:   unserialize_player(char *buf, DPlaya *player)
 *					char *buf: buffer to get the serialized form of the player from
 *		            DPlaya *player: Pointer to the DPlaya object to fill
 * 
 *  Returns:     0 on success or (-1) on failure.
 * 
 *  Description: Unserialize the info in buf and put it into player.
 *
 *****************************************************************************/
int unserialize_player(char *buf, DPlaya *player) {
	memcpy(player, buf, sizeof(DPlaya));
	return 0;
}
