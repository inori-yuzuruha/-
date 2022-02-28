#include <ctime>
#include <stdio.h>
#pragma warning( disable : 4996 )
//将time_c格式的时间转化为字符串，格式为“年-月-日	时:分”
void timeToString(time_t t, char* pBuf)
{
	struct tm* pTimeInfo;
	pTimeInfo = localtime(&t);
	strftime(pBuf, 20, "%Y-%m-%d %H:%M", pTimeInfo);
}
time_t stringToTime(char* pTime)
{
	struct tm tm1;
	time_t time1;

	sscanf(pTime,"%d-%d-%d %d:%d",&tm1.tm_year,&tm1.tm_mon,&tm1.tm_mday,&tm1.tm_hour,&tm1.tm_min);

	tm1.tm_year -= 1900;
	tm1.tm_mon -= 1;
	tm1.tm_sec = 0;
	tm1.tm_isdst -= 1;

	time1 = mktime(&tm1);

	return time1;

}