#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
int main(void)
{
	int fd;
	char buff[1024];   //������
	char buf[1024];    //д�����Ϣ
	fd = open("/dev/wangsen_dev", O_RDWR | O_NONBLOCK);

	if (fd == -1)
	{
		printf("Device open error!\n");
		return -1;
	}
	else {
		//����ʼ��Ϣ
		read(fd, buff, sizeof(buff));
		printf("%s\n", buff);

		//д��Ϣ
		printf("Please input:");
		scanf("%s", buf);
		write(fd, buf, sizeof(buf));

		//�ض��򵽿�ʼ�������ַ�λ��
		lseek(fd, 2, SEEK_SET);   //��ͷ��ʼ���ַ�
		printf("dev's message: %s\n", buff);

		//���豸��Ϣ
		read(fd, buff, sizeof(buff));
		printf("%s\n", buff);
		close(fd);
		return 0;
	}

}
