#include "open_source.h"
#define  SERVER_PORT 1001
int main(int argc, char** argv)
{
    int sockfd=-1;
    int send_count=0;
    char  buf[BUFFER_SIZE];
    socklen_t   len=sizeof(struct sockaddr_in);
    struct sockaddr_in  servaddr, cliaddr;

    if(sockfd==-1)
    {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sockfd==-1)
        {
            print_msg("create socket failue,%s\n",strerror(errno));
            exit (-1);
        }
        else
        {
            print_msg("create socket ok\n");
        }
        bzero(&servaddr, sizeof(struct sockaddr_in));
        servaddr.sin_family      = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port        = htons(SERVER_PORT);
        if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(struct sockaddr_in)) < 0)
        {
            print_msg("bind socket failue %s\n",strerror(errno));
        }
        else
        {
            print_msg("bind socket ok\n");
        }
    }
    len=sizeof(struct sockaddr_in);
    while(1)
    {
        memset(buf,0,4096);
        send_count=recvfrom(sockfd, buf,4096, 0, (struct sockaddr *)&cliaddr, &len);
        print_msg("[%d] [%d %s] [%s]\n",
                  send_count,cliaddr.sin_port,inet_ntoa(cliaddr.sin_addr),buf);
    }
}
