#include "mock_time.h"
#include <time.h>

// Default to real functions
static time_t real_time(time_t *t) { return time(t); }
static struct tm *real_localtime(const time_t *timep) { return localtime(timep); }
static time_t real_mktime(struct tm *tm) { return mktime(tm); }
static double real_difftime(time_t time1, time_t time0) { return difftime(time1, time0); }

time_t (*mock_time_func)(time_t *t) = real_time;
struct tm *(*mock_localtime_func)(const time_t *timep) = real_localtime;
time_t (*mock_mktime_func)(struct tm *tm) = real_mktime;
double (*mock_difftime_func)(time_t time1, time_t time0) = real_difftime;
