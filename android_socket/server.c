/*请注意,本文件使用的编码格式是GBK,请不要用其他的编码保存该文件！！！*/
/*
 * 修改此文件时，请务必确定使用GBK编码保存此文件，否则中文显示可能会乱码
 *
 * E-mail:zhfubi@126.com
 *
 * Create Date:2011-12-26
*/

/*
双向网络传输测试程序服务器端.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <syslog.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>
#define print_msg printf
#define	PORT_LISTEN			5003

void    *thread_read_function (void *arg);
void    *thread_send_function (void *arg);
void    Sig_Handler (int signo);
void    Signal_Initial (void);

int     thread_read=0;
int     thread_send=0;
int     thread_int=0;
int     listensocket = -1;
char    buffer[65536];
int     bufferlen=65536;

int main (int argc, char **argv)
{
	int     incomingsocket = -1;
	struct sockaddr_in ListenSocketAddr;
	struct sockaddr_in IncomingSocketAddr;
	socklen_t     addresslen;
    int recv_count=0;

    Signal_Initial();
	if ((listensocket = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		print_msg ("can't create send socket [%s]\n",strerror(errno));
        exit (EXIT_FAILURE);
	}
	memset (&ListenSocketAddr, 0, sizeof (ListenSocketAddr));
	ListenSocketAddr.sin_family = AF_INET;
	ListenSocketAddr.sin_addr.s_addr = INADDR_ANY;
	ListenSocketAddr.sin_port = htons (PORT_LISTEN);
	if (bind (listensocket, (struct sockaddr *) &ListenSocketAddr, sizeof (ListenSocketAddr)) < 0)
	{
		print_msg ("can't bind port %d [%s]\n", PORT_LISTEN, strerror (errno));
		exit (EXIT_FAILURE);
	}
	if (listen (listensocket, 10) == -1)
	{
		print_msg ("can't listen port %d [%s]\n", PORT_LISTEN,strerror (errno));
		exit (EXIT_FAILURE);
	}
	while (1)
	{
		addresslen = sizeof (IncomingSocketAddr);
		print_msg("begin accept\n");
        if ((incomingsocket = accept (listensocket, (struct sockaddr *) &IncomingSocketAddr, &addresslen)) < 0)
		{
			if(thread_int)
            {
                break;
            }
            print_msg ("can't accept port %d [%s]\n", PORT_LISTEN,strerror (errno));
            continue;
		}
		print_msg("begin recv\n");
		recv_count=recv (incomingsocket, buffer, bufferlen, 0);
		print_msg("recv_count=%d\n",recv_count);
        if(thread_int)
        {
            break;
        }
    }
	shutdown (listensocket, SHUT_RDWR);
	close (listensocket);
	exit(EXIT_SUCCESS);
}




void Signal_Initial (void)
{
    struct sigaction SignalHandler;

    sigemptyset (&(SignalHandler.sa_mask));
    SignalHandler.sa_handler = SIG_IGN;
    SignalHandler.sa_flags = 0;

    SignalHandler.sa_handler = &Sig_Handler;

    sigaddset (&(SignalHandler.sa_mask), SIGPIPE);
    if (sigaction(SIGPIPE, &SignalHandler, NULL) != 0)
    {
        print_msg ("[%s %d:%s]\n",__FUNCTION__,__LINE__,strerror(errno));
        exit(EXIT_FAILURE);
    }

    sigaddset (&(SignalHandler.sa_mask), SIGABRT);
    if (sigaction (SIGABRT, &SignalHandler, NULL) != 0)
    {
        print_msg ("[%s %d:%s]\n",__FUNCTION__,__LINE__,strerror(errno));
    }

    sigaddset (&(SignalHandler.sa_mask), SIGTERM);
    if (sigaction (SIGTERM, &SignalHandler, NULL) != 0)
    {
        print_msg ("[%s %d:%s]\n",__FUNCTION__,__LINE__,strerror(errno));
    }

    sigaddset (&(SignalHandler.sa_mask), SIGINT);
    if (sigaction (SIGINT, &SignalHandler, NULL) != 0)
    {
        print_msg ("[%s %d:%s]\n",__FUNCTION__,__LINE__,strerror(errno));
    }

    sigaddset (&(SignalHandler.sa_mask), SIGSEGV);
    if (sigaction (SIGSEGV, &SignalHandler, NULL) != 0)
    {
        print_msg ("[%s %d:%s]\n",__FUNCTION__,__LINE__,strerror(errno));
    }

    sigaddset (&(SignalHandler.sa_mask), SIGILL);
    if (sigaction (SIGILL, &SignalHandler, NULL) != 0)
    {
        print_msg ("[%s %d:%s]\n",__FUNCTION__,__LINE__,strerror(errno));
    }

    //SignalHandler.sa_handler = SIG_IGN;
    sigaddset (&(SignalHandler.sa_mask), SIGBUS);
    if (sigaction (SIGBUS, &SignalHandler, NULL) != 0)
    {
        print_msg ("[%s %d:%s]\n",__FUNCTION__,__LINE__,strerror(errno));
    }
}

void Sig_Handler (int signo)
{
    switch(signo)
    {
      case SIGPIPE:
        //thread_int=1;
        print_msg("[catch signal SIGPIPE]\n");
        break;
      case SIGABRT:
        print_msg("[catch signal SIGABRT]\n");
        break;
      case SIGTERM:
        print_msg("[catch signal SIGTERM]\n");
        break;
      case SIGINT:
        thread_int=1;
        //print_msg("[catch signal SIGINT]\n");
        break;
      case SIGSEGV:
        print_msg("[catch signal SIGSEGV]\n");
        break;
      case SIGILL:
        print_msg("[catch signal SIGILL]\n");
        break;
      case SIGBUS:
        print_msg("[catch signal SIGBUS]\n");
        break;
      default:
        print_msg("[catch signal default]\n");
        break;
    }
}

