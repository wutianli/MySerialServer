
#include "com_ser.h"


int main(int argc,char **argv)
{
	int p = 0;
	int fd_run,led_run_val[2];

	//��ʼ��led
	led_on();

	//����8�����ڵ����߳�
	for(p = 0; p < PORT_NUM; p++)
	{
		create_port(p);
	}

	//����8���������̵߳ĵȴ��߳�
	for(p = 0; p < PORT_NUM; p++)
	{
		create_wait_port(p);
	}

	//������WEB��̨ͨѶ�߳�
	creat_to_web();
	wait_to_web();


	//signal(SIGTERM, process_exit);
	//pause();

	//�ر�led
	led_off();

	return 0;
}

//����led
int led_on()
{
	int fd,led_on_val[2];

	fd = open(GPIO_DEV, O_RDWR);
	if (fd < 0)
	{
		printf("can't open GPIO_DEV!\n");
	}

	led_on_val[0] = GPIO_LED_ON_FLAG;
	led_on_val[1] = GPIO_LED_VAL;
	write(fd, led_on_val, sizeof(led_on_val));

	led_on_val[0] = RUN_ON_FLAG;
	write(fd, led_on_val, sizeof(led_on_val));

	led_on_val[0] = RX8_ON_FLAG;
	write(fd, led_on_val, sizeof(led_on_val));

	led_on_val[0] = TX8_ON_FLAG;
	write(fd, led_on_val, sizeof(led_on_val));

	return 0;
}

//�ر�led
int led_off()
{
	int fd_off,led_off_val[2];

	fd_off = open(GPIO_DEV, O_RDWR);
	if (fd_off < 0)
	{
		printf("can't open GPIO_DEV!\n");
	}

	led_off_val[0] = GPIO_LED_OFF_FLAG;
	led_off_val[1] = GPIO_LED_VAL;
	write(fd_off, led_off_val, sizeof(led_off_val));

	led_off_val[0] = RUN_OFF_FLAG;
	write(fd_off, led_off_val, sizeof(led_off_val));

	led_off_val[0] = RX8_OFF_FLAG;
	write(fd_off, led_off_val, sizeof(led_off_val));

	led_off_val[0] = TX8_OFF_FLAG;
	write(fd_off, led_off_val, sizeof(led_off_val));

	return 0;
}

/*�����˳�����*/
void process_exit()
{
	printf("the process was killed!\n");
	exit(0);
}

/*�߳��˳�����*/
void handle_quit()
{
    pthread_exit(NULL);
}

