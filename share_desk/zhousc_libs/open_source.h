#define _LARGEFILE64_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <ctype.h>
#include <linux/if_packet.h>
#include <pthread.h>

#define BUFFER_SIZE 4096
#define TRUE   1
#define FALSE  0
#define MSG_SIZE 1024
#define MSG_COUNT 10

extern int g_external_data_flag;
extern char g_external_data[MSG_SIZE];

#define print_msg(...) open_print_debug(__FUNCTION__,__LINE__,## __VA_ARGS__)
void open_print_debug (const char *func,size_t line,const char *fmt, ...);

void send_broadcast(char *device,int port,char *msg,int len);
void recv_broadcast(int *port);
void clear_data(void);


struct Files {
    int fd;
    char Buffer[2048];
    int  Decryptor_Enable;
    int  CachePtr;
    int  CacheFullness;
    long long CurrentFilePtr;
    long long VirtualFilePtr;
    char XORValue[4];
    long long File_Size;
};

int fd_open(struct Files * file,char *path);
int fd_read(struct Files *file, void *buf, size_t length);
long long fd_seek(struct Files *file, long long pos, int whence);
void fd_close(struct Files *file);
