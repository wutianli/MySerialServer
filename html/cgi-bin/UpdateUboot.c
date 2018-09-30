#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>
#include <strings.h>
#include <sys/resource.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/reboot.h>
#include "cgic.h"
#include "cgibase.h"

#define  FILE_120k 	120*1024
#define  FILE_160k  160*1024
#define  BufferLen  1024
#define  DATAFILE  "/tmp/Uboot.bin"

unsigned int  crc_table[16]={ 
    0x0000, 0x1081, 0x2102, 0x3183, 
    0x4204, 0x5285, 0x6306, 0x7387, 
    0x8408, 0x9489, 0xa50a, 0xb58b, 
    0xc60c, 0xd68d, 0xe70e, 0xf78f 
}; 

unsigned int  do_local_crc(unsigned char *ptr,unsigned long int len,unsigned int crc) 
{ 

	unsigned char da=0; 

	while(len--!=0)
	{ 
		da = (unsigned char)(crc&0x000f); 
		crc>>= 4; 
		crc ^= crc_table[da^(*ptr&0x0f)]; 
		da = (unsigned char)(crc&0x000f); 
		crc>>= 4; 
		crc ^= crc_table[da^(*ptr/16)]; 
		ptr++;   
	}
	
	return(crc); 
} 

unsigned int  do_pc_crc(char *file_name) 
{

    unsigned int crc_pc =0;
	char *p1 = NULL;
	char *p2 = NULL;
	
	char number[4];
	
	/*VPC6467TDRV4.1_5678.gz*/
	if ((p1 = strrchr(file_name, '_')) != NULL && (p2 = strrchr(file_name, '.')) != NULL)
	{
		strncpy(number,p1+1,p2-p1-1);
		crc_pc = strtol(number,NULL,16);
	}
	else
	{
		return -1;
	}
	
	return crc_pc;
}

int cgiMain(void)
{ 
	CGI_HEADER( ); 
	int i;
	cgiFilePtr srcfile;
	mode_t 		 mode;
	int 		 objfile;
	int 		 filesize  = 0;
	int 		 getsize   = 0;
	unsigned int crc_local = 0;
	unsigned int crc_pc    = 0;
	
	char name[64];
	char *buffer;
	
	if(cgiFormFileName("U_Update", name, sizeof(name)) != cgiFormSuccess)
	{
		printf("{msg:'文件名字为空'\n}");
		return -1;
	}
	
	cgiFormFileSize("U_Update",&filesize);
	
	if((filesize < FILE_120k)||(filesize > FILE_160k))
	{
		printf("{msg:'请选择大小正确的文件'\n}");
		return -1;
	}
	
	if (cgiFormFileOpen("U_Update", &srcfile) != cgiFormSuccess) 
	{
		printf("{msg:'不能正常打开源文件'\n}");
		return -1;
	}
	
	mode = S_IRWXU|S_IRGRP|S_IROTH; 	
	objfile = open(DATAFILE, O_RDWR|O_CREAT|O_TRUNC|O_APPEND, mode); 
	if(objfile < 0)
	{ 
		printf("{msg:'创建文件成功'\n}");
		return -1;
	} 
	
	buffer = (char *)malloc(BufferLen);
	
	while (cgiFormFileRead(srcfile, buffer, BufferLen, &getsize) == cgiFormSuccess)
	{ 
		if(getsize > 0)
		{
			write(objfile,buffer,getsize); 
			crc_local = do_local_crc(buffer,getsize,crc_local);
			memset(buffer, 0, BufferLen);
		}
	}
	cgiFormFileClose(srcfile);
	close(objfile);
	free(buffer);
	
	if((crc_pc = do_pc_crc(name)) < 0)
	{
		printf("{msg:'获取CRC校验码失败'\n}");
		return -1;
	}

	if(crc_local == crc_pc)
	{
		system("/usr/sbin/flash_erase /dev/mtd0 0xC0000 2");
		system("/usr/sbin/nandwrite -p -q -s 0xC0000 /dev/mtd0 /tmp/Uboot.bin");
		system("/bin/rm /tmp/*");
		//system("/usr/bin/killall Mp2ToH264");
		printf("{msg:'升级Uboot成功,请手动重启设备'\n}\n");
		return 0;
	}
	else
	{
		system("/bin/rm /tmp/*");
		printf("{msg:'CRC校验失败 请重新升级Uboot'\n}");
		return -1;
	}

}

	

