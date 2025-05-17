#include "combat.h"
#include "../static/messages.h"
#include "vessels.h"
#include <dynamic_array.h>
#include <stdint.h>
#include <stdio.h>

#define ROUND_CAP 10
#define MAX_BOARDING_ATTEMPT 3

// TODO
// improve reporting and add details
// implement missed features
void simulate_combat(darr_s *t1_raw, darr_s *fl1, darr_s *t2_raw, darr_s *fl2)
{
    uint8_t round = 0;

    while (fl1->size != 0 && fl2->size != 0 && round < ROUND_CAP)
    {
        round++;

        process_shooting(fl1, fl2);
        process_casualties(fl1, fl2);

        if (fl1->size == 0 || fl2->size == 0)
            break;

        if (round > 1)
        {
            process_boarding(fl1, fl2);
            process_casualties(fl1, fl2);
        }

        // Phase 4: Escaping (TODO)
        // printf("Phase: Ships attempting to escape\n");
        // process_escaping(fl1, fl2);

        round_report(round, fl1, fl2);
    }

    battle_report(round, fl1, fl2);

    // Battle phases:
    // 6. Results
}

void process_shooting(darr_s *fl1, darr_s *fl2)
{
    PRINT("Phase: Long-range combat\n");
    if (fl1 == NULL || fl2 == NULL)
        return;

    for (uint16_t i = 0; i < fl1->size; i++)
        shooting(fl1->data[i], fl2);

    for (uint16_t i = 0; i < fl2->size; i++)
        shooting(fl2->data[i], fl1);
}

void process_casualties(darr_s *fl1, darr_s *fl2)
{
    PRINT("Phase: Resolve casualties\n");
    if (fl1 == NULL || fl2 == NULL)
        return;

    uint16_t i = 0;

    while (i < fl1->size)
    {
        if (((vessel_state_s *)fl1->data[i])->destroyed)
            destroy_vessel(fl1, i);
        else
            i++;
    }

    i = 0;
    while (i < fl2->size)
    {
        if (((vessel_state_s *)fl2->data[i])->destroyed)
            destroy_vessel(fl2, i);
        else
            i++;
    }
}

void process_boarding(darr_s *fl1, darr_s *fl2)
{
    PRINT("Phase: Boarding actions\n");
    uint16_t max_size = fl1->size > fl2->size ? fl1->size : fl2->size;
    // TODO better approach for boarding order - sort by speed first
    // For now, alternating between fleets for fairness
    for (uint16_t i = 0; i < max_size; i++)
    {
        if (i < fl1->size && !((vessel_state_s *)fl1->data[i])->destroyed)
            boarding(fl1->data[i], fl2);
        if (i < fl2->size && !((vessel_state_s *)fl2->data[i])->destroyed)
            boarding(fl2->data[i], fl1);
    }
}

void round_report(uint8_t round, darr_s *fl1, darr_s *fl2)
{
#ifdef DEBUG
    PRINT("\nEnd of round %d status:\n", round);
    PRINT("Fleet 1: %zu vessels remaining\n", fl1->size);
    PRINT("Fleet 2: %zu vessels remaining\n", fl2->size);
#endif
}

void battle_report(uint8_t round, darr_s *fl1, darr_s *fl2)
{
#ifdef DEBUG
    PRINT("\n--- Battle ended in %i ---\n", round);

    if (fl1->size > 0 && fl2->size == 0)
        PRINT("Fleet 1 is victorious!\n");
    else if (fl2->size > 0 && fl1->size == 0)
        PRINT("Fleet 2 is victorious!\n");
    else if (fl1->size > 0 && fl2->size > 0)
        PRINT("Battle ended in stalemate. Both fleets still have vessels.\n");
    else
        PRINT("Mutual destruction! Both fleets have been eliminated.\n");
#endif
}

void shooting(vessel_state_s *vessel_state, darr_s *enemy_fleet)
{
    if (vessel_state == NULL || enemy_fleet == NULL || enemy_fleet->size == 0)
        return;

    // Status effects checks
    // if (vessel_state->shocked || vessel_state->running)
    //     return;

    // TODO implement advanced targeting strategy
    vessel_state_s *target_state = enemy_fleet->data[rand() % enemy_fleet->size];

    if (target_state->data->hull == 0)
        return;

    bool hit = false;
    uint16_t total_dmg = 0;

    // TODO crit, shoke, burn
    for (uint16_t i = 0; i <= vessel_state->data->volleys - 1; i++)
    {
        if (target_state->data->hull == 0)
            return;

        hit = ((rand() % 100) + vessel_state->data->accuracy + vessel_state->data->speed -
               target_state->data->manoeuvre - target_state->data->speed) > 100;

        if (hit)
            total_dmg += vessel_state->data->projectile_dmg > target_state->data->hull_armour
                             ? vessel_state->data->projectile_dmg - target_state->data->hull_armour
                             : 1;
    }

    if (target_state->data->hull <= total_dmg)
    {
        target_state->destroyed = true;
        target_state->data->hull = 0;
    }
    else
    {
        target_state->data->hull -= total_dmg;
    }
}

void destroy_vessel(darr_s *fleet, uint16_t index)
{
    if (fleet == NULL || fleet->size <= index || !fleet->data[index])
        return;

    free(((vessel_state_s *)fleet->data[index])->data);
    ((vessel_state_s *)fleet->data[index])->data = NULL;
    darr_remove_at(fleet, index);
}

void boarding(vessel_state_s *vessel_state, darr_s *enemy_fleet)
{
    if (vessel_state == NULL || enemy_fleet == NULL)
        return;

    if (vessel_state->boarded_count == 0 && vessel_state->data->boarding && enemy_fleet->size > 0)
    {
        vessel_state_s *target_state = find_boarding_target(enemy_fleet);
        if (target_state == NULL)
            return;

        // TODO consider crew damage debuffs
        bool hit = ((vessel_state->data->speed + vessel_state->data->manoeuvre) *
                        (1.0 + (vessel_state->data->boarding_momentum / 100.0)) >
                    target_state->data->speed - target_state->data->manoeuvre);
        if (hit)
        {
            vessel_state->boarded_count++;
            target_state->boarded_count++;

            resolve_collision_damage(vessel_state, target_state);
            if (vessel_state->destroyed || target_state->destroyed)
                return;

            resolve_crew_fight(vessel_state, target_state);
        }
    }
}

vessel_state_s *find_boarding_target(darr_s *fl)
{
    uint8_t attempts = 0;
    vessel_state_s *target_state = NULL;

    while (attempts < MAX_BOARDING_ATTEMPT)
    {
        target_state = fl->data[rand() % fl->size];

        if (!target_state->destroyed && target_state->boarded_count < MAX_BOARDED_COUNT)
            return target_state;

        attempts++;
    }

    return NULL;
}

void resolve_collision_damage(vessel_state_s *v1, vessel_state_s *v2)
{
    uint16_t v1_dmg = apply_armor_reduction(v1->data->collision_dmg, v2->data->hull_armour);
    uint16_t v2_dmg = apply_armor_reduction(v2->data->collision_dmg, v1->data->hull_armour);

    v1->data->hull = apply_collision_damage(v1->data->hull, v2_dmg);
    v2->data->hull = apply_collision_damage(v2->data->hull, v1_dmg);

    if (v1->data->hull == 0)
        v1->destroyed = true;
    if (v2->data->hull == 0)
        v2->destroyed = true;
}

uint16_t apply_armor_reduction(uint16_t dmg, uint16_t armor)
{
    return dmg >= armor ? dmg - armor : 0;
}

uint16_t apply_collision_damage(uint16_t current_hull, uint16_t dmg)
{
    return dmg >= current_hull ? 0 : current_hull - dmg;
}

void resolve_crew_fight(vessel_state_s *v1, vessel_state_s *v2)
{
    uint16_t v1_crew_dmg = calculate_crew_damage(v1, v2);
    uint16_t v2_crew_dmg = calculate_crew_damage(v2, v1);

    v1->data->crew = v2_crew_dmg >= v1->data->crew ? 0 : v1->data->crew - v2_crew_dmg;
    v2->data->crew = v1_crew_dmg >= v2->data->crew ? 0 : v2->data->crew - v1_crew_dmg;

    check_crew_surrender(v1);
    check_crew_surrender(v2);
}

uint16_t calculate_crew_damage(vessel_state_s *v1, vessel_state_s *v2)
{
    uint16_t c1_dmg = v1->data->crew * v1->data->crew_dmg;
    uint16_t c2_def = v2->data->crew * v2->data->crew_def;
    return c1_dmg >= c2_def ? c2_def : c1_dmg - c2_def;
}

void check_crew_surrender(vessel_state_s *v)
{
    if (v->data->crew < v->data->crew_max / 100 * 20)
        v->destroyed = true;
}
