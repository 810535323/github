/*./client serv_ip serv_port*/
#include "net.h"


void usage(char *s)
{
    printf("\n%s muticast_ip serv_port",s);
    printf("\n\t muticast_ip:muticast_ip address(between 224~239)");
    printf("\n\t muticast_port:server port(>5000)\n\n");
}

int main(int argc,char *argv[])
{

    int fd = -1;
    struct sockaddr_in sin;
    int port;
    /*创建fd用于保存socket返回值*/
    if(argc != 3)
    {
        usage(argv[0]);
        exit(1);
    }

    if((fd = socket(AF_INET,SOCK_DGRAM,0)) < 0 )
    {
        perror("socket");
        exit(1);
    }

    printf("connecting....");
    port = atoi(argv[2]);
    if(port < 5000)
    {
        usage(argv[0]);
        exit(1);
    }
    /*连接服务器 填充参数*/
    bzero(&sin,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
   // sin.sin_addr.s_addr = inet_addr(SERV_IP_ADDR);//只适用于IPV4
    printf("Success!\n");
    if(inet_pton(AF_INET, argv[1],(void *)&sin.sin_addr) != 1)
    {
        perror("inet_pton");
        exit(1);
    }
    char buf[BUFSIZE];
    while(1)
    {
        bzero(buf,BUFSIZE);
        if(fgets(buf,BUFSIZE,stdin)==NULL)
        {
            perror("fgets");
            continue;
        }

        sendto(fd,buf,strlen(buf),0,(struct sockaddr *)&sin,sizeof(sin));
        if( !strncasecmp(buf, QUIT_STR, strlen(QUIT_STR)))
        {
            printf("Client is exiting\n");
            break;
        }
    }
    return 0;
}

