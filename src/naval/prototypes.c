#include "prototypes.h"
#include "vessels.h"

char *get_prototype_name(vessel_type_e type)
{
    switch (type)
    {
    case VESSEL_LONGHSIP:
        return "Longship";
    case VESSEL_GALLEY:
        return "Galley";
    default:
        return "Unknown";
    }
}

bool apply_prototype(vessel_type_e type, vessel_s *v)
{
    switch (type)
    {
    case VESSEL_LONGHSIP:
        v->type = VESSEL_LONGHSIP;
        v->hull = 60;
        v->hull_max = 60;
        v->hull_armour = 2;
        // v->rigging = 30;
        v->speed = 10;
        v->manoeuvre = 4;
        v->crew = 60;
        v->crew_max = 60;
        v->crew_dmg = 4;
        v->crew_def = 2;
        v->volleys = 20;
        v->volley_momentum = 0;
        v->projectile_dmg = 2;
        v->accuracy = 40;
        v->collision_dmg = 26;
        v->collision_def = 10;
        v->boarding_momentum = 26;
        v->boarding = true;
        break;
    case VESSEL_GALLEY:
        v->type = VESSEL_GALLEY;
        v->hull = 40;
        v->hull_max = 40;
        v->hull_armour = 2;
        // v->rigging = 30;
        v->speed = 8;
        v->manoeuvre = 10;
        v->crew = 120;
        v->crew_max = 120;
        v->crew_dmg = 2;
        v->crew_def = 0;
        v->volleys = 60;
        v->volley_momentum = 10;
        v->projectile_dmg = 4;
        v->accuracy = 40;
        v->collision_dmg = 16;
        v->collision_def = 6;
        v->boarding_momentum = 10;
        v->boarding = true;
        break;
    case VESSEL_TYPE_COUNT:
    default:
        return false;
    }

    return true;
}
