#include "naval/vessels.h"
#include "static/utils.h"
#include <dynamic_array.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    printf("ARGS: %d\n", argc);
    if (argc < 2)
    {
        printf("Invalid arguments\n");
        printf("./naval-engine team [t]:[n] ... team [t]:[n]...\n");
        exit(EXIT_FAILURE);
    }

    uint8_t team_count = 0;
    int tmp_type = 0;
    int tmp_count = 0;
    vessel_type_e type;
    uint16_t count;

    darr_s *team1 = darr_create();
    malloc_failure_guard(team1, 0);
    darr_s *team2 = darr_create();
    malloc_failure_guard(team2, 1, team1);

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
            exit(EXIT_FAILURE);
        }
        else if (team_count == 0)
        {
            perror("Team settler argument absent.\n");
            exit(EXIT_FAILURE);
        }
        if (sscanf(argv[i], "%d:%d", &tmp_type, &tmp_count) != 2 || tmp_type < 0 || tmp_count < 0 ||
            tmp_type > VESSEL_TYPE_COUNT || tmp_count > USHRT_MAX)
        {
            printf("type: %d, count: %d", tmp_type, tmp_count);
            perror("Invalid vessel type or vessels number\n");
            exit(EXIT_FAILURE);
        }

        vessel_raw_s *raw_vessel = malloc(sizeof(vessel_raw_s));

        if (raw_vessel == NULL)
        {
            darr_destroy(team1);
            darr_destroy(team2);
            malloc_failure_guard(raw_vessel, 0);
        }

        type = (vessel_type_e)tmp_type;
        count = (uint16_t)tmp_count;
        raw_vessel = init_vessel_raw(type, count);

        darr_append(team_count == 1 ? team1 : team2, raw_vessel);
    }

    printf("t1size: %zu, t2size: %zu\n", team1->size, team2->size);

    for (size_t i = 0; i < team1->size; i++)
    {
        printf("vessel_type: %d\t vessel_count: %d\n", ((vessel_raw_s *)team1->data[i])->type,
               ((vessel_raw_s *)team1->data[i])->count);
    }
    printf("==========\n");
    for (size_t i = 0; i < team2->size; i++)
    {
        printf("vessel_type: %d\t vessel_count: %d\n", ((vessel_raw_s *)team2->data[i])->type,
               ((vessel_raw_s *)team2->data[i])->count);
    }

    darr_destroy(team1);
    darr_destroy(team2);

    return EXIT_SUCCESS;
}
