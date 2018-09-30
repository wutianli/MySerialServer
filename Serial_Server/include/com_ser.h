/*
 * com_sedr.h
 *
 *  Created on: 2014年4月23日
 *      Author: 李广鸿
 */

#ifndef COM_SEDR_H_
#define COM_SEDR_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <net/if.h>


#define PORT_NUM         8                 //串口数
#define IP_NUM           16

#define TCP_SERVER_FLAG  '1'               //TCP server工作模式标志
#define TCP_CLIENT_FLAG  '2'
#define UDP_FLAG         '3'

#define IP_NOTLIMIT_FLAG '0'
#define IP_LIMIT_FLAG    '1'               //IP访问限制标志

#define MANAGE_RO_FLAG   '1'               //对串口有只读操作权限
#define MANAGE_FULL_FLAG '2'               //对串口有可读写权限

#define P_RESET_FLAG     'p'               //串口重启标志
#define PORT_RESET_FLAG  '1'

#define RECOVER_FLAG	 'c'               //服务器恢复出厂标志
#define IP_SET_FLAG      's'               //IP设置标志
#define SER_RESET_FLAG   'r'               //服务器重启标志
#define UP_SYSTEM_FLAG   "upsystem"        //文件系统升级标志
#define UP_KERNEL_FLAG   "upkernel"        //内核升级标志

#define RS485_RCVE_FLAG  'r'               //RS484接收标志
#define RS485_SEND_FLAG  't'               //RS485发送标志

#define PORT_RS232       1
#define PORT_RS485       2
#define PORT_RS422       3

#define PORT1_DIR    "/mnt/config/cfg/port1.txt"
#define PORT2_DIR    "/mnt/config/cfg/port2.txt"
#define PORT3_DIR    "/mnt/config/cfg/port3.txt"
#define PORT4_DIR    "/mnt/config/cfg/port4.txt"
#define PORT5_DIR    "/mnt/config/cfg/port5.txt"
#define PORT6_DIR    "/mnt/config/cfg/port6.txt"
#define PORT7_DIR    "/mnt/config/cfg/port7.txt"
#define PORT8_DIR    "/mnt/config/cfg/port8.txt"

#define IP_DIR       "/mnt/config/cfg/ip.txt"
#define IP_SET_DIR   "/mnt/config/cfg/sys.txt"

#define GPIO_DEV     "/dev/gpio"
#define CPLD_DEV     "/dev/cpld"

#define DATABIT_OFFSET     8                 //串口数据位参数在配置文件中的位移
#define STOPBIT_OFFSET     10                //串口停止位参数在配置文件中的位移
#define PARITY_OFFSET      12
#define SER_IP_OFFSET      14
#define SER_PORT_OFFSET    30
#define LOCALPORT_OFFSET   36
#define SOCK_FLAG_OFFSET   42
#define IP_LIMIT_OFFSET    44
#define PORT_TYPE_OFFSET   46

#define IP_BIT_NUM         16                // IP信息的字符数
#define PORT_BIT_NUM       6
#define BAUD_BIT_NUM       8
#define OTHER_BIT_NUM      2
#define WRITE_DATA_NUM     16

#define IP_INFO_NUM        15                //IP限制信息的字符数
#define IP_SET_NUM         20

#define SOCKET_PORT        9999              //和WEB后台通讯的socket所用端口号
#define SOCKET_BUF_NUM     20

#define DATA_BUF_SIZE      1024              //数据中转buf的大小
#define UDP_BUF_SIZE       4096              //UDP发送buf的大小

#define RX8_ON_FLAG        2
#define RX8_OFF_FLAG       3
#define TX8_ON_FLAG        4
#define TX8_OFF_FLAG       5
#define RUN_ON_FLAG        6
#define RUN_OFF_FLAG       7

#define GPIO_LED_ON_FLAG   1
#define GPIO_LED_OFF_FLAG  0

#define GPIO_LED_VAL       0xff06000f

#define RX1_VAL            0x40000000
#define TX1_VAL            0x80000000
#define RX2_VAL            0x10000000
#define TX2_VAL            0x20000000
#define RX3_VAL            0x04000000
#define TX3_VAL            0x08000000
#define RX4_VAL            0x01000000
#define TX4_VAL            0x02000000
#define RX5_VAL            0x02
#define TX5_VAL            0x01
#define RX6_VAL            0x08
#define TX6_VAL            0x04
#define RX7_VAL            0x040000
#define TX7_VAL            0x020000

#define CMSPAR             010000000000          //奇偶校验标志

int sockfd[PORT_NUM], recv_sockfd[PORT_NUM], fcom[PORT_NUM], s_sockfd[PORT_NUM];

struct sockaddr_in local_addr, server_addr;

pthread_t thread_r[PORT_NUM];                    //tcp接收数据线程ID
pthread_t thread_s[PORT_NUM];                    //tcp发送数据线程ID
pthread_t udp_thread_r[PORT_NUM];                //udp接收数据线程ID
pthread_t udp_thread_s[PORT_NUM];                //udp发送数据线程ID
pthread_t thread_recv_m[PORT_NUM];               //tcp接收数据线程监控线程ID
pthread_t thread_send_m[PORT_NUM];               //tcp发送数据线程监控线程ID
pthread_t thread_sk,thread_w;                    //和web通信线程ID，串口主线程等待线程ID
pthread_t thread_m[PORT_NUM];                    //串口主线程ID


typedef struct _ip_info
{
	char 	ip[IP_INFO_NUM];
	char 	mask[IP_INFO_NUM];
	char 	read[IP_INFO_NUM];
	char 	write[IP_INFO_NUM];
	char    manage[IP_INFO_NUM];
}Ip_Info;

typedef struct _ip_set
{
	char ip[IP_SET_NUM];
	char mask[IP_SET_NUM];
	char gateway[IP_SET_NUM];
	char mac[IP_SET_NUM];
	char dns_a[IP_SET_NUM];
	char dns_b[IP_SET_NUM];
}User;

typedef struct _udp_data
{
	int port;
	struct sockaddr_in self;
	struct sockaddr_in dest;
}Udp_data;

int ComOpen(unsigned char *port);
int ComSetBaud(int fd, int baud);
int ComSetParity(int fd, int databits, int stopbits, char parity);
int ComInit(int baudrate, int databits, int stopbits, char parity, int com);
void create_recv(int port);
void *thread_recv(void *port);
void wait_recv(int port);
void create_send(int port);
void *thread_send(void *port);
void wait_send(int port);
void create_recv_monitoring(int port);
void *thread_recv_monitoring(void *port);
void wait_recv_monitoring(int port);
void create_send_monitoring(int port);
void *thread_send_monitoring(void *port);
void wait_send_monitoring(int port);
void read_limit_ip(FILE *pfile,Ip_Info *pversion);
char ip_limit_part(char *dip,int port);
int tcp_ser_do(int baud, int databits, int stopbits, char parity,
		      int port, char ip_limit, char limit, int serport);
int tcp_client_do(int baud, int databits, int stopbits, char parity,
		         int port, char ip_limit, char limit, char *dip, int serport);
int udp_do(int baud, int databits, int stopbits, char parity, int port,
		  char ip_limit, char limit, char *dip, int serport, int localport);
int port_do(int port);
void handle_quit();
void create_port(int port);
void *thread_port(void *port);
void wait_port(int port);
void creat_to_web();
int ip_set();
void *thread_to_web();
void wait_to_web();
void process_exit();
void create_wait_port(int port);
void *thread_wait_port(void *port);
void wait_wait_port(int port);
void port_full_do(int port);
void port_ro_do(int port);
void port_create(int port);
int led_on();
int led_off();
int port_led_rx(int port, int flag, int fp);
int port_led_tx(int port, int flag, int fp);
int cpld_port_type(int port, int type, int fd, char rs485_flag);
int type_port(int port);
int port_reset(int port);
void create_reset(int port);
void *thread_reset(void *port);
int get_mac(char* mac);
void udp_create_recv(Udp_data udp_data);
void *udp_thread_recv(void *udp_data);
void udp_wait_recv(int port);
void udp_create_send(Udp_data udp_data);
void *udp_thread_send(void *udp_data);
void udp_wait_send(int port);
void udp_port_full_do(Udp_data udp_data);
void udp_port_ro_do(Udp_data udp_data);

#endif
