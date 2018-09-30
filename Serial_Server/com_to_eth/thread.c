/*******************************************************************
串转网模块数据处理线程
*******************************************************************/

#include "com_ser.h"

/*创建端口主线程函数*/
void create_port(int port)
{
    int temp;

    memset(&thread_m[port], 0, sizeof(thread_m[port]));

    if((temp = pthread_create(&thread_m[port], NULL, thread_port,
    		                 (void *)port)) != 0)
	{
        printf("thread_port_error!\n");
		return;
	}
}

/*端口主线程*/
void *thread_port(void *port)
{
	int port_n = (int)port;

	signal(SIGQUIT, handle_quit);
	port_do(port_n);
	pthread_exit(NULL);
}

/*端口主线程等待函数*/
void wait_port(int port)
{
    if(thread_m[port] !=0)
	{
        pthread_join(thread_m[port], NULL);
        printf("thread_port_over\n");
    }
}

/*创建接收数据线程函数*/
void create_recv(int port)
{
	int temp;

    memset(&thread_r[port], 0, sizeof(thread_r[port]));

    if((temp = pthread_create(&thread_r[port], NULL, thread_recv,
    		                 (void *)port)) != 0)
    {
        printf("thread_recv_error!\n");
		return;
	}
}

/*接收数据线程*/
void *thread_recv(void *port)
{
	char  r_buf[DATA_BUF_SIZE];
	int   readnum, sendnum;
	int   port_n = (int)port;
	int   off = 0, on = 0;
	int   fd, fp;
	int   type;

	type = type_port(port_n);

	signal(SIGQUIT, handle_quit);

	fd = open(CPLD_DEV, O_RDWR);
	if (fd < 0)
    {
    	printf("can't open CPLD_DEV!\n");
    }

	cpld_port_type(port_n, type, fd,RS485_RCVE_FLAG);

    while(1)
	{
    	fp = open(GPIO_DEV, O_RDWR);
    	if (fp < 0)
    	{
    		printf("can't open GPIO_DEV!\n");
    	}

		memset(r_buf, 0, sizeof(r_buf));
		readnum = read(fcom[port_n], r_buf, sizeof(r_buf));
		if(readnum < 0)
		{
			perror("readnum < 0");
			close(fp);
			sleep(1);
			continue;
		}
		if(readnum == 0)
		{
			perror("readnum == 0");
		}

		sendnum = send(sockfd[port_n], r_buf, strlen(r_buf),0);
		if(sendnum < 0)
		{
			perror("sendnum < 0");
			//close(sockfd[port_n]);
			//return;
			continue;
		}
		else if(sendnum == 0)
		{
			perror("sendnum == 0");
		}

		if(off == 10)
		{
			//关闭接收led
			port_led_rx(port_n, GPIO_LED_OFF_FLAG, fp);
			off = 0;
		}
		if(on == 20)
		{
			//打开接收led
			port_led_rx(port_n, GPIO_LED_ON_FLAG, fp);
			on = 0;
		}

		off++;
		on++;

		close(fp);
	}
    close(fd);
    pthread_exit(NULL);
}

/*等待接收数据线程函数*/
void wait_recv(int port)
{
	if(thread_r[port] !=0)
	{
		pthread_join(thread_r[port],NULL);
        printf("thread_recv_over\n");
    }
}

/*创建发送数据线程函数*/
void create_send(int port)
{
    int temp;

    memset(&thread_s[port], 0, sizeof(thread_s[port]));

    if((temp = pthread_create(&thread_s[port], NULL, thread_send,
    		                 (void *)port)) != 0)
    {
        printf("thread_send_error\n");
		return;
	}
}

/*发送数据线程*/
void *thread_send(void *port)
{
	char  s_buf[DATA_BUF_SIZE];
	int   writenum, recvnum;
	int   port_n = (int)port;
	int   off = 0, on = 0;
	int   fd, fp;
	int   type;

	type = type_port(port_n);

	signal(SIGQUIT,handle_quit);

	while(1)
	{
		fd = open(CPLD_DEV, O_RDWR);
		if (fd < 0)
		{
			printf("can't open CPLD_DEV!\n");
		}

		fp = open(GPIO_DEV, O_RDWR);
		if (fp < 0)
		{
			printf("can't open GPIO_DEV!\n");
		}

		memset(s_buf, 0, sizeof(s_buf));
		recvnum = recv(sockfd[port_n],s_buf,WRITE_DATA_NUM,0);
		if(recvnum < 0)
		{
			perror("recvnum < 0");
			close(sockfd[port_n]);
			return;
		}
		else if(recvnum == 0)
		{
			perror("recvnum == 0");
			close(sockfd[port_n]);
			return;
		}
		if(off == 10)
		{
			//关闭发送led
			port_led_tx(port_n, GPIO_LED_OFF_FLAG, fp);
			off = 0;
		}
		if(on == 20)
		{
			//打开发送led
			port_led_tx(port_n, GPIO_LED_ON_FLAG, fp);
			on = 0;
		}
		close(fp);

		cpld_port_type(port_n, type, fd, RS485_SEND_FLAG);

		writenum = write(fcom[port_n], s_buf, strlen(s_buf));
		if(writenum <= 0)
		{
			perror("writenum <= 0");
		}

		cpld_port_type(port_n, type, fd, RS485_RCVE_FLAG);

		off++;
		on++;

		close(fd);
	}

	pthread_exit(NULL);
}

/*发送数据线程等待函数*/
void wait_send(int port)
{
    if(thread_s[port] !=0)
    {
    	pthread_join(thread_s[port], NULL);
        printf("thread_send_over\n");
    }
}


void create_recv_monitoring(int port)
{
	int temp;

    memset(&thread_recv_m[port], 0, sizeof(thread_recv_m[port]));

    if((temp = pthread_create(&thread_recv_m[port], NULL,
    		                 thread_recv_monitoring, (void *)port)) != 0)
	{
        printf("thread_recv_monitoring_error!\n");
		return;
	}
}

/*接收线程监控线程*/
void *thread_recv_monitoring(void *port)
{
	int port_n = (int)port;

	signal(SIGQUIT, handle_quit);

	while(1)
	{
		//判断线程是否存在，如果不存在，则重新创建线程
		if((pthread_kill(thread_r[port_n], 0)) == ESRCH)
		{
			create_recv(port_n);
		}
		sleep(1);
	}
	pthread_exit(NULL);
}


void wait_recv_monitoring(int port)
{
	if(thread_recv_m[port] !=0)
	{
        pthread_join(thread_recv_m[port],NULL);
        printf("thread_recv_monitoring_over\n");
    }
}


void create_send_monitoring(int port)
{
    int temp;

    memset(&thread_send_m[port], 0, sizeof(thread_send_m[port]));

    if((temp = pthread_create(&thread_send_m[port], NULL,
    		                 thread_send_monitoring, (void *)port)) != 0)
	{
        printf("thread_send_monitoring_error!\n");
        return;
	}
}

/*发送线程监控线程*/
void *thread_send_monitoring(void *port)
{
	int port_n = (int)port;

	signal(SIGQUIT,handle_quit);

	while(1)
	{
		if((pthread_kill(thread_s[port_n], 0)) == ESRCH)
		{
			create_send(port_n);
		}
		sleep(1);
	}
	pthread_exit(NULL);
}

void wait_send_monitoring(int port)
{
    if(thread_send_m[port] !=0)
	{
        pthread_join(thread_send_m[port],NULL);
        printf("thread_send_monitoring_over\n");
    }
}

//控制8个串口的接收led
int port_led_rx(int port, int flag, int fp)
{
	int port_led_rx_val[2];

	port_led_rx_val[0]  = flag;

	switch(port)
	{
		case 0:
			port_led_rx_val[1]  = RX1_VAL;
			break;
		case 1:
			port_led_rx_val[1]  = RX2_VAL;
			break;
		case 2:
			port_led_rx_val[1]  = RX3_VAL;
			break;
		case 3:
			port_led_rx_val[1]  = RX4_VAL;
			break;
		case 4:
			port_led_rx_val[1]  = RX5_VAL;
			break;
		case 5:
			port_led_rx_val[1]  = RX6_VAL;
			break;
		case 6:
			port_led_rx_val[1]  = RX7_VAL;
			break;
		case 7:
			if(flag == GPIO_LED_ON_FLAG)
			{
				port_led_rx_val[0]  = RX8_ON_FLAG;
			}
			if(flag == GPIO_LED_OFF_FLAG)
			{
				port_led_rx_val[0]  = RX8_OFF_FLAG;
			}
			break;
	}


	write(fp, port_led_rx_val, sizeof(port_led_rx_val));

	return 0;
}

//控制8个串口的发送led
int port_led_tx(int port, int flag, int fp)
{
	int port_led_tx_val[2];

	port_led_tx_val[0]  = flag;

	switch(port)
	{
		case 0:
			port_led_tx_val[1]  = TX1_VAL;
			break;
		case 1:
			port_led_tx_val[1]  = TX2_VAL;
			break;
		case 2:
			port_led_tx_val[1]  = TX3_VAL;
			break;
		case 3:
			port_led_tx_val[1]  = TX4_VAL;
			break;
		case 4:
			port_led_tx_val[1]  = TX5_VAL;
			break;
		case 5:
			port_led_tx_val[1]  = TX6_VAL;
			break;
		case 6:
			port_led_tx_val[1]  = TX7_VAL;
			break;
		case 7:
			if(flag == GPIO_LED_ON_FLAG)
			{
				port_led_tx_val[0]  = TX8_ON_FLAG;
			}
			if(flag == GPIO_LED_OFF_FLAG)
			{
				port_led_tx_val[0]  = TX8_OFF_FLAG;
			}
			break;
	}

	write(fp, port_led_tx_val, sizeof(port_led_tx_val));

	return 0;
}

//配置串口类型：RS232，RS485，RS422
int cpld_port_type(int port, int type, int fd, char rs485_flag)
{
	int cpld_val[2];

	cpld_val[0] = port;

	switch(type)
	{
		case PORT_RS232:
			cpld_val[1] = 0x16;
			write(fd, cpld_val, sizeof(cpld_val));
			break;
		case PORT_RS485:
			if(rs485_flag == RS485_RCVE_FLAG)
			{
				cpld_val[1] = 0x11;
				write(fd, cpld_val, sizeof(cpld_val));
			}
			if(rs485_flag == RS485_SEND_FLAG)
			{
				cpld_val[1] = 0x1d;
				write(fd, cpld_val, sizeof(cpld_val));
			}
			break;
		case PORT_RS422:

			cpld_val[1] = 0x31;
			write(fd, cpld_val, sizeof(cpld_val));
			break;
	}

	return 0;
}

//解析配置文件中关于串口类型的配置
int type_port(int port)
{
	char  *name[PORT_NUM] = {PORT1_DIR, PORT2_DIR, PORT3_DIR, PORT4_DIR,
							  PORT5_DIR, PORT6_DIR, PORT7_DIR, PORT8_DIR};
	int   fp, type, t_y;
	char  type_buf[OTHER_BIT_NUM];


	fp = open(name[port], O_RDONLY);
	if(fp < 0)
	{
		perror("open port_txt");
		return(-1);
	}

	lseek(fp, PORT_TYPE_OFFSET, SEEK_SET);

	t_y = read(fp, type_buf, OTHER_BIT_NUM);
	if(t_y < 0)
	{
		perror("read port type");
		return(-1);
	}

	close(fp);
	type = atoi(type_buf);
	return type;
}



/*创建udp接收数据线程函数*/
void udp_create_recv(Udp_data udp_data)
{
	int temp;

    memset(&udp_thread_r[udp_data.port], 0,
    	  sizeof(udp_thread_r[udp_data.port]));

    if((temp = pthread_create(&udp_thread_r[udp_data.port], NULL,
    		                 udp_thread_recv, (void *)&udp_data)) != 0)
    {
        printf("udp_thread_recv_error!\n");
		return;
	}
}

/*udp接收数据线程*/
void *udp_thread_recv(void *udp_data)
{
	Udp_data *udp_data_val = (Udp_data *)udp_data;
	struct 	 sockaddr_in dest = udp_data_val->dest;
	char     u_rbuf[1024];
	int      readnum, sendnum;
	int      port_n = udp_data_val->port;
	int      off = 0, on = 0;
	int      fd, fp;
	int      type;

	type = type_port(port_n);

	signal(SIGQUIT, handle_quit);

    while(1)
	{
    	fd = open(CPLD_DEV, O_RDWR);
		if (fd < 0)
		{
			printf("can't open CPLD_DEV!\n");
		}

		cpld_port_type(port_n, type, fd, RS485_RCVE_FLAG);

    	memset(u_rbuf, 0, sizeof(u_rbuf));
    	readnum = read(fcom[port_n], u_rbuf, sizeof(u_rbuf));
		if(readnum < 0)
		{
			perror("readnum < 0");
			//continue;
		}
		if(readnum == 0)
		{
			perror("readnum == 0");
		}

		fp = open(GPIO_DEV, O_RDWR);
		if (fp < 0)
		{
		    printf("can't open GPIO_DEV!\n");
		}

		if(off == 10)
		{
			port_led_rx(port_n, GPIO_LED_OFF_FLAG,fp);
			off = 0;
		}
		if(on == 20)
		{
			port_led_rx(port_n, GPIO_LED_ON_FLAG,fp);
			on = 0;
		}

		sendnum = sendto(sockfd[port_n], u_rbuf, strlen(u_rbuf), 0,
						        (struct sockaddr *)&dest, sizeof(dest));
		if(sendnum < 0)
		{
			perror("sendnum < 0");
			//close(sockfd[port_n]);
			//return;
		}
		else if(sendnum == 0)
		{
			perror("sendnum == 0");
		}

		off++;
		on++;

		close(fp);
		close(fd);

	}
    pthread_exit(NULL);
}

/*udp等待接收数据线程函数*/
void udp_wait_recv(int port)
{
	if(udp_thread_r[port] !=0)
	{
		pthread_join(udp_thread_r[port],NULL);
        printf("udp_thread_recv_over\n");
    }
}

/*创建udp发送数据线程函数*/
void udp_create_send(Udp_data udp_data)
{
    int temp;

    memset(&udp_thread_s[udp_data.port], 0, sizeof(udp_thread_s[udp_data.port]));

    if((temp = pthread_create(&udp_thread_s[udp_data.port], NULL,
    		                 udp_thread_send, (void *)&udp_data)) != 0)

    {
        printf("udp_thread_send_error\n");
		return;
	}
}

/*udp发送数据线程*/
void *udp_thread_send(void *udp_data)
{
	Udp_data *udp_data_val = (Udp_data *)udp_data;
	char     u_sbuf[UDP_BUF_SIZE];
	int      writenum, recvnum;
	int      port_n = udp_data_val->port;
	//struct sockaddr_in self = udp_data_val->self;
	int      off = 0, on = 0;
	int      fd, fp;
	int      type, i, data_leng, write_num;

	type = type_port(port_n);

	signal(SIGQUIT,handle_quit);

	while(1)
	{
		fd = open(CPLD_DEV, O_RDWR);
		if (fd < 0)
		{
			printf("can't open CPLD_DEV!\n");
		}

		fp = open(GPIO_DEV, O_RDWR);
		if (fp < 0)
		{
			printf("can't open GPIO_DEV!\n");
		}

		memset(u_sbuf, 0, sizeof(u_sbuf));

		recvnum   = recvfrom(recv_sockfd[port_n], u_sbuf,
				            sizeof(u_sbuf), 0, NULL,NULL);
		data_leng = strlen(u_sbuf);
		write_num = data_leng/WRITE_DATA_NUM;
		if(recvnum < 0)
		{
			//printf("%d\n", udp_data_val->port);
			perror("recvnum < 0");
		}
		else if(recvnum == 0)
		{
			perror("recvnum == 0");
		}

		if(off == 10)
		{
			port_led_tx(port_n,GPIO_LED_OFF_FLAG,fp);
			off = 0;
		}
		if(on == 20)
		{
			port_led_tx(port_n,GPIO_LED_ON_FLAG,fp);
			on = 0;
		}
		close(fp);

		cpld_port_type(port_n,type,fd,RS485_SEND_FLAG);

		for(i = 0; i < write_num+1; i++)
		{
			if(i == write_num)
			write(fcom[port_n],u_sbuf + (i*WRITE_DATA_NUM),
				 strlen(u_sbuf + (i*WRITE_DATA_NUM)));
			else
			write(fcom[port_n],u_sbuf + (i*WRITE_DATA_NUM),WRITE_DATA_NUM);
		}

		cpld_port_type(port_n, type, fd, RS485_RCVE_FLAG);

		off++;
		on++;

		close(fd);
	}

	pthread_exit(NULL);
}

/*udp发送数据线程等待函数*/
void udp_wait_send(int port)
{
    if(udp_thread_s[port] !=0)
    {
    	pthread_join(udp_thread_s[port],NULL);
        printf("udp_thread_send_over\n");
    }
}
