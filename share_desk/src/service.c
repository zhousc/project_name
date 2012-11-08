#include "source.h"
#define PORT 9200

void broadcast(char *buffer);
int main(int argc,char *argv[])
{
    if(argc != 2)
    {
        print_msg("Usage: %s <eth0>\n",argv[0]);
    }
    else
    {
        broadcast(argv[1]);
    }
    return 0;
}
char *msg="exx";
void broadcast(char *buffer)
{
    struct ifreq ifq;
    struct timeval timeout={3,0};//修改超时时间.
    char broadcast_ip[16];
    int sockfd=-1;
    int tmp=0;
    int len=0;
    int so_broadcast=1;
    int send_count=0;
    struct sockaddr_in  cliaddr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd==-1)
    {
        print_msg("create socket failue,%s\n",strerror(errno));
        return;
    }
    print_msg("network device : [%s]\n",buffer);
    strcpy (ifq.ifr_name, buffer);
    if (ioctl (sockfd, SIOCGIFADDR, &ifq) < 0) {
        print_msg("ioctl failue,%s\n",strerror(errno));
    }
    snprintf (broadcast_ip, 16,"%s",
    inet_ntoa(((struct sockaddr_in*)&(ifq.ifr_addr))->sin_addr));
    tmp=strlen(broadcast_ip)-1;
    print_msg("broadcast_ip=%s\n",broadcast_ip);
    while(1)
    {
        if(broadcast_ip[tmp] != '.')
        {
            broadcast_ip[tmp]=0;
        }
        else
        {
            break;
        }
        tmp--;
    }
    tmp++;
    broadcast_ip[tmp++]='2';
    broadcast_ip[tmp++]='5';
    broadcast_ip[tmp++]='5';
    broadcast_ip[tmp]=0;

    cliaddr.sin_family      = AF_INET;
    cliaddr.sin_addr.s_addr = inet_addr (broadcast_ip);
    cliaddr.sin_port        = htons(PORT);

    print_msg("broadcast_ip=%s\n",broadcast_ip);
    if(setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&so_broadcast,sizeof(so_broadcast)) < 0)
    {
        print_msg("%s\n",strerror(errno));
    }
    if(setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout)) < 0)
    {
        print_msg("%s\n",strerror(errno));
    }
    if(setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout)) < 0)
    {
        print_msg("%s\n",strerror(errno));
    }
    len=sizeof(struct sockaddr_in);

    send_count=sendto(sockfd,msg,strlen(msg), 0, (struct sockaddr *)&cliaddr, len);
    if(send_count != strlen(msg))
    {
        print_msg("%s\n",strerror(errno));
    }
    else
    {
        print_msg("send %d,%s\n",strlen(msg),buffer);
    }
    close(sockfd);
}

