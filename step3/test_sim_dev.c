/**
* Name: Philip Brunet
* Lab/task: Project 2
* Date: 05/09/19
**/
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>

#include "sim_dev.h"

/***
 * Generates random readable/printable content for testing
 */

char *generateContent(int size)
{
    char *content = malloc(size);

    int firstPrintable = ' ';
    int len = '~' - firstPrintable;

    for (int i = 0; i < size - 1; i++)
        *(content + i) = firstPrintable + rand() % len;

    content[size - 1] = '\0';
    return content;
}

#define oops(msg, errnum) { perror(msg); exit(errnum); }

#define BUFSIZE 4096

int main(void)
{
	int i, fd, len, wlen, tmp, tmp2;

	// test message to send to the device
	char msg[] = "Skeleton Kernel Module Test";
	char receive_buffer[BUFSIZE];

	// variables for holding device control data
	// int ioctl_control_data, ioctl_status_data;

	// open the I/O channel to the device
	fd = open("/dev/simdev", O_RDWR | O_SYNC);
	if ( fd == -1)
		oops("Unable to open device...", 1);

	// test device write function
	len = write(fd, msg, strlen(msg) + 1);
	if ( len == -1 )
		oops("Unable to write to the simuated device.\n", 2);

	printf("'%s' written to /dev/simdev\n", msg);

	// test device read function
	len = read(fd, receive_buffer, BUFSIZE);
	if ( len == -1 )
		oops("Unable to read from the simulated device.\n", 3);

	printf("'%s' read from /dev/simdev\n", receive_buffer);

	// test ioctl with the device

	// this is a sample content/command to send to the device
	// ioctl_control_data = 0xABCDEFEF;
	DISK_REGISTER sim_dev_register = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	sim_dev_register.track = 100;
	sim_dev_register.sector = 20;
	sim_dev_register.command = 0xA; // arbitrary example
	ioctl(fd, IOCTL_SIM_DEV_WRITE, &sim_dev_register);

	// receive status from the device
	// ioctl(fd, IOCTL_SIM_DEV_WRITE, &ioctl_control_data);
	ioctl(fd, IOCTL_SIM_DEV_READ, &sim_dev_register);
	printf("DEVICE RESPONSE: '%s'\n",
	       !sim_dev_register.error_occured ? "OK" : "ERROR");

	close(fd);
}

