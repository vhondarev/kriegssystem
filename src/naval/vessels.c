#include "vessels.h"
#include "../static/utils.h"

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
