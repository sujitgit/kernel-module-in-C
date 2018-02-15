# kernel-module-in-C
implement a kernel module that creates a /dev/mytime character device. The character device should support the read() operation. When read() is called, it should return a single string that contains the result both current_kernel_time() and getnstimeofday() functions in the kernel.

Objective:

 Implement a kernel module that creates a /dev/mytime character device. The character device should support the read() operation. When read() is called, it should return a single string that contains the result both current_kernel_time() and getnstimeofday() functions in the kernel. For example, the output should be as follows:

current_kernel_time: 378959784 384958
getnstimeofday: 378959784 384990

Also, provide a user-level application program that compares the resolution of time reported by your module with that reported by user-level gettimeofday() library routine. Understand and explain the reason for any difference in your journal.

One such application could be written as follows (please fill in the missing code):

#define N ...

struct timeval gtodTimes[N];
char *procClockTimes[N];

...
/* allocate memory for character buffers HERE before you use them */

fd = open("/dev/mytime", O_RDONLY);
/* check for errors HERE */

for( i=0; i < N; i++)
{
	gettimeofday(&gtodTimes[i], 0);
	bytes_read = read(fd, procClockTime[i], MAX_LENGTH);
	/* check for errors HERE */
}

close(fd);

for(i=0; i < N; i++) {
	printf("...", gtodTimes[i], procClockTime[i]);
	/* fix the output format appropriately in the above line */
}

Run the user-level program for various values of N (both very small and very large). In your journal, explain the reason for any differences that you observe between the three timing mechanisms.
