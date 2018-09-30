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
#define FILE_DIR "/mnt/config/cfg/port1.txt"


void read_file(char *file_name)
{	
	int fp;
	char baudrate[8];
	char databit[2];
	char stopbit[2];
	char parity[2];
	char ip_add[16];
	char portnumbd[6];
	char portnumyc[6];
	char work_id[2];
	char ip_limit[2];
	char port_type[2];
	int i;
	char send_rev[2];
	cgiFormString("name",send_rev,sizeof(send_rev));
	if(send_rev[0]=='1')
	{
		fp = open(file_name,O_RDONLY);
		/*******波特率******/
		int r_b = read(fp,baudrate,8);
		if(r_b < 0)
		{
			perror("read baud");
			exit(1);
		}
		memset(databit,0,sizeof(databit));
		/*******数据位******/
		lseek(fp,8,0);
		int r_d = read(fp,databit,2);
		if(r_d < 0)
		{
			perror("read databit");
			exit(1);
		}
		/*******停止位******/
		lseek(fp,10,0);
		int r_s = read(fp,stopbit,1);
		if(r_s < 0)
		{
			perror("read stopbit");
			exit(1);
		}
		/*******校验位******/
		lseek(fp,12,0);
		int r_p = read(fp,parity,1);
		if(r_p < 0)
		{
			perror("read parity");
			exit(1);
		}
		/*******IP地址******/
		lseek(fp,14,0);
		int r_ip = read(fp,ip_add,15);
		if(r_ip < 0)
		{
			perror("read dip");
			exit(1);
		}
		/*******本地端口号******/
		lseek(fp,30,0);
		int r_port = read(fp,portnumbd,5);
		if(r_port < 0)
		{
			perror("read portdata");
			exit(1);
		}
		/*******远程端口号******/
		lseek(fp,36,0);
		int r_f = read(fp,portnumyc,6);
		if(r_f < 0)
		{
			perror("read flag");
			exit(1);
		}
		/*******工作模式******/
		lseek(fp,42,0);
		 r_f = read(fp,work_id,1);
		if(r_f < 0)
		{
			perror("read flag");
			exit(1);
		}
		/*******IP限制模式******/
		lseek(fp,44,0);
		r_f = read(fp,ip_limit,1);
		if(r_f < 0)
		{
			perror("read flag");
			exit(1);
		}
		
		lseek(fp,46,0);
		r_f = read(fp,port_type,1);
		if(r_f < 0)
		{
			perror("read type");
			exit(1);
		}
		close(fp);
		databit[1]  = '\0';	
		stopbit[1]  = '\0';
		baudrate[7] = '\0';
		parity[1]   = '\0';
		ip_add[15]  = '\0';
		portnumbd[5]  = '\0';
		portnumyc[5]  ='\0';	
		work_id[1]  ='\0';
		ip_limit[1] ='\0';
		port_type[1] = '\0';
		//向WEB前端发送信息
		printf("%s:",baudrate);
		printf("%s:",databit);
		printf("%s:",stopbit);
		printf("%s:",parity);
		printf("%s:",ip_add);
		printf("%s:",portnumbd);
		printf("%s:",portnumyc);
		printf("%s:",work_id);
		printf("%s:",ip_limit);
		printf("%s:",port_type);
	}
	else
	{
		memset(baudrate, ' ',  sizeof(baudrate));
		memset(databit, ' ',   sizeof(databit));
		memset(stopbit, ' ',   sizeof(stopbit));
		memset(work_id, ' ',   sizeof(work_id));
		memset(ip_add, ' ',    sizeof(ip_add));
		memset(portnumbd, ' ', sizeof(portnumbd));
		memset(portnumyc, ' ', sizeof(portnumyc));
		memset(ip_limit, ' ',  sizeof(ip_limit));
		memset(port_type, ' ',  sizeof(port_type));
		
		cgiFormString("port", baudrate, sizeof(baudrate));
		cgiFormString("sjw", databit, sizeof(databit));
		cgiFormString("tzw", stopbit, sizeof(stopbit));
		cgiFormString("jyw", parity, sizeof(parity));
		cgiFormString("work", work_id, sizeof(work_id));
		cgiFormString("ip_add", ip_add, sizeof(ip_add));
		cgiFormString("portnumbd", portnumbd, sizeof(portnumbd));
		cgiFormString("portnumyc", portnumyc, sizeof(portnumyc));
		cgiFormString("ip_limit", ip_limit, sizeof(ip_limit));
		cgiFormString("type", port_type, sizeof(port_type));		
		fp = open(file_name,O_WRONLY);
		if(fp<0)
		{
			printf("open fail\n");
		
		}
		/*******波特率******/
		lseek(fp,0,0);
		//strncpy(baudrateCpy,baudrate,strlen(baudrate));
		printf("----ip_add = %s----\n",ip_add);
		int r_b = write(fp,baudrate,sizeof(baudrate));
		if(r_b < 0)
		{
			printf("%s",baudrate);
			exit(1);
		}
		lseek(fp,8,0);
		r_b = write(fp,databit,1);
		if(r_b < 0)
		{
			printf("%s",databit);
			exit(1);
		}
		lseek(fp,10,0);
		r_b = write(fp,stopbit,1);
		if(r_b < 0)
		{
			printf("%s",stopbit);
			exit(1);
		}
		lseek(fp,12,0);
		r_b = write(fp,parity,1);
		if(r_b < 0)
		{
			printf("%s",parity);
			exit(1);
		}
		lseek(fp,14,0);
		r_b = write(fp,ip_add,15);
		if(r_b < 0)
		{
			printf("%s",parity);
			exit(1);
		}
		lseek(fp,30,0);
		r_b = write(fp,portnumbd,5);
		if(r_b < 0)
		{
			printf("%s",portnumbd);
			exit(1);
		}
		lseek(fp,36,0);
		r_b = write(fp,portnumyc,6);
		if(r_b < 0)
		{
			printf("%s",portnumyc);
			exit(1);
		}
		lseek(fp,42,0);
		r_b = write(fp,work_id,1);
		if(r_b < 0)
		{
			printf("%s",work_id);
			exit(1);
		}
		lseek(fp,44,0);
		r_b = write(fp,ip_limit,1);
		if(r_b < 0)
		{
			printf("%s",ip_limit);
			exit(1);
		}
		lseek(fp,46,0);
		r_b = write(fp,port_type,1);
		if(r_b < 0)
		{
			printf("%s",port_type);
			exit(1);
		}
	}
}

int cgiMain(void)
{
	CGI_HEADER(  );
	read_file(FILE_DIR);
	return 0;
}