/*请注意,本文件使用的编码格式是GBK,请不要用其他的编码保存该文件！！！*/
/*
 * 修改此文件时，请务必确定使用GBK编码保存此文件，否则中文显示可能会乱码
 *
 * E-mail:zhfubi@126.com
 * 
 * Create Date:2011-12-26
*/
/*
双向网络传输测试程序客户端.
*/
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

#define	PORT_LISTEN			10000
#define print_msg printf
void    *thread_read_function (void *arg);
void    *thread_send_function (void *arg);
void    Sig_Handler (int signo);
void    Signal_Initial (void);


int thread_read=0;
int thread_send=0;
int thread_int=0;
char    buffer[65536];
int     bufferlen=0;
int main (int argc,char *argv[])
{
    int     sendsocket=-1;
    struct sockaddr_in socket_addr;    
    pthread_attr_t thread_attr;
    pthread_t ThreadID = 0;
    
    
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
    memset (buffer, 0, 65536);
    bufferlen=65536;
    pthread_attr_init (&thread_attr);
    if (pthread_create (&ThreadID, &thread_attr, thread_send_function, &sendsocket) != 0)
    {
        print_msg ("can't create pthread [%s]\n",strerror(errno));
    }
    
    pthread_attr_init (&thread_attr);
    if (pthread_create (&ThreadID, &thread_attr, thread_read_function, &sendsocket) != 0)
    {
        print_msg ("can't create pthread [%s]\n",strerror(errno));
    }
    
    while(thread_send==0 || thread_send==0)
    {
        usleep(1000);
    }
    print_msg ("end of test\n");
    close(sendsocket);
	return 0;
}


void   *thread_read_function (void *arg)
{
    int     socket=*(int *)arg;
    int     send_count=0;
    print_msg("begin to send\n");
    while ((send_count=send (socket, buffer, bufferlen, 0)) > 0)
    {
        if(thread_int)
        {
            break;
        }
        //sleep(1);
    }    
    //print_msg("end to send [%s]\n",strerror(errno));
    thread_read=1;
    pthread_exit (NULL);
}
void   *thread_send_function (void *arg)
{   
    int     socket=*(int *)arg;
    int     recv_count=0;
    print_msg("begin to recv\n");
    while ((recv_count=recv (socket, buffer, bufferlen, 0)) > 0)
    {
        if(thread_int)
        {
            break;
        }
        //sleep(1);
    }
    //print_msg("end to recv [%s]\n",strerror(errno));
    thread_send=1;
    pthread_exit (NULL);
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