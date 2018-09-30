/*******************************************************************
��ת��ģ�飺��ģ��ͨ����ȡ�������ڵ������ļ������������Ƿ�������IP��ȫ���ƣ�
�Լ�����ģʽ������ģʽ��TCP SERVER��TCP CLIENT��UDP��
Ȼ�������ݴӴ��ڵ����ڵ�ת���̣߳����ﵽ���ݵĴ�ת�����ܡ�
*******************************************************************/

#include "com_ser.h"

/*****************************************************************************
* �������ƣ� read_limit_ip
* ����������IP ���������ļ���ȡ����
* ��������� pfile:�������ļ����ļ�������
			pversion:IP��Ϣ�ṹ��
* ��������� ��
* ��   ��  ֵ�� ��
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
* �������ƣ� ip_limit_part
* ���������� IP���������ļ���������
* ��������� dip:������ʷ�������IP
			port:�����д�Ķ˿ں�
* ��������� ��
* ��   ��   ֵ�� �ɹ��򷵻�0
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
* �������ƣ� tcp_ser_do
* ���������� ������������TCP SERVER ģʽ������
* ��������� baud:������
			databits:����λ
			stopbits:ֹͣλ
			parity:У��λ
			port:�˿ں�
			ip_limit:IP ��������־
			limit:�˿�Ȩ�����Ʊ�־
			localport:SOCKET ���ض˿ں�
* ��������� ��
* ��   ��   ֵ�� �ɹ��򷵻�0
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
* �������ƣ� tcp_client_do
* ���������� ������������TCP CLIENT ģʽ������
* ��������� baud:������
			databits:����λ
			stopbits:ֹͣλ
			parity:У��λ
			port:�˿ں�
			ip_limit:IP ��������־
			limit:�˿�Ȩ�����Ʊ�־
			dip:SOCKET Զ�̷�����IP
			serport:SOCKET Զ�̷������˿ں�
* ��������� ��
* ��   ��   ֵ�� �ɹ��򷵻�0
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
* �������ƣ� udp_do
* ���������� ������������UDP ģʽ������
* ��������� baud:������
			databits:����λ
			stopbits:ֹͣλ
			parity:У��λ
			port:�˿ں�
			ip_limit:IP ��������־
			limit:�˿�Ȩ�����Ʊ�־
			dip:SOCKET �Զ�IP
			serport:SOCKET �Զ˶˿ں�
* ��������� ��
* ��   ��   ֵ�� �ɹ��򷵻�0
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

	//����socketΪUDP�㲥ģʽ
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
* �������ƣ� port_do
* ���������� �������˿ڴ��������˿����ò�����ȡ�����
* ��������� port:�˿ں�
* ��������� ��
* ��   ��   ֵ�� �ɹ�����0
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
	//sprintf(name, "/mnt/config/cfg/port%d.txt", (port - 1))��

	fp = open(name[port], O_RDONLY);
	if(fp < 0)
	{
		perror("open name[port]");
		return(-1);
	}

    r_b = read(fp, baudrate, BAUD_BIT_NUM);   //��ȡ������������Ϣ
	if(r_b < 0)
	{
		perror("read baud");
		return(-1);
	}
	baud = atoi(baudrate);

	lseek(fp, DATABIT_OFFSET, SEEK_SET);
	r_d = read(fp, databit, OTHER_BIT_NUM);   //��ȡ����λ��Ϣ
	if(r_d < 0)
	{
		perror("read databit");
		return(-1);
	}
	databits = atoi(databit);

	lseek(fp, STOPBIT_OFFSET, SEEK_SET);
	r_s = read(fp, stopbit, OTHER_BIT_NUM);    //��ȡֹͣλ��Ϣ
	if(r_s < 0)
	{
		perror("read stopbit");
		return(-1);
	}
	stopbits = atoi(stopbit);

	lseek(fp, PARITY_OFFSET, SEEK_SET);
	r_p = read(fp, parity, OTHER_BIT_NUM);      //��ȡУ��λ��Ϣ
	if(r_p < 0)
	{
		perror("read parity");
		return(-1);
	}

	lseek(fp, SER_IP_OFFSET, SEEK_SET);
	r_ip = read(fp, dip, IP_BIT_NUM);           //��ȡ�Զ�IP��ַ��Ϣ
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
	r_port = read(fp, serport_n, PORT_BIT_NUM);  //��ȡ�Զ˶˿ں���Ϣ
	if(r_port < 0)
	{
		perror("read serport_n");
		return(-1);
	}
	serport = atoi(serport_n);

	lseek(fp, LOCALPORT_OFFSET, SEEK_SET);
	r_lp = read(fp, localport_n, PORT_BIT_NUM);  //��ȡ���ض˿ں���Ϣ
	if(r_lp < 0)
	{
		perror("read localport_n");
		return(-1);
	}
	localport = atoi(localport_n);

	lseek(fp, SOCK_FLAG_OFFSET, SEEK_SET);
	r_f = read(fp, flag, OTHER_BIT_NUM);          //��ȡ���ڹ���ģʽ��Ϣ
	if(r_f < 0)
	{
		perror("read flag");
		return(-1);
	}

	lseek(fp, IP_LIMIT_OFFSET, SEEK_SET);
	ip_lim = read(fp, ip_limit, OTHER_BIT_NUM);    //��ȡIP���������Ϣ
	if(ip_lim < 0)
	{
		perror("read ip_lim");
		return(-1);
	}
	close(fp);

	limit = ip_limit_part(dip,port);                //�������IP��Ӧ�Ĵ��ڲ���Ȩ

	//�˿�ģʽΪTCP SERVER
	if(flag[0] == TCP_SERVER_FLAG)
	{
		tcp_ser_do(baud, databits, stopbits, parity[0],
				  port, ip_limit[0], limit, localport);
	}

	//�˿�ģʽΪTCP CLIENT
	if(flag[0] == TCP_CLIENT_FLAG)
	{
		tcp_client_do(baud, databits, stopbits, parity[0],
				     port, ip_limit[0], limit, dip, serport);
	}

	//�˿�ģʽΪUDP
	if(flag[0] == UDP_FLAG)
	{
		udp_do(baud, databits, stopbits, parity[0], port,
			  ip_limit[0], limit, dip, serport, localport);
	}
	return 0;
}

//�û��Դ����пɶ���дȨ��
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


//�û��Դ�����ֻ��Ȩ��
void port_ro_do(int port)
{
	create_recv(port);
	//create_recv_monitoring(port);
	wait_recv(port);
	//wait_recv_monitoring(port);
}

//udpģʽ���û��Դ����пɶ���дȨ��
void udp_port_full_do(Udp_data udp_data)
{
	udp_create_recv(udp_data);
	udp_create_send(udp_data);
	udp_wait_recv(udp_data.port);
	udp_wait_send(udp_data.port);
}


//udpģʽ���û��Դ�����ֻ��Ȩ��
void udp_port_ro_do(Udp_data udp_data)
{
	udp_create_recv(udp_data);
	udp_wait_recv(udp_data.port);
}

