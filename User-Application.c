#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#define N 10
#define MAX_LENGTH 500
static char *procClockTimes[N];

int main(int argc, char *argv[]) {
	struct timeval gtodTimes[N];
	int fd;
	int i = 0;
	int bytes_read=0;
	//...
	for (i=0; i<N; i++) {
		procClockTimes[i] = malloc(500 * sizeof(char));
	}
	for (i=0; i<N; i++) {
		strcpy(procClockTimes[i], "dasfsfsff");
		printf("procClockTimes: %s\n", procClockTimes[i]);
	}
	fd = open("/dev/mytime", O_RDONLY);
	if(fd<0) {
		printf("There was an error opening the file, fd is %d\n", fd);
		exit(1);
	}
	/* check for errors HERE */

	for( i=0; i < N; i++)
	{
		//printf("\nReading the current time in user application\n");
		gettimeofday(&gtodTimes[i], 0);
		//printf("\nReading the charcacter device which writes the current time to global static var procClockTimes[i]\n");
		bytes_read = read(fd, procClockTimes[i], MAX_LENGTH);
		printf("DEBUG DEBG");
		if (bytes_read<0) {
			printf("\nThere was an error reading the file\n");
			exit(1);
		}
		/* check for errors HERE */
	}

	close(fd);

	for(i=0; i < N; i++) {
		printf("\nFOR the %dth interation", (i+1));
		printf("\nFROM USER APPLICATION: gettimeofday:\n tv_sec: %ld tv_usec: %ld\n", gtodTimes[i].tv_sec, gtodTimes[i].tv_usec);
		printf("\nFROM KERNEL: \n %s\n\n\n",procClockTimes[i]);
	}
	for (i=0; i<N; i++) {
		free(procClockTimes[i]);
	}
	return 0;
}

