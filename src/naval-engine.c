#include "naval/vessels.h"
#include <dynamic_array.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Invalid arguments\n");
        printf("./naval-engine team [t]:[n] ... team [t]:[n]...\n");
        exit(EXIT_FAILURE);
    }

    darr_s *t1_raw = darr_create(), *t2_raw = darr_create(), *t1_fleet = darr_create(),
           *t2_fleet = darr_create();

    if (t1_raw == NULL || t2_raw == NULL)
    {
        goto error_cleanup;
    }

    if (!vessel_parse_raw(argc, argv, t1_raw, t2_raw))
    {
        goto error_cleanup;
    }

    if (!init_fleet(t1_raw, t1_fleet))
    {
        goto error_cleanup;
    }

    if (!init_fleet(t1_raw, t2_fleet))
    {
        goto error_cleanup;
    }

    // Combat simuation
    // TODO

    darr_destroy(t1_raw);
    darr_destroy(t2_raw);
    darr_destroy(t1_fleet);
    darr_destroy(t2_fleet);

    return EXIT_SUCCESS;

error_cleanup:
    darr_destroy(t1_raw);
    darr_destroy(t2_raw);
    darr_destroy(t1_fleet);
    darr_destroy(t2_fleet);
    return EXIT_FAILURE;
}
