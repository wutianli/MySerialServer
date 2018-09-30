/*******************************************************************
串转网模块：该模块通过读取各个串口的配置文件，解析串口是否设置了IP安全限制，
以及工作模式，工作模式有TCP SERVER，TCP CLIENT，UDP，
然后建立数据从串口到网口的转换线程，来达到数据的串转网功能。
*******************************************************************/

#include "com_ser.h"

/*****************************************************************************
* 函数名称： read_limit_ip
* 功能描述：IP 限制配置文件读取函数
* 输入参数： pfile:打开配置文件的文件描述符
			pversion:IP信息结构体
* 输出参数： 无
* 返   回  值： 无
******************************************************************************/
void read_limit_ip(FILE *pfile, Ip_Info *pversion)
{
	int   i = 0;
	char  buf[100];
	char  *p = NULL;
	char  *ver_buffer[5] = {"ip:", "mask:", "read:", "write:", "manage:"};
	char  find_str[100];

	fgets(buf,100,pfile);

	for(i = 0; i < 5; i++)
	{
		if((p = strstr(buf,ver_buffer[i]))!=NULL)
		{
			memset(find_str, 0, sizeof(find_str));
			sprintf(find_str ,"%s%s", ver_buffer[i],"%[^:]");
			sscanf(p, find_str, (char *)pversion + i * IP_INFO_NUM);
		}
	}
}

/*****************************************************************************
* 函数名称： ip_limit_part
* 功能描述： IP限制配置文件解析函数
* 输入参数： dip:允许访问服务器的IP
			port:允许读写的端口号
* 输出参数： 无
* 返   回   值： 成功则返回0
******************************************************************************/
char ip_limit_part(char *dip, int port)
{
	Ip_Info ip_info[IP_NUM];
	FILE    *fop = fopen(IP_DIR, "r");
	int     k = 0;

	for(k = 0; k < IP_NUM; k++)
	{
		memset(&ip_info[k], 0, sizeof(Ip_Info));
		read_limit_ip(fop, &ip_info[k]);
	}

	for(k = 0; k < IP_NUM; k++)
	{
		if((0 == memcmp(ip_info[k].ip,dip,strlen(dip))) &&
				(ip_info[k].manage[0] == MANAGE_RO_FLAG) &&
				((port + 1) == atoi(&ip_info[k].read[0])))
		{
			return MANAGE_RO_FLAG;
		}
		if((0 == memcmp(ip_info[k].ip,dip,strlen(dip))) &&
				(ip_info[k].manage[0] == MANAGE_FULL_FLAG) &&
				((port + 1) == atoi(&ip_info[k].read[0])) &&
				((port + 1) == atoi(&ip_info[k].write[0])))
		{
			return MANAGE_FULL_FLAG;
		}
	}
	return 0;
}

/*****************************************************************************
* 函数名称： tcp_ser_do
* 功能描述： 服务器工作在TCP SERVER 模式处理函数
* 输入参数： baud:波特率
			databits:数据位
			stopbits:停止位
			parity:校验位
			port:端口号
			ip_limit:IP 限制与否标志
			limit:端口权限限制标志
			localport:SOCKET 本地端口号
* 输出参数： 无
* 返   回   值： 成功则返回0
******************************************************************************/
int tcp_ser_do(int baud, int databits, int stopbits,
		      char parity, int port, char ip_limit,
		      char limit, int localport)
{
	struct sockaddr_in their_addr;
	int    ret, opt;
	int    size = sizeof(their_addr);

	if ((s_sockfd[port] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("s_socket");
		return(-1);
	}

	opt  = 1;
	setsockopt(s_sockfd[port], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	memset(&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family      = AF_INET;
	local_addr.sin_port        = htons(localport);
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if((ret = bind(s_sockfd[port],
			      (struct sockaddr *)&local_addr, sizeof(local_addr))) != 0)
	{
		perror("ser_bind");
		return(-1);
	}

	listen(s_sockfd[port], 1);

	sockfd[port] = accept(s_sockfd[port],
			             (struct sockaddr*)&their_addr, &size);
	if(sockfd[port] == -1)
	{
		return 0;
	}

	if(ip_limit == IP_NOTLIMIT_FLAG)
	{
		fcom[port] = ComInit(baud, databits, stopbits, parity, port);
		if(fcom[port] < 0)
		{
			return(-1);
		}

		port_full_do(port);
	}
	if(ip_limit == IP_LIMIT_FLAG)
	{
		if(limit == MANAGE_FULL_FLAG)
		{
			fcom[port] = ComInit(baud, databits, stopbits, parity, port);
			if(fcom[port] < 0)
			{
				return(-1);
			}

			port_full_do(port);
		}
		if(limit == MANAGE_RO_FLAG)
		{
			fcom[port] = ComInit(baud, databits, stopbits, parity, port);
			if(fcom[port] < 0)
			{
				return(-1);
			}

			port_ro_do(port);
		}

	}
	close(sockfd[port]);
	close(s_sockfd[port]);
	close(fcom[port]);
	return 0;
}

/*****************************************************************************
* 函数名称： tcp_client_do
* 功能描述： 服务器工作在TCP CLIENT 模式处理函数
* 输入参数： baud:波特率
			databits:数据位
			stopbits:停止位
			parity:校验位
			port:端口号
			ip_limit:IP 限制与否标志
			limit:端口权限限制标志
			dip:SOCKET 远程服务器IP
			serport:SOCKET 远程服务器端口号
* 输出参数： 无
* 返   回   值： 成功则返回0
******************************************************************************/


int tcp_client_do(int baud, int databits, int stopbits,
		         char parity, int port, char ip_limit,
		         char limit, char *dip, int serport)
{
	int ret, opt;

	loop:
	if(ret = (sockfd[port] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("c_socket");
		return(-1);
	}

	opt = 1;
	setsockopt(sockfd[port], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = htons(serport);
	server_addr.sin_addr.s_addr = inet_addr(dip);

	while((ret = connect(sockfd[port], (struct sockaddr *)&server_addr,
		 sizeof(server_addr))) < 0)
	{
		printf("port %d\n", port + 1);
		perror("client_connect");
		close(sockfd[port]);
		sleep(1);
		goto loop;
	}
	if(ret == EINPROGRESS)
	{
		printf("connect...\n");
	}
	if(ret == 0)
	{
		printf("Connect success\n");
	}

	if(ip_limit == IP_NOTLIMIT_FLAG)
	{
		fcom[port] = ComInit(baud, databits, stopbits, parity, port);
		if(fcom[port] < 0)
		{
			return(-1);
		}

		port_full_do(port);
	}
	if(ip_limit == IP_LIMIT_FLAG)
	{
		if(limit == MANAGE_FULL_FLAG)
		{
			fcom[port] = ComInit(baud, databits, stopbits, parity, port);
			if(fcom[port] < 0)
			{
				return(-1);
			}

			port_full_do(port);
		}
		if(limit == MANAGE_RO_FLAG)
		{
			fcom[port] = ComInit(baud, databits, stopbits, parity, port);
			if(fcom[port] < 0)
			{
				return(-1);
			}

			port_ro_do(port);
		}
	}
	close(sockfd[port]);
	close(fcom[port]);
	return 0;
}

/*****************************************************************************
* 函数名称： udp_do
* 功能描述： 服务器工作在UDP 模式处理函数
* 输入参数： baud:波特率
			databits:数据位
			stopbits:停止位
			parity:校验位
			port:端口号
			ip_limit:IP 限制与否标志
			limit:端口权限限制标志
			dip:SOCKET 对端IP
			serport:SOCKET 对端端口号
* 输出参数： 无
* 返   回   值： 成功则返回0
******************************************************************************/

int udp_do(int baud, int databits, int stopbits,
		  char parity, int port, char ip_limit,
		  char limit, char *dip, int serport, int localport)
{
	Udp_data udp_data;
	int      opt, fd;
	int      off = 0,on = 0;
	int      ret;

	sockfd[port] = socket(AF_INET, SOCK_DGRAM,0);
	if(sockfd[port] == -1)
	{
		perror("udp_socket");
		return -1;
	}

	recv_sockfd[port] = socket(AF_INET, SOCK_DGRAM, 0);
	if(recv_sockfd[port] == -1)
	{
		perror("udp_recv_socket");
		return -1;
	}

	opt = 1;
	setsockopt(sockfd[port], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	setsockopt(recv_sockfd[port], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	//设置socket为UDP广播模式
	setsockopt(sockfd[port], SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
	setsockopt(recv_sockfd[port], SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));

	memset(&udp_data.dest,0,sizeof(udp_data.dest));
	udp_data.dest.sin_family      = AF_INET;
	udp_data.dest.sin_port        = htons(serport);
	inet_aton(dip,&udp_data.dest.sin_addr);

	memset(&udp_data.self,0,sizeof(udp_data.self));
	udp_data.self.sin_family      = AF_INET;
	udp_data.self.sin_port        = htons(localport);
	udp_data.self.sin_addr.s_addr = INADDR_ANY;

	ret = bind(recv_sockfd[port], (struct sockaddr *)&udp_data.self,
			  sizeof(udp_data.self));
	if(ret == -1)
	{
		perror("udp_bind");
		return -1;
	}

	if(ip_limit == IP_NOTLIMIT_FLAG)
	{
		fcom[port] = ComInit(baud, databits, stopbits, parity, port);
		if(fcom[port] < 0)
		{
			return(-1);
		}

		udp_port_full_do(udp_data);
	}

	if(ip_limit == IP_LIMIT_FLAG)
	{
		if(limit == MANAGE_FULL_FLAG)
		{
			fcom[port] = ComInit(baud, databits, stopbits, parity, port);
			if(fcom[port] < 0)
			{
				return(-1);
			}

			udp_port_full_do(udp_data);
		}
		if(limit == MANAGE_RO_FLAG)
		{
			fcom[port] = ComInit(baud, databits, stopbits, parity, port);
			if(fcom[port] < 0)
			{
				return(-1);
			}

			udp_port_ro_do(udp_data);
		}
	}
	close(sockfd[port]);
	return 0;
}

/*****************************************************************************
* 函数名称： port_do
* 功能描述： 服务器端口处理函数，端口配置参数读取与解析
* 输入参数： port:端口号
* 输出参数： 无
* 返   回   值： 成功返回0
******************************************************************************/

int port_do(int port)
{
	char  dip[IP_BIT_NUM], serport_n[PORT_BIT_NUM],
	      localport_n[PORT_BIT_NUM], ip_limit[OTHER_BIT_NUM], flag[OTHER_BIT_NUM];

	char  baudrate[BAUD_BIT_NUM],databit[OTHER_BIT_NUM],
	      stopbit[OTHER_BIT_NUM],parity[OTHER_BIT_NUM], limit;

	char  *name[PORT_NUM] = {PORT1_DIR,PORT2_DIR,PORT3_DIR,PORT4_DIR,
			               PORT5_DIR,PORT6_DIR,PORT7_DIR,PORT8_DIR};

	int   serport,localport, baud, databits, stopbits;
	int   fp, r_b, r_d, r_s, r_p, r_ip, m, r_port, r_lp, r_f, ip_lim;

	//char  name[25];
	//sprintf(name, "/mnt/config/cfg/port%d.txt", (port - 1))；

	fp = open(name[port], O_RDONLY);
	if(fp < 0)
	{
		perror("open name[port]");
		return(-1);
	}

    r_b = read(fp, baudrate, BAUD_BIT_NUM);   //读取波特率配置信息
	if(r_b < 0)
	{
		perror("read baud");
		return(-1);
	}
	baud = atoi(baudrate);

	lseek(fp, DATABIT_OFFSET, SEEK_SET);
	r_d = read(fp, databit, OTHER_BIT_NUM);   //读取数据位信息
	if(r_d < 0)
	{
		perror("read databit");
		return(-1);
	}
	databits = atoi(databit);

	lseek(fp, STOPBIT_OFFSET, SEEK_SET);
	r_s = read(fp, stopbit, OTHER_BIT_NUM);    //读取停止位信息
	if(r_s < 0)
	{
		perror("read stopbit");
		return(-1);
	}
	stopbits = atoi(stopbit);

	lseek(fp, PARITY_OFFSET, SEEK_SET);
	r_p = read(fp, parity, OTHER_BIT_NUM);      //读取校验位信息
	if(r_p < 0)
	{
		perror("read parity");
		return(-1);
	}

	lseek(fp, SER_IP_OFFSET, SEEK_SET);
	r_ip = read(fp, dip, IP_BIT_NUM);           //读取对端IP地址信息
	m = 0;
	for(m; m < IP_NUM; m++)
	{
		if(dip[m] == ' ')
		dip[m] = '\0';
	}
	if(r_ip < 0)
	{
		perror("read dip");
		return(-1);
	}

	lseek(fp, SER_PORT_OFFSET, SEEK_SET);
	r_port = read(fp, serport_n, PORT_BIT_NUM);  //读取对端端口号信息
	if(r_port < 0)
	{
		perror("read serport_n");
		return(-1);
	}
	serport = atoi(serport_n);

	lseek(fp, LOCALPORT_OFFSET, SEEK_SET);
	r_lp = read(fp, localport_n, PORT_BIT_NUM);  //读取本地端口号信息
	if(r_lp < 0)
	{
		perror("read localport_n");
		return(-1);
	}
	localport = atoi(localport_n);

	lseek(fp, SOCK_FLAG_OFFSET, SEEK_SET);
	r_f = read(fp, flag, OTHER_BIT_NUM);          //读取串口工作模式信息
	if(r_f < 0)
	{
		perror("read flag");
		return(-1);
	}

	lseek(fp, IP_LIMIT_OFFSET, SEEK_SET);
	ip_lim = read(fp, ip_limit, OTHER_BIT_NUM);    //读取IP限制与否信息
	if(ip_lim < 0)
	{
		perror("read ip_lim");
		return(-1);
	}
	close(fp);

	limit = ip_limit_part(dip,port);                //获得限制IP对应的串口操作权

	//端口模式为TCP SERVER
	if(flag[0] == TCP_SERVER_FLAG)
	{
		tcp_ser_do(baud, databits, stopbits, parity[0],
				  port, ip_limit[0], limit, localport);
	}

	//端口模式为TCP CLIENT
	if(flag[0] == TCP_CLIENT_FLAG)
	{
		tcp_client_do(baud, databits, stopbits, parity[0],
				     port, ip_limit[0], limit, dip, serport);
	}

	//端口模式为UDP
	if(flag[0] == UDP_FLAG)
	{
		udp_do(baud, databits, stopbits, parity[0], port,
			  ip_limit[0], limit, dip, serport, localport);
	}
	return 0;
}

//用户对串口有可读可写权限
void port_full_do(int port)
{
	create_recv(port);
	create_send(port);
	//create_recv_monitoring(port);
	//create_send_monitoring(port);
	wait_recv(port);
	wait_send(port);
	//wait_recv_monitoring(port);
	//wait_send_monitoring(port);
}


//用户对串口有只读权限
void port_ro_do(int port)
{
	create_recv(port);
	//create_recv_monitoring(port);
	wait_recv(port);
	//wait_recv_monitoring(port);
}

//udp模式下用户对串口有可读可写权限
void udp_port_full_do(Udp_data udp_data)
{
	udp_create_recv(udp_data);
	udp_create_send(udp_data);
	udp_wait_recv(udp_data.port);
	udp_wait_send(udp_data.port);
}


//udp模式下用户对串口有只读权限
void udp_port_ro_do(Udp_data udp_data)
{
	udp_create_recv(udp_data);
	udp_wait_recv(udp_data.port);
}

