#include "prototypes.h"
#include "vessels.h"

vessel_s prototypes[] = {
    {
        .type = VESSEL_LONGHSIP,
        .hull = 100,
        .hull_max = 100,
        .hull_armour = 2,
        .speed = 12,
        .manoeuvre = 18,
        .cargo = 20,
        .crew = 40,
        .crew_max = 40,
        .crew_dmg = 10,
        .crew_def = 4,
        .volleys = 10,
        .projectile_dmg = 4,
        .accuracy = 20,
        .collision_dmg = 20,
        .collision_def = 10,
        .boarding_momentum = 32,
        .boarding = true,
    },
    {
        .type = VESSEL_GALLEY,
        .hull = 120,
        .hull_max = 120,
        .hull_armour = 2,
        .speed = 8,
        .manoeuvre = 12,
        .cargo = 30,
        .crew = 120,
        .crew_max = 120,
        .crew_dmg = 6,
        .crew_def = 2,
        .volleys = 40,
        .projectile_dmg = 4,
        .accuracy = 20,
        .collision_dmg = 24,
        .collision_def = 10,
        .boarding_momentum = 28,
        .boarding = true,
    },
    {
        .type = VESSEL_COG,
        .hull = 80,
        .hull_max = 80,
        .hull_armour = 2,
        .speed = 8,
        .manoeuvre = 8,
        .cargo = 60,
        .crew = 20,
        .crew_max = 20,
        .crew_dmg = 4,
        .crew_def = 2,
        .volleys = 4,
        .projectile_dmg = 6,
        .accuracy = 24,
        .collision_dmg = 12,
        .collision_def = 6,
        .boarding_momentum = 0,
        .boarding = false,
    },
    {
        .type = VESSEL_KNARR,
        .hull = 150,
        .hull_max = 150,
        .hull_armour = 6,
        .speed = 10,
        .manoeuvre = 14,
        .cargo = 40,
        .crew = 30,
        .crew_max = 30,
        .crew_dmg = 4,
        .crew_def = 2,
        .volleys = 8,
        .projectile_dmg = 8,
        .accuracy = 32,
        .collision_dmg = 16,
        .collision_def = 8,
        .boarding_momentum = 10,
        .boarding = true,
    },
    {
        .type = VESSEL_CARRACK,
        .hull = 180,
        .hull_max = 180,
        .hull_armour = 6,
        .speed = 8,
        .manoeuvre = 12,
        .cargo = 120,
        .crew = 60,
        .crew_max = 60,
        .crew_dmg = 8,
        .crew_def = 2,
        .volleys = 24,
        .projectile_dmg = 8,
        .accuracy = 40,
        .collision_dmg = 24,
        .collision_def = 12,
        .boarding_momentum = 10,
        .boarding = true,
    },
    {
        .type = VESSEL_CLIPPER,
        .hull = 200,
        .hull_max = 200,
        .hull_armour = 8,
        .speed = 16,
        .manoeuvre = 24,
        .cargo = 300,
        .crew = 40,
        .crew_max = 40,
        .crew_dmg = 6,
        .crew_def = 2,
        .volleys = 12,
        .projectile_dmg = 10,
        .accuracy = 36,
        .collision_dmg = 16,
        .collision_def = 8,
        .boarding_momentum = 0,
        .boarding = false,
    },
    {
        .type = VESSEL_GALLEON,
        .hull = 260,
        .hull_max = 260,
        .hull_armour = 10,
        .speed = 8,
        .manoeuvre = 12,
        .cargo = 200,
        .crew = 180,
        .crew_max = 180,
        .crew_dmg = 8,
        .crew_def = 4,
        .volleys = 32,
        .projectile_dmg = 12,
        .accuracy = 32,
        .collision_dmg = 32,
        .collision_def = 18,
        .boarding_momentum = 24,
        .boarding = true,
    },
    {
        .type = VESSEL_FRIGATE,
        .hull = 300,
        .hull_max = 300,
        .hull_armour = 10,
        .speed = 14,
        .manoeuvre = 20,
        .cargo = 220,
        .crew = 240,
        .crew_max = 240,
        .crew_dmg = 8,
        .crew_def = 4,
        .volleys = 48,
        .projectile_dmg = 16,
        .accuracy = 54,
        .collision_dmg = 32,
        .collision_def = 24,
        .boarding_momentum = 12,
        .boarding = true,
    },
};

char *get_prototype_name(vessel_type_e type)
{
    switch (type)
    {
    case VESSEL_LONGHSIP:
        return "Longship";
    case VESSEL_GALLEY:
        return "Galley";
    case VESSEL_COG:
        return "Cog";
    case VESSEL_KNARR:
        return "Knarr";
    case VESSEL_CARRACK:
        return "Carrack";
    case VESSEL_CLIPPER:
        return "Clipper";
    case VESSEL_GALLEON:
        return "Galleon";
    case VESSEL_FRIGATE:
        return "Frigate";
    case VESSEL_TYPE_COUNT:
    default:
        return "Unknown";
    }
}

bool apply_prototype(vessel_type_e type, vessel_s *v)
{
    if (type < 0 || type >= VESSEL_TYPE_COUNT || v == NULL)
        return false;

    v->type = prototypes[type].type;
    v->hull = prototypes[type].hull;
    v->hull_max = prototypes[type].hull_max;
    v->hull_armour = prototypes[type].hull_armour;
    v->speed = prototypes[type].speed;
    v->manoeuvre = prototypes[type].manoeuvre;
    v->cargo = prototypes[type].cargo;
    v->crew = prototypes[type].crew;
    v->crew_max = prototypes[type].crew_max;
    v->crew_dmg = prototypes[type].crew_dmg;
    v->crew_def = prototypes[type].crew_def;
    v->volleys = prototypes[type].volleys;
    v->projectile_dmg = prototypes[type].projectile_dmg;
    v->accuracy = prototypes[type].accuracy;
    v->collision_dmg = prototypes[type].collision_dmg;
    v->collision_def = prototypes[type].collision_def;
    v->boarding_momentum = prototypes[type].boarding_momentum;
    v->boarding = prototypes[type].boarding;

    return true;
}
