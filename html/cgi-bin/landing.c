#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "cgibase.h"
#include "cgic.h"
#define FILE_DIR "/mnt/config/cfg/key.txt"



int cgiMain(void)
{
	char account_buf[1024];
	char password_buf[1024];
	char account[1024];
	char password[1024];
	int i,j;
	CGI_HEADER();
	FILE  *fp 		= NULL;
	char  *pfile	= NULL;
	unsigned long int file_size = 0;
	
	if((fp=fopen(FILE_DIR,"rb"))==NULL)
	{		
		printf("open file fail!\n");
		exit(1);
	}
	fseek(fp,0,SEEK_END);
	file_size=ftell(fp);	
	rewind(fp);	
	if((pfile = malloc(file_size+1)) == NULL)
	{
		printf("malloc fail!\n");
		exit(1);
	}	
	fread(pfile,file_size,1,fp);
	pfile[file_size] = '\0';
	fclose(fp);
	cgiFormString("account", account_buf, sizeof(account_buf)-1);
	cgiFormString("password", password_buf, sizeof(password_buf)-1);
	sscanf(pfile, "%[^:]", account);
	sscanf(pfile, "%*[^:]:%[^@]", password);
	i=strcmp(account,account_buf);
	j=strcmp(password,password_buf);
	if(i==0&&j==0)
	{
		printf("1:%d\n",i);	
	}
	else
	{
		printf("0:%d\n",i);	
	}
	free(pfile);
}