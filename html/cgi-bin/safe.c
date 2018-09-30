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
#define FILE_DIR "/mnt/config/cfg/ip.txt"

struct _users_table
{	
	char ip[20];	
	char mask[20];
	char read[20];
	char write[20];
	char manage[20];
}Users[10];

struct sendbuff
{
	char buff[100];
}c_send[10];
void read_file(char *file_name)
{	
	FILE  *fp 		= NULL;
	int i;
	int j;
	int file_size;
	char send_rev[2];
	char line[100]={0};
	int count=0;
	char  find_str[50];
	char *p=NULL;
	char buff[20];
	char *ver_buffer[5]={"ip:","mask:","read:","write:","manage:"};
	char *ip_buffer[10]={"ip1","ip2","ip3","ip4","ip5","ip6","ip7","ip8","ip9","ip10"};
	char *mask_buffer[10]={"mask1","mask2","mask3","mask4","mask5","mask6","mask7","mask8","mask9","mask10"};
	char *read_buffer[10]={"read1","read2","read3","read4","read5","read6","read7","read8","read9","read10"};
	char *write_buffer[10]={"write1","write2","write3","write4","write5","write6","write7","write8","write9","write10"};
	char *manage_buffer[10]={"manage1","manage2","manage3","manage4","manage5","manage6","manage7","manage8","manage9","manage10"};
	cgiFormString("name",send_rev,sizeof(send_rev));	
	if(send_rev[0]=='1') 
	{
		if((fp=fopen(file_name,"r"))==NULL)
		{		
			printf("open file fail!\n");
			exit(1);
		}
		fseek(fp,0,SEEK_END);
		file_size=ftell(fp);	
		rewind(fp);	
		while (fgets(line,sizeof(line)-1,fp))
		{
			count++;
		}
		rewind(fp);	
		for(i=0;i<10;i++)
		{
			fgets(c_send[i].buff,100,fp);	
			for(j=0; j<5; j++)	
			{		
				if((p = strstr(c_send[i].buff,ver_buffer[j]))!=NULL)
				{			
					memset(find_str, 0, sizeof(find_str));
					sprintf(find_str ,"%s%s", ver_buffer[j],"%[^:]");
					sscanf(p, find_str, (char *)(&Users[i])+j*20);
				}
			}
			printf("%s:",Users[i].ip);	
			printf("%s:",Users[i].mask);
			printf("%s:",Users[i].read);
			printf("%s:",Users[i].write);
			printf("%s:",Users[i].manage);			
		}
	}
	else
	{
		if((fp=fopen(file_name,"w"))==NULL)
		{		
			printf("open file fail!\n");
			exit(1);
		}
		for(i=0;i<10;i++)
		{
			memset(buff,0, sizeof(buff));
			cgiFormString(ip_buffer[i],Users[i].ip,sizeof(Users[i].ip));
			printf("%s",Users[i].ip);
			strcat(Users[i].ip,":");	
			strcat(buff,ver_buffer[0]);
			strcat(buff,Users[i].ip);
			fputs(buff,fp);
			memset(buff,0, sizeof(buff));
			cgiFormString(mask_buffer[i],Users[i].mask,sizeof(Users[i].mask));			
			strcat(Users[i].mask,":");	
			strcat(buff,ver_buffer[1]);
			strcat(buff,Users[i].mask);
			fputs(buff,fp);
			memset(buff,0, sizeof(buff));		
			cgiFormString(read_buffer[i],Users[i].read,sizeof(Users[i].read));			
			strcat(Users[i].read,":");	
			strcat(buff,ver_buffer[2]);
			strcat(buff,Users[i].read);
			fputs(buff,fp);		
			memset(buff,0, sizeof(buff));
			cgiFormString(write_buffer[i],Users[i].write,sizeof(Users[i].write));			
			strcat(Users[i].write,":");	
			strcat(buff,ver_buffer[3]);
			strcat(buff,Users[i].write);
			fputs(buff,fp);	
			memset(buff,0, sizeof(buff));
			cgiFormString(manage_buffer[i],Users[i].manage,sizeof(Users[i].manage));			
			strcat(Users[i].manage,":\n");	
			strcat(buff,ver_buffer[4]);
			strcat(buff,Users[i].manage);
			fputs(buff,fp);			
		}				
	}
	close(fp);
}

int cgiMain(void)
{
	CGI_HEADER(  );
	read_file(FILE_DIR);
	return 0;
}