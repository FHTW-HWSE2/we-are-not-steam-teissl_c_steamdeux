#ifndef MOCK_TIME_H
#define MOCK_TIME_H

#include <time.h>

// Function pointers for mocking
extern time_t (*mock_time_func)(time_t *t);
extern struct tm *(*mock_localtime_func)(const time_t *timep);
extern time_t (*mock_mktime_func)(struct tm *tm);
extern double (*mock_difftime_func)(time_t time1, time_t time0);

// Wrappers
static inline time_t mock_time(time_t *t) { return mock_time_func(t); }
static inline struct tm *mock_localtime(const time_t *timep) { return mock_localtime_func(timep); }
static inline time_t mock_mktime(struct tm *tm) { return mock_mktime_func(tm); }
static inline double mock_difftime(time_t time1, time_t time0) { return mock_difftime_func(time1, time0); }

#endif // MOCK_TIME_H
