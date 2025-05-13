#include "vessels.h"
#include "../static/utils.h"
#include <stdlib.h>
#include <string.h>

uint16_t vessel_count = 0;

vessel_raw_s *init_vessel_raw(vessel_type_e type, uint16_t count)
{
    vessel_raw_s *v = malloc(sizeof(vessel_raw_s));
    if (malloc_failure_guard(v, 0) == NULL)
        return v;

    v->type = type;
    v->count = count;

    return v;
}

vessel_s *init_vessel(vessel_type_e type)
{
    vessel_s *v = malloc(sizeof(vessel_s));
    if (malloc_failure_guard(v, 0) == NULL)
        return v;

    switch (type)
    {
    case VESSEL_LONGHSIP:
        v->type = VESSEL_LONGHSIP;
        v->hull = 50;
        v->rigging = 30;
        v->speed = 6;
        v->manoeuvre = 10;
        v->crew = 60;
        v->crew_dmg = 2;
        v->crew_def = 1;
        v->volleys = 10;
        v->volley_momentum = 0;
        v->projectile_dmg = 1;
        v->accuracy = 40;
        v->collision_dmg = 20;
        v->boarding_momentum = 1;
        v->boarding = true;
        return v;
    case VESSEL_GALLEY:
        v->type = VESSEL_GALLEY;
        v->hull = 60;
        v->rigging = 30;
        v->speed = 4;
        v->manoeuvre = 5;
        v->crew = 120;
        v->crew_dmg = 1;
        v->crew_def = 0;
        v->volleys = 60;
        v->volley_momentum = 1;
        v->projectile_dmg = 1;
        v->accuracy = 40;
        v->collision_dmg = 10;
        v->boarding_momentum = 0;
        v->boarding = true;
        return v;
    case VESSEL_TYPE_COUNT:
        break;
    }

    perror("Wrong vessel type.");
    return NULL;
}

vessel_state_s *init_vessel_state(vessel_s *d)
{
    if (d == NULL)
    {
        perror("Initialize vessel stait failed because of NULL data.");
        return NULL;
    }

    vessel_state_s *vs = malloc(sizeof(vessel_state_s));
    if (malloc_failure_guard(vs, 0) == NULL)
        return vs;

    if (vessel_count == USHRT_MAX)
    {
        free(vs);
        perror("Reached maximum vessel count.");
        return NULL;
    }

    vs->data = d;
    vs->id = vessel_count++;
    vs->lock_target = USHRT_MAX;
    vs->locked_by = USHRT_MAX;
    vs->locked_by = USHRT_MAX;
    vs->on_fire_round = 0;
    vs->on_fire = false;
    vs->shocked = false;
    vs->disabled = false;
    vs->run_away = false;

    return vs;
}

bool vessel_parse_raw(int argc, char *argv[], darr_s *t1_parsed, darr_s *t2_parsed)
{
    bool is_error = false;
    uint8_t team_count = 0;
    int tmp_type = 0;
    int tmp_count = 0;
    vessel_type_e type;
    uint16_t count;

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
            darr_destroy(t1_parsed);
            darr_destroy(t2_parsed);
            return false;
        }

        vessel_raw_s *raw_vessel = malloc(sizeof(vessel_raw_s));

        if (raw_vessel == NULL)
        {
            darr_destroy(t1_parsed);
            darr_destroy(t2_parsed);
            malloc_failure_guard(raw_vessel, 0);
            return false;
        }

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
                return false;
            }
            darr_append(fleet, vstate);
        }
    }

    return true;
}
