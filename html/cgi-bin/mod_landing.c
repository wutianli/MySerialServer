#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "cgibase.h"
#include "cgic.h"
int cgiMain(void)
{
	char account_buf[1024];
	char password_buf[1024];
	char account[1024];
	char password[1024];
	char buff[1024];
	int i,j;
	CGI_HEADER();
	FILE  *fp 		= NULL;
	char  *pfile	= NULL;
	unsigned long int file_size = 0;
	
	if((fp=fopen("/mnt/config/cfg/key.txt","wb+"))==NULL)
	{		
		printf("0:%d\n",i);	
		exit(1);
	}
	cgiFormString("account", account_buf, sizeof(account_buf)-1);
	cgiFormString("password", password_buf, sizeof(password_buf)-1);
	i=sprintf(buff,"%s:%s@",account_buf,password_buf);
	fwrite(buff,strlen(buff),1,fp);
	printf("1:%d\n",i);	
	rewind(fp);	
}