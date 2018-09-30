#include<stdio.h>
#include<stdlib.h>
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

#define   TEXT_LENGTH    5


int cgiMain(void)
{
	CGI_HEADER(  );
	
	int  	flag=0;
	int     err_no = 0;
	int     result;
	char 	data_buf[TEXT_LENGTH];
	char 	msg_sendbuf[400];
	char    msg_recvbuf[MSGBUFSIZE];

	char    *ver_buffer[2]={"0","1"};
	
	SourceModeConfig_Obj  *sourcemode=NULL;
	sourcemode  = (SourceModeConfig_Obj *)malloc(sizeof(SourceModeConfig_Obj));
	if(sourcemode == NULL)
	{
		exit(1);
	}
	
	memset(data_buf, 0, sizeof(data_buf));
	memset(msg_sendbuf, 0, sizeof(msg_sendbuf));
	memset(msg_recvbuf, 0, sizeof(msg_recvbuf));	
	
	if(err_no = cgiFormString("name", data_buf, sizeof(data_buf)-1))
	{
		flag = 0;
	}
	else
	{
		flag = atoi(data_buf);
	}
	
	
	if(flag == 1)
	{
		DataPackage(msg_sendbuf,ASK_STATUS,NULL,CHANTOTALNUM);
	}
	else if(flag == 0)
	{
		if(err_no = cgiFormRadio("mode",ver_buffer,2,&result,0))
		{
			return FAIL;
		}
		else
		{
			sourcemode->mode = result;
		}
		DataPackage(msg_sendbuf,SET_TSMODE,sourcemode,CHANTOTALNUM);
	}
	
	socket_func(msg_sendbuf, msg_recvbuf, sizeof(msg_recvbuf), SEND_PORT);
	
	memset(sourcemode, 0, sizeof(SourceModeConfig_Obj));
	DataUnpackage(msg_recvbuf, SET_TSMODE, sourcemode,CHANTOTALNUM);
	
	printf("%s\n",ver_buffer[sourcemode->mode]);
	
	if(sourcemode != NULL)
	{
		free(sourcemode);
		sourcemode = NULL;
	}
	
}
