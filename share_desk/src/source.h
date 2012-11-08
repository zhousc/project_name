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


#define print_msg(...) print_debug(__FUNCTION__,__LINE__,## __VA_ARGS__)
void print_debug (const char *func,size_t line,const char *fmt, ...);
#define BUFFER_SIZE 4096
#define TRUE   1
#define FALSE  0
