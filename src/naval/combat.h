#ifndef KS_COMBAT_H
#define KS_COMBAT_H

#include "dynamic_array.h"
#include "vessels.h"
#include <stdint.h>

void simulate_combat(darr_s *fl1, darr_s *fl2, darr_s *t1_raw, darr_s *t2_raw);

void process_shooting(darr_s *fl1, darr_s *fl2);

void process_casualties(darr_s *fl1, darr_s *fl2);

void process_boarding(darr_s *fl1, darr_s *fl2);

void process_round_end(uint8_t r, darr_s *fl1, darr_s *fl2);

void process_battle_end(uint8_t r, darr_s *fl1, darr_s *fl2, darr_s *t1_raw, darr_s *t2_raw);

void shooting(vessel_state_s *vs, darr_s *fl);

uint16_t calc_available_volleys(vessel_s *v);

void resolve_fleet_status(darr_s *fl);

void destroy_vessel(darr_s *fl, uint16_t index);

void boarding(vessel_state_s *vs, darr_s *fl);

vessel_state_s *find_boarding_target(darr_s *fl);

void resolve_collision_damage(vessel_state_s *v1, vessel_state_s *v2);

uint16_t calc_collision_dmg(vessel_s *v1, vessel_s *v2);

uint16_t calc_vessel_speed(vessel_s *v);

uint16_t max_dmg(uint16_t hp, uint16_t total_dmg);

void resolve_crew_fight(vessel_state_s *v1, vessel_state_s *v2);

uint16_t calc_crew_damage(vessel_s *v1, vessel_s *v2);

uint16_t calc_crew_losses(vessel_state_s *vs1, uint16_t integrity_dmg);

bool check_crew_surrender(vessel_state_s *v);

#endif
