#include "combat.h"
#include "../static/messages.h"
#include "vessels.h"
#include <dynamic_array.h>
#include <stdint.h>
#include <stdio.h>

#define ROUND_CAP 10
#define MAX_BOARDING_ATTEMPT 3

void simulate_combat(darr_s *t1_raw, darr_s *fl1, darr_s *t2_raw, darr_s *fl2)
{
    uint8_t round = 0;

    while (fl1->size != 0 && fl2->size != 0 && round < ROUND_CAP)
    {
        round++;
        PRINT(MSG_ROUND_INFO, round);

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

        process_round_end(round, fl1, fl2);
    }

    process_battle_end(round, fl1, fl2);

    // Battle phases:
    // 6. Results
}

void process_shooting(darr_s *fl1, darr_s *fl2)
{
    PRINT(MSG_PHASE_SHOOTING);
    if (fl1 == NULL || fl2 == NULL)
        return;

    PRINT(MSG_CURRENT_TURN, MSG_ATTACKER);
    for (uint16_t i = 0; i < fl1->size; i++)
        shooting(fl1->data[i], fl2);

    PRINT(MSG_CURRENT_TURN, MSG_DEFENDER);
    for (uint16_t i = 0; i < fl2->size; i++)
        shooting(fl2->data[i], fl1);
}

void process_casualties(darr_s *fl1, darr_s *fl2)
{
    PRINT(MSG_PHASE_CASUALTIES);
    if (fl1 == NULL || fl2 == NULL)
        return;

    PRINT(MSG_CURRENT_TURN, MSG_ATTACKER);
    resolve_fleet_casualties(fl1);

    PRINT(MSG_CURRENT_TURN, MSG_DEFENDER);
    resolve_fleet_casualties(fl2);
}

void process_boarding(darr_s *fl1, darr_s *fl2)
{
    PRINT(MSG_PHASE_BOARDING);
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

void process_round_end(uint8_t round, darr_s *fl1, darr_s *fl2)
{
#ifdef DEBUG
    PRINT(MSG_END_ROUND_INFO, round);
    PRINT(MSG_LEFT_VESSELS, MSG_ATTACKER, fl1->size);
    PRINT(MSG_LEFT_VESSELS, MSG_ATTACKER, fl2->size);
#endif
}

void process_battle_end(uint8_t round, darr_s *fl1, darr_s *fl2)
{
#ifdef DEBUG
    PRINT(MSG_END_BATTLE_INFO, round);

    if (fl1->size > 0 && fl2->size == 0)
        PRINT(MSG_ATTACKER_WON);
    else if (fl2->size > 0 && fl1->size == 0)
        PRINT(MSG_DEFENDER_WON);
    else if (fl1->size > 0 && fl2->size > 0)
        PRINT(MSG_STALEMATE_RESULT);
    else
        PRINT(MSG_MUTUAL_DESTRUCTION_RESULT);
#endif
}

void shooting(vessel_state_s *vessel_state, darr_s *enemy_fleet)
{
#ifdef DEBUG
    getchar();
#endif

    if (vessel_state == NULL || enemy_fleet == NULL || enemy_fleet->size == 0)
        return;

    // Status effects checks
    // if (vessel_state->shocked || vessel_state->running)
    //     return;

    // TODO implement advanced targeting strategy
    vessel_state_s *target_state = enemy_fleet->data[rand() % enemy_fleet->size];

    PRINT(MSG_TARGETING_RESULT, get_prototype_name(vessel_state->data->type),
          get_prototype_name(target_state->data->type));

    if (target_state->data->hull == 0)
        return;

    bool hit = false;
    uint16_t total_hits = 0;
    uint16_t total_dmg = 0;

    // TODO crit, shoke, burn, damage crew
    for (uint16_t i = 0; i <= vessel_state->data->volleys - 1; i++)
    {
        if (target_state->data->hull == 0)
            return;

        hit = ((rand() % 100) + vessel_state->data->accuracy + vessel_state->data->speed -
               target_state->data->manoeuvre - target_state->data->speed) > 100;

        if (hit)
        {
            total_hits++;
            total_dmg += vessel_state->data->projectile_dmg > target_state->data->hull_armour
                             ? vessel_state->data->projectile_dmg - target_state->data->hull_armour
                             : 1;
        }
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

    PRINT(MSG_SHOOTING_REPORT, vessel_state->data->volleys, total_hits, total_dmg);
    PRINT(MSG_VESSEL_STATUS, MSG_DEFENDER, target_state->data->hull, target_state->data->crew);
}

void resolve_fleet_casualties(darr_s *fl)
{
    uint16_t i = 0;
    vessel_state_s *vs;

    while (i < fl->size)
    {
        vs = fl->data[i];
        if (vs->destroyed)
        {
            PRINT(MSG_VESSEL_SUNKED, get_prototype_name(vs->data->type));
            destroy_vessel(fl, i);
        }
        else
            i++;
    }
}

void destroy_vessel(darr_s *fl, uint16_t index)
{
    if (fl == NULL || fl->size <= index || !fl->data[index])
        return;

    free(((vessel_state_s *)fl->data[index])->data);
    ((vessel_state_s *)fl->data[index])->data = NULL;
    darr_remove_at(fl, index);
}

void boarding(vessel_state_s *vessel_state, darr_s *enemy_fleet)
{
#ifdef DEBUG
    getchar();
#endif

    if (vessel_state == NULL || enemy_fleet == NULL)
        return;

    PRINT(MSG_BOARDING_TRY);

    if (vessel_state->boarded_count == 0 && vessel_state->data->boarding && enemy_fleet->size > 0)
    {
        vessel_state_s *target_state = find_boarding_target(enemy_fleet);
        if (target_state == NULL)
            return;

        PRINT(MSG_TARGETING_RESULT, get_prototype_name(vessel_state->data->type),
              get_prototype_name(target_state->data->type));

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

void resolve_collision_damage(vessel_state_s *vs1, vessel_state_s *vs2)
{
    vessel_s *v1 = vs1->data;
    vessel_s *v2 = vs2->data;
    // TODO collision damage should not be greater than opponent hull
    // dmg crew on collision
    // add speed difference to a damage
    uint16_t v1_hull_dmg =
        calc_collision_damage(v1->hull, calc_armor_reduction(v2->collision_dmg, v1->collision_def));
    uint16_t v1_hull_res = v1->hull - v1_hull_dmg;

    uint16_t v2_hull_dmg =
        calc_collision_damage(v2->hull, calc_armor_reduction(v1->collision_dmg, v2->collision_def));
    uint16_t v2_hull_res = v2->hull - v2_hull_dmg;

    PRINT(MSG_VESSELS_COLLISION);
    PRINT(MSG_COLLISION_REPORT, MSG_ATTACKER, v1_hull_dmg, v1->hull, v1_hull_res);
    PRINT(MSG_COLLISION_REPORT, MSG_DEFENDER, v2_hull_dmg, v2->hull, v2_hull_res);

    v1->hull = v1_hull_res;
    v2->hull = v2_hull_res;

    if (v1->hull == 0)
    {
        vs1->destroyed = true;
        PRINT(MSG_VESSEL_SUNKED, MSG_ATTACKER);
    }
    if (v2->hull == 0)
    {
        vs2->destroyed = true;
        PRINT(MSG_VESSEL_SUNKED, MSG_DEFENDER);
    }
}

uint16_t calc_armor_reduction(uint16_t dmg, uint16_t armor)
{
    return dmg > armor ? dmg - armor : 0;
}

uint16_t calc_collision_damage(uint16_t hull, uint16_t dmg)
{
    return hull > dmg ? dmg : hull;
}

void resolve_crew_fight(vessel_state_s *vs1, vessel_state_s *vs2)
{
    vessel_s *v1 = vs1->data;
    vessel_s *v2 = vs2->data;

    uint16_t v1_dmg = calculate_crew_damage(v1, v2);
    uint16_t v2_dmg = calculate_crew_damage(v2, v1);
    uint16_t v1_crew_dmg = v2_dmg > v1->crew ? v1->crew : v2_dmg;
    uint16_t v2_crew_dmg = v1_dmg > v2->crew ? v2->crew : v1_dmg;

    PRINT(MSG_VESSELS_BOARDING_FIGHT);
    PRINT(MSG_BOARDING_FIGHT_REPORT, MSG_ATTACKER, v1_crew_dmg, v1->crew, v1->crew - v1_crew_dmg);
    PRINT(MSG_BOARDING_FIGHT_REPORT, MSG_DEFENDER, v2_crew_dmg, v2->crew, v2->crew - v2_crew_dmg);

    v1->crew = v1->crew - v1_crew_dmg;
    v2->crew = v2->crew - v2_crew_dmg;

    if (check_crew_surrender(vs1))
    {
        PRINT(MSG_VESSEL_SUNKED, MSG_ATTACKER);
    }
    if (check_crew_surrender(vs2))
    {
        PRINT(MSG_VESSEL_SUNKED, MSG_DEFENDER);
    }
}

uint16_t calculate_crew_damage(vessel_s *v1, vessel_s *v2)
{
    if (v1 == NULL || v2 == NULL)
    {
        return 0;
    }

    uint16_t total_dmg = 0;

    for (uint16_t i = 0; i < v1->crew; i++)
    {
        total_dmg += v1->crew_dmg > v2->crew_def ? (rand() % (v1->crew_dmg - v2->crew_def + 1))
                                                 : (rand() % 2);
    }

    return total_dmg;
}

bool check_crew_surrender(vessel_state_s *v)
{
    if (v->data->crew < v->data->crew_max / 100 * 20)
    {
        v->destroyed = true;
        return true;
    }

    return false;
}
