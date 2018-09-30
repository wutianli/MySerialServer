/* ************************************************************************
 *       Filename:  client.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2014年3月31日14:09:07
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  Helong 
 *        Company:  
 * ************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "cgibase.h"
#include "cgic.h"
#include "config.h"
struct PORT_
{
	char buff[3];
}port[8];
int cgiMain()
{
	
	int fd = 0;
	char buf[100]="";
	char a[10]={'1','2'};
	int i;
	char portt[9];
	portt[0] = 'p';
	struct sockaddr_in server_addr;
	CGI_HEADER(  );
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9999);
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if( fd < 0 )
	{
		perror("socket");
		exit(1);
	}
	
	if (connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) !=0)
	{
		perror("connect");
		exit(1);
	}
	/***********************接受WEB数据****************************/
	cgiFormString("serial_1",port[0].buff,sizeof(port[0].buff));
	cgiFormString("serial_2",port[1].buff,sizeof(port[1].buff));
	cgiFormString("serial_3",port[2].buff,sizeof(port[2].buff));
	cgiFormString("serial_4",port[3].buff,sizeof(port[3].buff));
	cgiFormString("serial_5",port[4].buff,sizeof(port[4].buff));
	cgiFormString("serial_6",port[5].buff,sizeof(port[5].buff));
	cgiFormString("serial_7",port[6].buff,sizeof(port[6].buff));
	cgiFormString("serial_8",port[7].buff,sizeof(port[7].buff));
	
	for(i=0;i<8;i++)
	{
		if(port[i].buff[0]=='o')
		{
			portt[i + 1] = '1';
		}
		else
			portt[i + 1] = '0';
		
	}
	send(fd,portt, 9, 0);	
	close(fd);
	return 0;
}


