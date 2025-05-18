#include "vessels.h"
#include "../static/messages.h"
#include "../static/utils.h"
#include "prototypes.h"
#include "stdio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEAM_SETTLER "team"

uint16_t vessel_count = 0;

vessel_raw_s *init_vessel_raw(vessel_type_e type, uint16_t count)
{
    vessel_raw_s *v = malloc(sizeof(vessel_raw_s));
    if (malloc_failure_guard(v))
        return v;

    v->type = type;
    v->count = count;

    return v;
}

vessel_s *init_vessel(vessel_type_e type)
{
    vessel_s *v = malloc(sizeof(vessel_s));
    if (malloc_failure_guard(v))
        return v;

    if (!apply_prototype(type, v))
    {
        perror(MSG_ERR_VESSEL_TYPE);
        free(v);
        return NULL;
    }

    return v;
}

vessel_state_s *init_vessel_state(vessel_s *d)
{
    if (d == NULL)
    {
        perror(MSG_ERR_VESSEL_NULL_INIT);
        return NULL;
    }

    vessel_state_s *vs = malloc(sizeof(vessel_state_s));
    if (malloc_failure_guard(vs))
        return vs;

    if (vessel_count == USHRT_MAX)
    {
        free(vs);
        perror(MSG_ERR_VESSEL_LIMIT);
        return NULL;
    }

    vs->data = d;
    vs->on_fire_round = 0;
    vs->on_fire = false;
    vs->boarded_count = 0;
    vs->shocked = false;
    vs->running = false;

    return vs;
}

bool parse_vessel_raw(int argc, char *argv[], darr_s *t1_parsed, darr_s *t2_parsed)
{
    bool is_error = false;
    uint8_t team_count = 0;
    int tmp_type = 0;
    int tmp_count = 0;
    vessel_type_e type;
    uint16_t count;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], TEAM_SETTLER) == 0)
        {
            team_count++;
            continue;
        }
        else if (team_count > 2)
        {
            perror(MSG_ERR_PARSE_LIMIT);
            is_error = true;
        }
        else if (team_count == 0)
        {
            perror(MSG_ERR_PARSE_SETTLER);
            is_error = true;
        }
        else if (sscanf(argv[i], "%d:%d", &tmp_type, &tmp_count) != 2 || tmp_type < 0 ||
                 tmp_count < 0 || tmp_type > VESSEL_TYPE_COUNT || tmp_count > USHRT_MAX)
        {
            perror(MSG_ERR_PARSE_TYPE_OR_QUANTITY);
            is_error = true;
        }

        if (is_error)
            return false;

        vessel_raw_s *raw_vessel = malloc(sizeof(vessel_raw_s));
        if (malloc_failure_guard(raw_vessel))
            return false;

        type = (vessel_type_e)tmp_type;
        count = (uint16_t)tmp_count;
        raw_vessel = init_vessel_raw(type, count);

        darr_append(team_count == 1 ? t1_parsed : t2_parsed, raw_vessel);
    }

    return true;
}

bool init_fleet(darr_s *raw, darr_s *fleet)
{
    for (size_t i = 0; i < raw->size; i++)
    {
        vessel_raw_s *r = raw->data[i];
        for (size_t j = 0; j < r->count; j++)
        {
            vessel_s *vessel = init_vessel(r->type);
            vessel_state_s *vstate = init_vessel_state(vessel);
            if (vessel == NULL || vstate == NULL)
            {
                perror(MSG_ERR_FLEET_INIT);
                return false;
            }
            darr_append(fleet, vstate);
        }
    }

    if (fleet->size == 0)
    {
        perror(MSG_ERR_FLEET_EMPTY);
        return false;
    }

    return true;
}
