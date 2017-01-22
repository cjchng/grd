/*
ime_t 	st_atime 	The most recent time the file was accessed.
time_t 	st_mtime 	The most recent time the contents of the file were changed.
time_t 	st_ctime 	The most recent time the status of the file was changed.
*/
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>

struct timespec get_modify_time(char *path, struct stat stat_buf, char* clock_str,  int *error_flag){
	printf("\nget_modify_time file is: %s\n", path);
	if (lstat(path, &stat_buf) < 0) {
		printf("lstat error\n");
		*error_flag= 0 ;
		//exit(-1);
	}
	strcpy(clock_str, ctime(&stat_buf.st_mtim));
	char *ap=clock_str ;
	while( *(++ap) != '\n')
	{
		if(*ap==' ')
			*ap = '_';
	}
	*ap = '\0' ;
	//printf("\n\nclock_str is [%s]\n", clock_str);
	return (stat_buf.st_mtim) ;
}


int size_of_file(char *name)
{

	struct stat	buf;
	if (lstat(name, &buf) < 0) {
		printf("lstat error");
	}
	printf("buf.st_size is %d",buf.st_size );
	return buf.st_size ;
}
// http://www.binarytides.com/get-time-difference-in-microtime-in-c/
//The function gettimeofday can be used to retrieve the total number of seconds and balance microseconds elapsed since EPOCH.

//Structure timeval looks like this :

//struct timeval {
//    time_t      tv_sec;     /* seconds */
//    suseconds_t tv_usec;    /* microseconds */
//};


//On Linux it can be done like this :

/**
 * Get time difference in microseconds
 * */

//#include <stdio.h>


double time_diff(struct timeval x , struct timeval y);
/*
int main()
{
	int i;

	struct timeval before , after;
	gettimeofday(&before , NULL);

	//Time taking task
	for (i=1 ; i <= 100 ; i++)
	{
		printf("%d %d %d \n",i, i*i, i*i*i);
	}

	gettimeofday(&after , NULL);

	printf("Total time elapsed : %.0lf us" , time_diff(before , after) );

	return 0;
}
*/
double time_diff(struct timeval x , struct timeval y)
{
	double x_ms , y_ms , diff;

	x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
	y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;

	diff = (double)y_ms - (double)x_ms;

	return diff;
}

