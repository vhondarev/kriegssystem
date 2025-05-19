#include "naval/combat.h"
#include "naval/vessels.h"
#include "static/messages.h"
#include <dynamic_array.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf(MSG_INVALID_ARGUMENTS);
        printf(MSG_EXE_HINT);
        return EXIT_FAILURE;
    }

    srand(time(NULL));

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    clock_t time_start, time_end;
    time_start = clock();
#pragma GCC diagnostic pop

    darr_s *t1_raw = darr_create(), *t2_raw = darr_create(), *t1_fleet = darr_create(),
           *t2_fleet = darr_create();

    if (t1_raw == NULL || t2_raw == NULL)
    {
        goto error_cleanup;
    }

    if (!parse_vessel_raw(argc, argv, t1_raw, t2_raw))
    {
        goto error_cleanup;
    }

    if (!init_fleet(t1_raw, t1_fleet))
    {
        goto error_cleanup;
    }

    if (!init_fleet(t2_raw, t2_fleet))
    {
        goto error_cleanup;
    }

    simulate_combat(t1_raw, t1_fleet, t2_raw, t2_fleet);

    darr_destroy(t1_raw);
    darr_destroy(t2_raw);
    darr_destroy(t1_fleet);
    darr_destroy(t2_fleet);

    time_end = clock();
    printf(MSG_EXE_TIME_REPORT, (((double)(time_end - time_start)) / CLOCKS_PER_SEC));
    // PRINT(MSG_EXE_TIME_REPORT, (((double)(time_end - time_start)) / CLOCKS_PER_SEC));

    return EXIT_SUCCESS;

error_cleanup:
    darr_destroy(t1_raw);
    darr_destroy(t2_raw);
    darr_destroy(t1_fleet);
    darr_destroy(t2_fleet);
    return EXIT_FAILURE;
}
