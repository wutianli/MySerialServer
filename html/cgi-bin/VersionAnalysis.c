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

#define FILE_DIR    "/mnt/config/cfg/version.txt"

/*----------?????¡¤???----------*/
typedef struct _tagVersion
{	
	char 	uboot_version[10];	
	char 	kernel_version[10];	
	char 	program_version[10];	
	char 	ramdisk_version[10];
}
Version_Obj;


/**********************************************************************
 * ????¨² read_file
 * ????¨² ??¦Ó???? ??malloc????????
 * ????¨² file_name - ????¦Ó??¡¤?
 * ????¨² ?
 * ? ? ?¨² malloc??????
 * ???¨·        ???     ???	      ????
 * ---------------------------------------------------
 * 2013/05/10	     V1.0	  ???	      ??
 ***********************************************************************/

char *read_file(char *file_name)
{	
	FILE  *fp 		= NULL;
	char  *pfile	= NULL;
	unsigned long int file_size = 0;
	
	if((fp=fopen(file_name,"r"))==NULL)
	{		
		printf("open file fail!\n");
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
	
	fclose(fp);
	
	return pfile;
}


/**********************************************************************
 * ????¨² analysis
 * ????¨² ?¦Ó?????  ?????????Version_Obj????
 * ????¨² *pfile    -¦Ó???????
			  *pversion -Version_Obj?????
 * ????¨² ?
 * ? ? ?¨² ?
 * ???¨·        ???     ???	      ????
 * ---------------------------------------------------
 * 2013/05/10	     V1.0	  ???	      ??
 ***********************************************************************/

void analysis(char *pfile,Version_Obj *pversion)
{	
	int   i = 0;	
	char *p = NULL;	
	char *ver_buffer[4]={"uboot_version:","kernel_version:","program_version:","ramdisk_version:"};
	char  find_str[50];
	
	for(i=0; i<4; i++)	
	{		
		
		if((p = strstr(pfile,ver_buffer[i]))!=NULL)
		{
			
			memset(find_str, 0, sizeof(find_str));
			sprintf(find_str ,"%s%s", ver_buffer[i],"%[^\n]");
			
			sscanf(p, find_str, (char *)pversion + i * 10);
			//printf("%s\n",(char *)pversion + i * 10);
		}
	}
}

int cgiMain(void)
{
	CGI_HEADER(  );
	char *pfile=NULL;   	
	Version_Obj  Version;

	if((pfile = read_file(FILE_DIR)) != NULL)
	{
		memset(&Version,0,sizeof(Version_Obj));

		analysis(pfile,&Version);
		if(pfile != NULL)
		{
			free(pfile);
			pfile = NULL;
		}
		
	}
	/*************uboot_version**************/
	printf("%s:\n",Version.uboot_version);
	/*************kernel_version*************/
	printf("%s:\n",Version.kernel_version);
	/*************program_version************/
	printf("%s:\n",Version.program_version);
	/*************ramdisk_version************/
	printf("%s:\n",Version.ramdisk_version);

}
