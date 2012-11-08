#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define PORT_LISTEN         15551
#define SERVER_IP_ADDRESS   "192.168.2.11"


int main (void)
{
    int     sendsocket = -1;
    struct sockaddr_in SendSocketAddr;
    char    buffer[65536];
    int     bufferlen=0;
    char *teststr="abcd\n";
    int send_count=0;

    if (sendsocket != -1)
    {
        shutdown (sendsocket, SHUT_RDWR);
        close (sendsocket);
    }
    if ((sendsocket = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf (stderr, "Can't create send socket\n");
        exit (EXIT_FAILURE);
    }
    memset (&SendSocketAddr, 0, sizeof (SendSocketAddr));
    SendSocketAddr.sin_family = AF_INET;
    SendSocketAddr.sin_addr.s_addr = inet_addr ("192.168.2.11");
    SendSocketAddr.sin_port = htons (PORT_LISTEN);

	printf ("=============>> %08X\n",inet_addr ("192.168.2.11"));
	
    while (connect (sendsocket, (struct sockaddr *) &SendSocketAddr, sizeof (SendSocketAddr)) == -1)
    {
        printf ("Can't connect\n");
        usleep (100000);
    }
    memset (buffer, 0, 65536);
    //bufferlen=strlen(teststr);
    //printf("bufferlen=%d\n",bufferlen);
    bufferlen=65536;
    //while ((send_count=send (sendsocket, buffer, bufferlen, 0)) > 0)
    //{
    //    ;
    //}
    while ((send_count=recv (sendsocket, buffer, bufferlen, 0)) > 0)
    {
        ;//printf("recv_count=%d %s\n",recv_count,buffer);
    }
    printf ("Can't send\n");
    close(sendsocket);
    printf("send_count=%d\n",send_count);
    return 0;
}