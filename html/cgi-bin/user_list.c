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
#define FILE_DIR "./user/name.txt"

struct _users_table
{	
	char name[40];	
	char password[10];
	char r_port[3];
	char w_port[3];
	char manage[3];
	char phone[12];
}Users[10];

struct sendbuff
{
	char buff[40];
}c_send[10];
void read_file(char *file_name)
{	
	FILE  *fp 		= NULL;
	int i;
	int file_size;
	char send_rev[2];
	char line[100]={0};
	int count=0;

	char *name_buffer[10]={"name1","name2","name3","name4","name5","name6","name7","name8","name9","name10"};
	char *password_buffer[10]={"password1","password2","password3","password4","password5","password6","password7","password8","password9","password10"};
	char *r_port_buffer[10]={"r_port1","r_port2","r_port3","r_port4","r_port5","r_port6","r_port7","r_port8","r_port9","r_port10"};
	char *w_port_buffer[10]={"w_port1","w_port2","w_port3","w_port4","w_port5","w_port6","w_port7","w_port8","w_port9","w_port10"};
	char *manage_buffer[10]={"manage1","manage2","manage3","manage4","manage5","manage6","manage7","manage8","manage9","manage10"};
	char *phone_buffer[10]={"phone1","phone2","phone3","phone4","phone5","phone6","phone7","phone8","phone9","phone10"};
	//char *send_buff[]={"buff1[40]","buff2[40]","buff3[40]","buff4[40]","buff5[40]","buff6[40]","buff7[40]","buff8[40]","buff9[40]","buff10[40]"};
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
		printf("%d:",count);
		for(i=0;i<count;i++)
		{
			fgets(c_send[0].buff,39,fp);
			printf("%s",c_send[0].buff);
			
		}

	}
	else
	{
		if((fp=fopen(file_name,"w"))==NULL)
		{		
			printf("open file fail!\n");
			exit(1);
		}
		//接收网页数据处理
		for(i=0;i<10;i++)
		{
			cgiFormString(name_buffer[i],Users[i].name,sizeof(Users[i].name));
			cgiFormString(password_buffer[i],Users[i].password,sizeof(Users[i].password));
			cgiFormString(r_port_buffer[i],Users[i].r_port,sizeof(Users[i].r_port));
			cgiFormString(w_port_buffer[i],Users[i].w_port,sizeof(Users[i].w_port));
			cgiFormString(manage_buffer[i],Users[i].manage,sizeof(Users[i].manage));
			cgiFormString(phone_buffer[i],Users[i].phone,sizeof(Users[i].phone));
			
			strcat(Users[i].name,":");	
			strcat(Users[i].password,":");
			strcat(Users[i].r_port,":");
			strcat(Users[i].w_port,":");
			strcat(Users[i].manage,":");
			strcat(Users[i].phone,":\n");
			strcat(Users[i].name,Users[i].password);	
			strcat(Users[i].name,Users[i].r_port);
			strcat(Users[i].name,Users[i].w_port);
			strcat(Users[i].name,Users[i].manage);
			strcat(Users[i].name,Users[i].phone);
			fputs(Users[i].name,fp);
			
		}
		//写入文件
		/*
		fputs(Users[0].name,fp);
		fputs(Users[1].name,fp);
		*/
	}
	printf("%s",Users[0].name);
	printf("%s",Users[1].name);
	close(fp);
}

int cgiMain(void)
{
	CGI_HEADER(  );
	read_file(FILE_DIR);
	return 0;
}