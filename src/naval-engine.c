#include "naval/vessels.h"
#include "static/utils.h"
#include <dynamic_array.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Invalid arguments\n");
        printf("./naval-engine team [t]:[n] ... team [t]:[n]...\n");
        exit(EXIT_FAILURE);
    }

    bool is_error = false;
    uint8_t team_count = 0;
    int tmp_type = 0;
    int tmp_count = 0;
    vessel_type_e type;
    uint16_t count;

    darr_s *team1 = darr_create();
    if (malloc_failure_guard(team1, 0) == NULL)
    {
        exit(EXIT_FAILURE);
    }

    darr_s *team2 = darr_create();
    if (malloc_failure_guard(team2, 1, team1) == NULL)
    {
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "team") == 0)
        {
            team_count++;
            continue;
        }
        else if (team_count > 2)
        {
            perror("Too many teams\n");
            is_error = true;
        }
        else if (team_count == 0)
        {
            perror("Team settler argument absent.\n");
            is_error = true;
        }
        else if (sscanf(argv[i], "%d:%d", &tmp_type, &tmp_count) != 2 || tmp_type < 0 ||
                 tmp_count < 0 || tmp_type > VESSEL_TYPE_COUNT || tmp_count > USHRT_MAX)
        {
            perror("Invalid vessel type or vessels number\n");
            is_error = true;
        }

        if (is_error)
        {
            darr_destroy(team1);
            darr_destroy(team2);
            exit(EXIT_FAILURE);
        }

        vessel_raw_s *raw_vessel = malloc(sizeof(vessel_raw_s));

        if (raw_vessel == NULL)
        {
            darr_destroy(team1);
            darr_destroy(team2);
            malloc_failure_guard(raw_vessel, 0);
            exit(EXIT_FAILURE);
        }

        type = (vessel_type_e)tmp_type;
        count = (uint16_t)tmp_count;
        raw_vessel = init_vessel_raw(type, count);

        darr_append(team_count == 1 ? team1 : team2, raw_vessel);
    }

    darr_destroy(team1);
    darr_destroy(team2);

    return EXIT_SUCCESS;
}
