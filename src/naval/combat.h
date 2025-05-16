#ifndef KS_COMBAT_H
#define KS_COMBAT_H

#include "dynamic_array.h"
#include "vessels.h"

typedef struct
{
    vessel_raw_s *initiators;
    vessel_raw_s *deserters;
    vessel_raw_s *survivors;
} team_state_s;

void simulate_battle(darr_s *t1_raw, darr_s *t1_fleet, darr_s *t2_raw, darr_s *t2_fleet);

void process_shooting(darr_s *fl1, darr_s *fl2);

void process_casualties(darr_s *fl1, darr_s *fl2);

void process_boarding(darr_s *fl1, darr_s *fl2);

void round_report(uint8_t round, darr_s *fl1, darr_s *fl2);

void battle_report(uint8_t round, darr_s *fl1, darr_s *fl2);

void shooting(vessel_state_s *vessel_state, darr_s *fl);

void destroy_vessel(darr_s *fleet, uint16_t index);

void boarding(vessel_state_s *vessel_state, darr_s *fl);

vessel_state_s *find_boarding_target(darr_s *fl);

void resolve_collision_damage(vessel_state_s *v1, vessel_state_s *v2);

uint16_t apply_armor_reduction(uint16_t dmg, uint16_t armor);

uint16_t apply_collision_damage(uint16_t current_hull, uint16_t dmg);

void resolve_crew_fight(vessel_state_s *v1, vessel_state_s *v2);

uint16_t calculate_crew_damage(vessel_state_s *v1, vessel_state_s *v2);

void check_crew_surrender(vessel_state_s *v);

#endif
