
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

#define	PORT_LISTEN			5000
#define print_msg printf
void    *thread_read_function (void *arg);
void    *thread_send_function (void *arg);
void    Sig_Handler (int signo);
void    Signal_Initial (void);
  
            
int thread_read=0;
int thread_send=0;
int thread_int=0;
char    buffer[65536*2]={1,2,3,4,5,6,7,8,9,-2,0xfe};
int     bufferlen=0;
int main (int argc,char *argv[])
{
    int     sendsocket=-1;
	int     send_count=0;
    struct sockaddr_in socket_addr;
	int loop=0;

    if(argc != 2)
    {
        print_msg("%s IP\n",argv[0]);
        return 0;
    }
    Signal_Initial();

    sendsocket = socket (AF_INET, SOCK_STREAM, 0);
    if (sendsocket == -1){
        print_msg ("can't create send socket [%s]\n",strerror(errno));
        exit (EXIT_FAILURE);
    }
    memset (&socket_addr, 0, sizeof (socket_addr));
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_addr.s_addr = inet_addr (argv[1]);
    socket_addr.sin_port = htons (PORT_LISTEN);
    if (connect (sendsocket, (struct sockaddr *) &socket_addr, sizeof (socket_addr)) == -1)
    {
        print_msg ("can't connect %s [%s]\n",argv[1],strerror(errno));
        exit (EXIT_FAILURE);
    }
    //memset (buffer, 0, 65536);
	//while(1)
	//{
	  bufferlen=strlen(buffer);
	  print_msg ("bufferlen = %d\n",bufferlen);
	  {
		unsigned int kk=0x1fabcdef;
	  send_count=send (sendsocket, &kk, sizeof(int), 0);
	  print_msg("kk = %d\n",kk);
	  }
	  print_msg ("send_count = %d\n",send_count);
	  //sleep(1);
	  loop++;

	  //if(loop > 3) break;
	//}

    close(sendsocket);
	return 0;
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
        thread_int=1;
        //print_msg("[catch signal SIGPIPE]\n");
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