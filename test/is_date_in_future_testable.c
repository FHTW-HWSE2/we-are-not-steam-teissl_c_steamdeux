#include <time.h>
#include <string.h>
#ifdef UNITTEST_MOCK_TIME
#include "../test/mocks/mock_time.h"
#define time mock_time
#define localtime mock_localtime
#define mktime mock_mktime
#define difftime mock_difftime
#endif

int is_date_in_future(const char* date_str) {
    if (!date_str) return 0;
    struct tm date_tm = {0};
    if (!strptime(date_str, "%d.%m.%Y", &date_tm)) return 0;
    date_tm.tm_isdst = -1;
    time_t now = time(NULL);
    struct tm now_tm = *localtime(&now);
    now_tm.tm_hour = 0; now_tm.tm_min = 0; now_tm.tm_sec = 0;
    now_tm.tm_isdst = -1;
    time_t today = mktime(&now_tm);
    time_t date_time = mktime(&date_tm);
    return difftime(date_time, today) >= 0;
}
