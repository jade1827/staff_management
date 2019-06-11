#include<stdio.h>
#include<string.h>
#include<time.h>
#include<unistd.h>

int main(int argc, const char *argv[])
{
	time_t now;
	struct tm *tm_now;
    char date[128]={'0'};
	time(&now);

	tm_now=localtime(&now);
	sprintf(date,"%d-%d-%d %d:%d:%d",tm_now->tm_year+1900,tm_now->tm_mon+1,tm_now->tm_mday,\
			tm_now->tm_hour,tm_now->tm_min,tm_now->tm_sec);
      printf("%s\n",date);

	return 0;
}
