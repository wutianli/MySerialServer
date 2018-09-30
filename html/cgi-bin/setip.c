#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include "cgibase.h"
#include "cgic.h"
#include "config.h"
#include <fcntl.h>
#define FILE_DIR "/mnt/config/cfg/rc.conf"
#define FILE_DIR2 "./ip/config.conf"
#define request '1'

struct _users_table
{	
	char ip[20];	
	char mask[20];
	char gateway[20];
}Users;

void read_file()
{
	FILE  *fp = NULL;
	int i;
	int j;
	int file_size;
	char *pfile=NULL;
	char *p=NULL;
	char send_rev[2];
	char line[100]={0};	
	char find_str[50];
	char buff[20];
	char *ver_buffer[3]={"IPADDR0=\"","NETMASK0=\"","GATEWAY0=\""};
	cgiFormString("name",send_rev,sizeof(send_rev));	
	
	/***************************************************************
	   处理WEB发送的请求数据命令	send_rev = request（1） 请求数据
	****************************************************************/
	if(send_rev[0]==request) 
	{
		if((fp=fopen(FILE_DIR,"r"))==NULL)
		{		
			printf("open ip.conf fail!\n");
			exit(1);
		}
		fseek(fp,0,SEEK_END);
		file_size=ftell(fp);	
		rewind(fp);	
		if((pfile = malloc(file_size)) == NULL)
		{
			printf("malloc fail!\n");
			exit(1);
		}	
		fread(pfile,file_size,1,fp);
		for(i=0;i<3;i++)
		{
			if((p = strstr(pfile,ver_buffer[i]))!=NULL)
			{			
				memset(find_str, 0, sizeof(find_str));
				sprintf(find_str ,"%s%s", ver_buffer[i],"%[^\"]");
				sscanf(p, find_str, (char *)(&Users)+i*20);
			}			
		}
		fclose(fp);
		/****************发送数据************/
		printf("%s.",Users.ip);
		printf("%s.",Users.mask);
		printf("%s.",Users.gateway);
	}
	/***************************************************************
					处理WEB发送的修改数据命令	
	****************************************************************/
	else
	{
		FILE  *fp 		= NULL;
		FILE  *fp1 		= NULL;
		char line[100];
		char buff[10];
		char *ip_buff[4]={"ip01","ip02","ip03","ip04"};
		char *mask_buff[4]={"subnet01","subnet02","subnet03","subnet04"};
		char *gateway_buff[4]={"gateway01","gateway02","gateway03","gateway04"};
		int i = 0,j = 0;
		int fd = 0;
		char portt[2] = "s";
	
		memset(Users.ip,0, sizeof(Users.ip));
		for(i=0;i<4;i++)
		{
			if(i!=3)
			{
				memset(buff,0, sizeof(buff));
				cgiFormString(ip_buff[i],buff,sizeof(buff));
				strcat(Users.ip,buff);
				strcat(Users.ip,".");
			}
			else
			{
				memset(buff,0, sizeof(buff));
				cgiFormString(ip_buff[i],buff,sizeof(buff));
				strcat(Users.ip,buff);			
			}
		}
		for(i=0;i<4;i++)
		{
			if(i!=3)
			{
				memset(buff,0, sizeof(buff));
				cgiFormString(mask_buff[i],buff,sizeof(buff));
				strcat(Users.mask,buff);
				strcat(Users.mask,".");
			}
			else
			{
				memset(buff,0, sizeof(buff));
				cgiFormString(mask_buff[i],buff,sizeof(buff));
				strcat(Users.mask,buff);				
			}
			
		}
		for(i=0;i<4;i++)
		{
			if(i!=3)
			{
				memset(buff,0, sizeof(buff));
				cgiFormString(gateway_buff[i],buff,sizeof(buff));
				strcat(Users.gateway,buff);
				strcat(Users.gateway,".");
			}
			else
			{
				memset(buff,0, sizeof(buff));
				cgiFormString(gateway_buff[i],buff,sizeof(buff));
				strcat(Users.gateway,buff);				
			}
			
		}
		printf("ip = %s",Users.ip);
		printf("mask = %s",Users.mask);
		printf("gateway = %s",Users.gateway);
		system("ifconfig eth0 192.168.1.10");
		if((fp=fopen(FILE_DIR,"w"))==NULL)
		{		
			printf("open ip.conf fail!\n");
			exit(1);
		}
		if((fp1=fopen(FILE_DIR2,"r+"))==NULL)
		{		
			printf("open config.conf fail!\n");
			exit(1);
		}
		i=0;
		while(!feof(fp1))   
		{
			if(i!=26&&i!=27&&i!=29)
			{
				memset(line,0, sizeof(line));
				fgets(line,sizeof(line)-1,fp1);
				fputs(line,fp);
			}				
			else
			{
				switch(i)
				{
					case 26:				
						memset(line,0, sizeof(line));
						fgets(line,sizeof(line)-1,fp1);
						while(Users.ip[j]!='\0')
						{
							line[16+j]=Users.ip[j];
							j++;
						}
						line[16+j]='"';
						line[17+j]='\n';
						line[18+j]='\0';
						j=0;
						fputs(line,fp);
						break;
					case 27:				
						memset(line,0, sizeof(line));
						fgets(line,sizeof(line)-1,fp1);
						while(Users.mask[j]!='\0')
						{
							line[17+j]=Users.mask[j];
							j++;
						}
						line[17+j]='"';
						line[18+j]='\n';
						line[19+j]='\0';
						fputs(line,fp);
						j=0;
						break;
					case 29:
						memset(line,0, sizeof(line));
						fgets(line,sizeof(line)-1,fp1);
						while(Users.gateway[j]!='\0')
						{
							line[17+j]=Users.gateway[j];
							j++;
						}
						line[17+j]='"';
						line[18+j]='\n';
						line[19+j]='\0';
						fputs(line,fp);
						break;
				}
			}
			i++;
		}
		fclose(fp);
		fclose(fp1);
		usleep(1000);
		/***************发送修改IP的状态******************/
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
		}	
		
		send(fd,portt, 2, 0);	
		
		close(fd);
	}
	
}

int cgiMain(void)
{
	CGI_HEADER(  );
	read_file();
	return 0;
}

