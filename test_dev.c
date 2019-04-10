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
	char buff[1024];   //缓冲区
	char buf[1024];    //写入的信息
	fd = open("/dev/wangsen_dev", O_RDWR | O_NONBLOCK);

	if (fd == -1)
	{
		printf("Device open error!\n");
		return -1;
	}
	else {
		//读初始信息
		read(fd, buff, sizeof(buff));
		printf("%s\n", buff);

		//写信息
		printf("Please input:");
		scanf("%s", buf);
		write(fd, buf, sizeof(buf));

		//重定向到开始后两个字符位置
		lseek(fd, 2, SEEK_SET);   //从头开始读字符
		printf("dev's message: %s\n", buff);

		//读设备信息
		read(fd, buff, sizeof(buff));
		printf("%s\n", buff);
		close(fd);
		return 0;
	}

}
