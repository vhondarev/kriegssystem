#ifndef KS_COMBAT_H
#define KS_COMBAT_H

#include "vessels.h"

typedef struct
{
    vessel_raw_s *initiators;
    vessel_raw_s *deserters;
    vessel_raw_s *survivors;
} team_state_s;

void simulate_battle(darr_s *t1_raw, darr_s *t1_fleet, darr_s *t2_raw, darr_s *t2_fleet);

void shooting(vessel_state_s *vessel_state, darr_s *enemy_fleet);

void casualties(darr_s *fleet, uint16_t index);

#endif
