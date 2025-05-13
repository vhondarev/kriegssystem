#ifndef KS_VESSELS__H
#define KS_VESSELS__H

#include <dynamic_array.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

extern uint16_t vessel_count;

typedef enum
{
    VESSEL_LONGHSIP,
    VESSEL_GALLEY,
    VESSEL_TYPE_COUNT,
} vessel_type_e;

typedef struct
{
    vessel_type_e type;
    uint16_t count;
} vessel_raw_s;

typedef struct
{
    vessel_type_e type;
    uint16_t hull;
    uint16_t rigging;
    uint16_t speed;
    uint16_t manoeuvre;
    uint16_t crew;
    uint16_t crew_dmg;
    uint16_t crew_def;
    uint16_t volleys;
    uint16_t volley_momentum;
    uint16_t projectile_dmg;
    uint16_t accuracy;
    uint16_t collision_dmg;
    uint16_t boarding_momentum;
    bool boarding;
} vessel_s;

typedef struct
{
    vessel_s *data;
    uint16_t id;
    uint16_t lock_target;
    uint16_t locked_by;
    uint8_t on_fire_round;
    bool on_fire;
    bool shocked;
    bool disabled;
    bool run_away;
} vessel_state_s;

vessel_raw_s *init_vessel_raw(vessel_type_e type, uint16_t count);

vessel_s *init_vessel(vessel_type_e type);

vessel_state_s *init_vessel_state(vessel_s *d);

bool vessel_parse_raw(int argc, char *argv[], darr_s *t1_parsed, darr_s *t2_parsed);

bool init_fleet(darr_s *raw, darr_s *fleet);

#endif
