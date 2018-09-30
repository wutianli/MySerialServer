/* ************************************************************************
 *       Filename:  SysReboot.c
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

int cgiMain()
{
	CGI_HEADER(  );
	int fd = 0;
	char portt[2]="r";
	struct sockaddr_in server_addr;	
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
	send(fd,portt, 2, 0);	
	close(fd);
	return 0;
}


