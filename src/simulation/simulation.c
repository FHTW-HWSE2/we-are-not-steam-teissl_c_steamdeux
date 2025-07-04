#define _XOPEN_SOURCE
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "../inc/data/data.h"
#include "simulation.h"

void start_simulation() {
    int choice, duration;
    time_t sim_now = time(NULL); // Startzeit für Simulation
    char datebuf[64];
    while (1) {
        // Aktuelles Simulationsdatum/Uhrzeit anzeigen
        strftime(datebuf, sizeof(datebuf), "%Y-%m-%d %H:%M:%S", localtime(&sim_now));
        printf("\n--- Simulation Menu ---\n");
        printf("Aktuelles Simulationsdatum: %s\n", datebuf);
        printf("1. Simulate days\n");
        printf("2. Simulate weeks\n");
        printf("3. Simulate months\n");
        printf("0. Exit\n");
        printf("Your choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (choice == 0) {
            printf("Exiting simulation menu...\n");
            return;
        }
        printf("Enter amount: ");
        if (scanf("%d", &duration) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        int days = 0;
        switch (choice) {
            case 1: days = duration; break;
            case 2: days = duration * 7; break;
            case 3: days = duration * 30; break;
            default:
                printf("Invalid choice.\n");
                continue;
        }

        FILE *uf = fopen(USERS_JSON_PATH, "r");
        if (!uf) { printf("Fehler beim Öffnen von users.json\n"); return; }
        fseek(uf, 0, SEEK_END);
        long usize = ftell(uf);
        fseek(uf, 0, SEEK_SET);
        char *udata = malloc(usize + 1);
        fread(udata, 1, usize, uf);
        udata[usize] = '\0';
        fclose(uf);
        cJSON *users = cJSON_Parse(udata);
        free(udata);
        if (!users) { printf("User-JSON Fehler\n"); return; }

        FILE *gf = fopen(GAMES_JSON_PATH, "r");
        if (!gf) { printf("Fehler beim Öffnen von games.json\n"); return; }
        fseek(gf, 0, SEEK_END);
        long gsize = ftell(gf);
        fseek(gf, 0, SEEK_SET);
        char *gdata = malloc(gsize + 1);
        fread(gdata, 1, gsize, gf);
        gdata[gsize] = '\0';
        fclose(gf);
        cJSON *gjson = cJSON_Parse(gdata);
        free(gdata);
        cJSON *games = cJSON_GetObjectItem(gjson, "games");

        FILE *rf = fopen(REPORTS_JSON_PATH, "r");
        cJSON *report_arr = NULL;
        if (rf) {
            fseek(rf, 0, SEEK_END);
            long rsize = ftell(rf);
            fseek(rf, 0, SEEK_SET);
            char *rdata = malloc(rsize + 1);
            fread(rdata, 1, rsize, rf);
            rdata[rsize] = '\0';
            fclose(rf);
            report_arr = cJSON_Parse(rdata);
            free(rdata);
        }
        if (!report_arr) report_arr = cJSON_CreateArray();

        srand(time(NULL));
        int total_days = days;
        // Simulationszeitpunkt für diese Runde
        time_t now = sim_now;

        for (int i = 0; i < cJSON_GetArraySize(users); ++i) {
            cJSON *user = cJSON_GetArrayItem(users, i);
            if (!user) continue;
            const char *tag = cJSON_GetObjectItem(user, "gamertag")->valuestring;
            const char *end = cJSON_GetObjectItem(user, "subscription_end_date")->valuestring;
            int is_active = cJSON_GetObjectItem(user, "is_subscribed")->valueint;

            struct tm abo_tm = {0};
            strptime(end, "%d.%m.%Y", &abo_tm);
            time_t abo_end_time = mktime(&abo_tm);
            if (difftime(abo_end_time, now) < 0) continue;

            for (int j = 0; j < cJSON_GetArraySize(games); ++j) {
                cJSON *game = cJSON_GetArrayItem(games, j);
                if (!game) continue;
                const char *title = cJSON_GetObjectItem(game, "title")->valuestring;
                const char *mode = cJSON_GetObjectItem(game, "mode")->valuestring;

                int session_minutes = rand() % 121 + 30;
                cJSON *session = cJSON_CreateObject();
                cJSON_AddStringToObject(session, "gamertag", tag);
                cJSON_AddStringToObject(session, "game", title);
                cJSON_AddNumberToObject(session, "duration_minutes", session_minutes);

                char timebuf[64];
                time_t sim_time = now - (rand() % (total_days * 86400));
                strftime(timebuf, sizeof(timebuf), "%Y-%m-%dT%H:%M:%S", localtime(&sim_time));
                cJSON_AddStringToObject(session, "timestamp", timebuf);

                if (strstr(mode, "Multi")) {
                    cJSON_AddNumberToObject(session, "kills", rand() % 20);
                    cJSON_AddNumberToObject(session, "deaths", rand() % 15);
                    cJSON_AddNumberToObject(session, "assists", rand() % 10);
                } else {
                    cJSON_AddNumberToObject(session, "score", rand() % 1000);
                    cJSON_AddNumberToObject(session, "progress", rand() % 101);
                }

                cJSON_AddItemToArray(report_arr, session);
            }
        }

        FILE *outf = fopen(REPORTS_JSON_PATH, "w");
        if (!outf) { printf("Fehler beim Speichern der Reports\n"); return; }
        char *outdata = cJSON_Print(report_arr);
        fputs(outdata, outf);
        fclose(outf);
        free(outdata);

        cJSON_Delete(users);
        cJSON_Delete(gjson);
        cJSON_Delete(report_arr);

        // Simulationszeit fortschreiben
        sim_now = now + days * 86400;
        printf("Simulation abgeschlossen. Sessions gespeichert.\n");
        // Nach einer Simulation zurück ins Menü (Schleife bleibt erhalten)
    }
}
