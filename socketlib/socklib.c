/*
 *	socklib.c
 *
 *	This file contains functions used lots when writing internet
 *	client/server programs.  The two main functions here are:
 *
 *	make_server_socket( portnum )	returns a server socket
 *					or -1 if error
 *      make_server_socket_q(portnum,backlog)
 *
 *	connect_to_server(char *hostname, int portnum)
 *					returns a connected socket
 *					or -1 if error
 */

#include	<stdio.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include	<time.h>
#include	<strings.h>
#include    <errno.h>

#include    "socklib.h"

#define   HOSTLEN  256
#define	  BACKLOG  1

	//int listenfd, connfd;
	//struct sockaddr_in servaddr;
	//char buff[4096];
	//int n;


int make_server_socket(int portnum)
{
	return make_server_socket_q(portnum, BACKLOG);
}
int make_server_socket_q(int portnum, int backlog)
{
	int listenfd;
	struct sockaddr_in servaddr;


	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		//exit(0);
		return -1;
	}

	/** build address and bind it to socket **/
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY );
	servaddr.sin_port = htons(7080);

	if (bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1)
	{
		printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
		//exit(0);
		return -1;
	}


	/** arrange for incoming calls **/
	if (listen(listenfd, 10) == -1)
	{
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
		//exit(0);
		return -1;
	}

	return listenfd;
}

int connect_to_server(char *host, int portnum)
{
	int sock;
	struct sockaddr_in  servadd;        /* the number to call */
	struct hostent      *hp;            /* used to get number */

	/** Step 1: Get a socket **/

	sock = socket( AF_INET, SOCK_STREAM, 0 );    /* get a line   */
	if ( sock == -1 )
		return -1;

	/** Step 2: connect to server **/

	bzero( &servadd, sizeof(servadd) );     /* zero the address     */
	hp = gethostbyname( host );             /* lookup host's ip #   */
	if (hp == NULL)
		return -1;
	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(portnum);      /* fill in port number  */
	servadd.sin_family = AF_INET ;          /* fill in socket type  */

	if ( connect(sock,(struct sockaddr *)&servadd, sizeof(servadd)) !=0)
	       return -1;

	return sock;
}
