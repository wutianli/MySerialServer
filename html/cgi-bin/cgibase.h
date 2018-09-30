#ifndef _CGIBASE_H_
#define _CGIBASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define OK           0
#define FAIL         -1

#define TEMPLATE_PATH        "/mnt/hgfs/myWork/CGI/eg/user_and_book_admin_system/template/"
/*define CGI MIME type and charset*/
#define CGI_HEADER() \
	do { \
	printf("Content-Type:text/html;charset=gb2312\r\n\r\n"); \
	} while(0)

#ifdef __cplusplus
extern "C"{
#endif



#ifdef __cplusplus
};
#endif

#endif


