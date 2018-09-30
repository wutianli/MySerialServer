/*******************************************************************
串口参数配置模块：该模块根据传入的参数，打开相应串口设备，然后对该串口进行初始化，
初始化的内容包括设置该串口的波特率，数据位，停止位，校验位。
*******************************************************************/

#include "com_ser.h"

/********************************************************************
* 函数名称： ComOpen
* 功能描述： 打开设备
* 输入参数： port:打开设备的设备结点
* 输出参数： 无
* 返   回   值： 成功则返回打开设备的文件描述符
********************************************************************/

int ComOpen(unsigned char *port)
{
   int fd;

   fd = open(port, O_RDWR|O_NOCTTY);
   if(fd==-1)
   {
      perror("ArmCom:   Can't Open Com Port");
      return(-1);
   }

   return fd;
}

/*****************************************************************************
* 函数名称： ComSetBaud
* 功能描述： 设置波特率
* 输入参数： fd:打开设备的文件描述符
			baud:波特率
* 输出参数： 无
* 返   回   值： 成功则返回0
******************************************************************************/

int baud_attr[]={B1000000,B921600,B500000,B230400,B115200, B57600,
		        B38400, B19200, B9600, B4800, B2400, B1200, B300};
int name_attr[]={1000000,921600,500000,230400,115200, 57600,
		        38400, 19200, 9600, 4800, 2400, 1200, 300};
int ComSetBaud(int fd, int baud)
{
   int    i, j;
   int    status;
   struct termios Opt;

   tcgetattr(fd, &Opt);

	for(j=0; j<2; j++)
	{
		for(i=0; i<sizeof(baud_attr)/sizeof(int); i++)
		{
			if(baud==name_attr[i])
			{
				tcflush(fd, TCIOFLUSH);
				cfsetispeed(&Opt, baud_attr[i]);
				cfsetospeed(&Opt, baud_attr[i]);
				status = tcsetattr(fd, TCSANOW, &Opt);
				if(status != 0)
				{
					perror("ArmCom:   tcsetattr fail");
					return(-1);
				}
				tcflush(fd, TCIOFLUSH);
			}
		}
	}
	return (0);
}

/*****************************************************************************
* 函数名称： ComSetParity
* 功能描述： 设置数据位、停止位、校验位
* 输入参数： fd:打开设备的文件描述符
			databits:数据位
			stopbits:停止位
			parity:校验位
* 输出参数： 无
* 返   回   值： 成功则返回0
******************************************************************************/

int ComSetParity(int fd, int databits, int stopbits, char parity)
{
	struct termios options;

	if(tcgetattr(fd, &options) != 0)
	{
		perror("ArmCom:   tcgetattr fail");
		return(-1);
	}

	options.c_cflag &= ~CSIZE;

	switch(databits)
	{
		case 5:
			options.c_cflag |= CS5;                 //数据位：5
			break;
		case 6:
			options.c_cflag |= CS6;
			break;
		case 7:
			options.c_cflag |= CS7;
			break;
		case 8:
			options.c_cflag |= CS8;                 //数据位：8
			break;
		default:
			fprintf(stderr, "ArmCom:   Unsupported data size");
		return(-1);
    }

    switch(parity)
    {
		case 'n':
		case 'N':
			 options.c_cflag &= ~PARENB;
			 options.c_iflag &= ~INPCK;              //校验：none
			 break;
		case 'o':
		case 'O':
			 options.c_cflag |= (PARODD | PARENB);   //校验：odd
			 options.c_iflag |= INPCK;
			 break;
		case 'e':
		case 'E':
			 options.c_cflag |= PARENB;				 //校验：even
			 options.c_cflag &= ~PARODD;
			 options.c_iflag |= INPCK;
			 break;
		case 'm':                                    //校验：mark
		case 'M':
			options.c_cflag |= PARENB | CMSPAR | PARODD;
			break;
		case 's':
		case 'S':                                    //校验：space
			//options.c_cflag &= ~PARENB;
			//options.c_cflag &= ~CSTOPB;
			options.c_cflag |= PARENB | CMSPAR;
			break;

		default:
			 fprintf(stderr, "ArmCom:   Unsupported parity");
			 return(-1);
   }

   switch(stopbits)
   {
       case 1:
			 options.c_cflag &= ~CSTOPB;
			 break;
   	   case 2:
			 options.c_cflag |= CSTOPB;
		     break;
   	   default:
			 fprintf(stderr, "ArmCom:   Unsupported stop bits");
			 return(-1);
   }


   	if(parity=='n' || parity=='N')
	options.c_iflag =0;


	options.c_lflag &= ~(ICANON | ECHO | ECHOE |ISIG);
	options.c_oflag = 0;

	tcflush(fd, TCIFLUSH);
	options.c_cc[VTIME] = 150;
	options.c_cc[VMIN]  = 0;


	if(tcsetattr(fd, TCSANOW, &options) != 0)
	{
		 perror("ArmCom:   Setup Serial Port");
		 return (-1);
	}
	return(0);
}

/*****************************************************************************
* 函数名称： ComInit
* 功能描述： 端口初始化
* 输入参数： baudrate:波特率
			databits:数据位
			stopbits:停止位
			parity:校验位
			com:端口号
* 输出参数： 无
* 返   回   值： 成功则返回打开端口的文件描述符
******************************************************************************/
int ComInit(int baudrate, int databits, int stopbits, char parity, int com)
{
   int    fd, i;
   int    fdd = 0;
   struct sockaddr_in server_addr;
   char   portt[9]= "p00000000";
   char   *dev[PORT_NUM] = {"/dev/ttyS5", "/dev/ttyS4", "/dev/ttyS3", "/dev/ttyS2",
		                   "/dev/ttyS9", "/dev/ttyS8", "/dev/ttyS7", "/dev/ttyS6"};

   fd = ComOpen(dev[com]);
   if(fd == -1)
   {
      printf("ArmCom: Fail to Open COM(%s)\n", dev);
      return(-1);
   }
   else
   {
      ComSetBaud(fd, baudrate);
	  if(ComSetParity(fd, databits, stopbits, parity)==(-1))
	  {
		  //如果配置失败，给应用程序发送串口重启信号，重启串口
		  for(i = 0; i < PORT_NUM; i++)
		  {
			  if(com == i)
			  portt[i+1] = PORT_RESET_FLAG;
		  }
		  bzero(&server_addr, sizeof(server_addr));
		  server_addr.sin_family      = AF_INET;
		  server_addr.sin_port        = htons(SOCKET_PORT);
		  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

		  fdd = socket(AF_INET, SOCK_STREAM, 0);
		  if( fdd < 0 )
		  {
		  		perror("cominit_socket");
		  		return (-1);
		  }
		  if (connect(fdd, (struct sockaddr*)&server_addr, sizeof(server_addr)) !=0)
		  {
		  		perror("cominit_connect");
		  		return (-1);
		  }
		  sleep(1);
		  send(fdd,portt, 9, 0);
		  close(fdd);

		  printf("ArmCom: Set COM(%s) Parity Error\n", dev[com]);
		  return (-1);
	  }

	  printf("Armcom: Open COM(%s), Baudrate=%d\n", dev[com], baudrate);
	  return fd;

	}
}
