// cJSON stubs for unit testing
#include <stdlib.h>
#include <string.h>

typedef struct cJSON { int valueint; char *valuestring; } cJSON;
cJSON *cJSON_CreateObject(void) { return malloc(sizeof(cJSON)); }
int cJSON_AddStringToObject(cJSON *obj, const char *key, const char *value) { return 1; }
void cJSON_Delete(cJSON *obj) { free(obj); }
cJSON *cJSON_CreateArray(void) { return malloc(sizeof(cJSON)); }
int cJSON_IsArray(const cJSON *obj) { return 1; }
int cJSON_GetArraySize(const cJSON *obj) { return 1; }
cJSON *cJSON_GetArrayItem(const cJSON *obj, int idx) { return (cJSON*)obj; }
cJSON *cJSON_Duplicate(const cJSON *obj, int recurse) { return malloc(sizeof(cJSON)); }
int cJSON_AddItemToArray(cJSON *array, cJSON *item) { return 1; }
cJSON *cJSON_GetObjectItem(const cJSON *obj, const char *key) { static cJSON dummy; dummy.valueint = 1; dummy.valuestring = "dummy"; return &dummy; }
char *cJSON_Print(const cJSON *obj) { return strdup("{\"dummy\":1}"); }
