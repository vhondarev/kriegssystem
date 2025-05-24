#include "combat.h"
#include "../static/messages.h"
#include "prototypes.h"
#include "vessels.h"
#include <dynamic_array.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ROUND_CAP 10
#define MAX_BOARDING_ATTEMPT 3
#define CREW_FATALITY_RATE 0.4
#define CREW_SURRENDER_CAP 0.2

void simulate_combat(darr_s *fl1, darr_s *fl2, darr_s *t1_end_raw, darr_s *t2_end_raw)
{
    if (fl1 == NULL || fl1->data == NULL || fl2 == NULL || fl2->data == NULL)
        return;

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

    process_battle_end(round, fl1, fl2, t1_end_raw, t2_end_raw);

    // Battle phases:
    // 6. Results
}

void process_shooting(darr_s *fl1, darr_s *fl2)
{
    if (fl1 == NULL || fl1->data == NULL || fl2 == NULL || fl2->data == NULL)
        return;

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
    if (fl1 == NULL || fl1->data == NULL || fl2 == NULL || fl2->data == NULL)
        return;

    PRINT(MSG_PHASE_CASUALTIES);
    if (fl1 == NULL || fl2 == NULL)
        return;

    PRINT(MSG_CURRENT_TURN, MSG_ATTACKER);
    resolve_fleet_status(fl1);

    PRINT(MSG_CURRENT_TURN, MSG_DEFENDER);
    resolve_fleet_status(fl2);
}

void process_boarding(darr_s *fl1, darr_s *fl2)
{
    if (fl1 == NULL || fl1->data == NULL || fl2 == NULL || fl2->data == NULL)
        return;

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
    PRINT(MSG_LEFT_VESSELS, MSG_DEFENDER, fl2->size);
#endif
}

void process_battle_end(uint8_t round, darr_s *fl1, darr_s *fl2, darr_s *t1_raw, darr_s *t2_raw)
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
    pack_fleet_to_raw(fl1, t1_raw);
    pack_fleet_to_raw(fl2, t2_raw);

    char result[1024] = {'\0'};

    raw_to_str_result(t1_raw, result);
    raw_to_str_result(t2_raw, result);

    printf("%s\n", result);
}

void shooting(vessel_state_s *vs, darr_s *enemy_fleet)
{
#ifdef DEBUG
    getchar();
#endif

    if (vs == NULL || enemy_fleet == NULL || enemy_fleet->size == 0)
        return;

    vs->boarded_count = 0;

    // Status effects checks
    // if (vessel_state->shocked || vessel_state->running)
    //     return;

    // TODO implement advanced targeting strategy
    vessel_state_s *target_state = enemy_fleet->data[rand() % enemy_fleet->size];

    PRINT(MSG_TARGETING_RESULT, get_prototype_name(vs->data->type),
          get_prototype_name(target_state->data->type));

    if (target_state->data->hull == 0)
        return;

    bool hit = false;
    uint16_t available_volleys = calc_available_volleys(vs->data);
    uint16_t total_hits = 0;
    uint16_t total_dmg = 0;

    // TODO crit, shoke, burn
    for (uint16_t i = 0; i <= available_volleys - 1; i++)
    {
        if (target_state->data->hull == 0)
            return;

        hit = ((rand() % 100) + vs->data->accuracy + calc_vessel_speed(vs->data) -
               target_state->data->manoeuvre - calc_vessel_speed(target_state->data)) > 100;

        if (hit)
        {
            total_hits++;
            total_dmg +=
                vs->data->projectile_dmg > target_state->data->hull_armour
                    ? (rand() % (vs->data->projectile_dmg - target_state->data->hull_armour + 1))
                    : 1;
        }
    }

    total_dmg = max_dmg(target_state->data->hull, total_dmg);
    uint16_t crew_losses = calc_crew_losses(target_state, total_dmg);

    PRINT(MSG_SHOOTING_REPORT, vs->data->volleys, total_hits, total_dmg);
    PRINT(MSG_HULL_DMG_REPORT, MSG_DEFENDER, total_dmg, target_state->data->hull,
          target_state->data->hull - total_dmg);
    PRINT(MSG_CREW_SUFFER_REPORT, MSG_DEFENDER, crew_losses, target_state->data->crew,
          target_state->data->crew - crew_losses);

    target_state->delayed_hull_dmg = total_dmg;
    target_state->delayed_crew_dmg = crew_losses;
}

uint16_t calc_available_volleys(vessel_s *v)
{
    uint16_t crew_capacity = ((float)v->crew / v->crew_max) * v->volleys;
    uint16_t hull_capacity = ((float)v->hull / v->hull_max) * v->volleys;
    return crew_capacity > hull_capacity ? hull_capacity : crew_capacity;
}

void resolve_fleet_status(darr_s *fl)
{
    if (fl == NULL || fl->data == NULL)
        return;

    uint16_t i = 0;
    vessel_state_s *vs;

    while (i < fl->size)
    {
        vs = fl->data[i];
        if (vs->delayed_hull_dmg > 0)
        {
            vs->data->hull = vs->data->hull - vs->delayed_hull_dmg;
            vs->delayed_hull_dmg = 0;
        }

        if (vs->destroyed)
        {
            PRINT(MSG_VESSEL_SUNKED, get_prototype_name(vs->data->type));
            destroy_vessel(fl, i);
        }
        else
        {
            i++;
            if (vs->delayed_crew_dmg > 0)
            {
                vs->data->crew = vs->data->crew - vs->delayed_crew_dmg;
                vs->delayed_crew_dmg = 0;
            }
        }
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

void boarding(vessel_state_s *vs, darr_s *enemy_fleet)
{
#ifdef DEBUG
    getchar();
#endif

    if (vs == NULL || vs->data == NULL || enemy_fleet == NULL || enemy_fleet->data == NULL)
        return;

    PRINT(MSG_BOARDING_TRY);

    if (vs->boarded_count == 0 && vs->data->boarding && enemy_fleet->size > 0)
    {
        vessel_state_s *target_state = find_boarding_target(enemy_fleet);
        if (target_state == NULL)
            return;

        PRINT(MSG_TARGETING_RESULT, get_prototype_name(vs->data->type),
              get_prototype_name(target_state->data->type));

        bool hit = ((calc_vessel_speed(vs->data) + vs->data->manoeuvre) *
                        (1.0 + (vs->data->boarding_momentum / 100.0)) >
                    calc_vessel_speed(target_state->data) - target_state->data->manoeuvre);

        if (hit)
        {
            vs->boarded_count++;
            target_state->boarded_count++;

            resolve_collision_damage(vs, target_state);

            if (vs->destroyed || target_state->destroyed)
                return;

            resolve_crew_fight(vs, target_state);
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
    if (vs1 == NULL || vs1->data == NULL || vs2 == NULL || vs2->data == NULL)
        return;

    vessel_s *v1 = vs1->data;
    vessel_s *v2 = vs2->data;

    uint16_t v1_take_dmg = max_dmg(v1->hull, calc_collision_dmg(v2, v1));
    uint16_t v2_take_dmg = max_dmg(v2->hull, calc_collision_dmg(v1, v2));
    uint16_t v1_crew_losses = calc_crew_losses(vs1, v1_take_dmg);
    uint16_t v2_crew_losses = calc_crew_losses(vs2, v2_take_dmg);

    PRINT(MSG_VESSELS_COLLISION);
    PRINT(MSG_HULL_DMG_REPORT, MSG_ATTACKER, v1_take_dmg, v1->hull, v1->hull - v1_take_dmg);
    PRINT(MSG_CREW_SUFFER_REPORT, MSG_ATTACKER, v1_crew_losses, v1->crew,
          v1->crew - v1_crew_losses);
    PRINT(MSG_HULL_DMG_REPORT, MSG_DEFENDER, v2_take_dmg, v2->hull, v2->hull - v2_take_dmg);
    PRINT(MSG_CREW_SUFFER_REPORT, MSG_DEFENDER, v2_crew_losses, v2->crew,
          v2->crew - v2_crew_losses);

    v1->hull = v1->hull - v1_take_dmg;
    v2->hull = v2->hull - v2_take_dmg;
    v1->crew = v1->crew - v1_crew_losses;
    v2->crew = v2->crew - v2_crew_losses;

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
uint16_t calc_collision_dmg(vessel_s *v1, vessel_s *v2)
{
    if (v1 == NULL || v2 == NULL)
        return 0;

    uint16_t v1_speed = calc_vessel_speed(v1);
    uint16_t v2_speed = calc_vessel_speed(v2);
    uint16_t dmg =
        max_dmg(v1->hull, v1->collision_dmg + (v1_speed > v2_speed ? v1_speed - v2_speed : 0));

    return dmg > v2->collision_def ? dmg - v2->collision_def : (rand() % 2);
}

uint16_t calc_vessel_speed(vessel_s *v)
{
    if (v == NULL)
        return 0;

    float reduction_factor = 1.0;

    if (v->crew_max * 0.60 < v->crew)
        reduction_factor = 0.8;
    else if (v->crew_max * 0.40 < v->crew)
        reduction_factor = 0.5;

    return (uint16_t)v->speed * reduction_factor;
}

uint16_t max_dmg(uint16_t hp, uint16_t dmg)
{
    return hp > dmg ? dmg : hp;
}

void resolve_crew_fight(vessel_state_s *vs1, vessel_state_s *vs2)
{
    if (vs1 == NULL || vs2 == NULL || vs1->data == NULL || vs2->data == NULL)
        return;

    vessel_s *v1 = vs1->data;
    vessel_s *v2 = vs2->data;

    uint16_t v1_take_dmg = max_dmg(v1->crew, calc_crew_damage(v2, v1));
    uint16_t v2_take_dmg = max_dmg(v2->crew, calc_crew_damage(v1, v2));

    PRINT(MSG_VESSELS_BOARDING_FIGHT);
    PRINT(MSG_CREW_SUFFER_REPORT, MSG_ATTACKER, v1_take_dmg, v1->crew, v1->crew - v1_take_dmg);
    PRINT(MSG_CREW_SUFFER_REPORT, MSG_DEFENDER, v2_take_dmg, v2->crew, v2->crew - v2_take_dmg);

    v1->crew = v1->crew - v1_take_dmg;
    v2->crew = v2->crew - v2_take_dmg;

    if (check_crew_surrender(vs1))
        PRINT(MSG_VESSEL_SUNKED, MSG_ATTACKER);
    if (check_crew_surrender(vs2))
        PRINT(MSG_VESSEL_SUNKED, MSG_DEFENDER);
}

uint16_t calc_crew_damage(vessel_s *v1, vessel_s *v2)
{
    if (v1 == NULL || v2 == NULL)
        return 0;

    uint16_t total_dmg = 0;

    for (uint16_t i = 0; i < v1->crew; i++)
    {
        total_dmg += v1->crew_dmg > v2->crew_def ? (rand() % (v1->crew_dmg - v2->crew_def + 1))
                                                 : (rand() % 2);
    }

    return total_dmg * CREW_FATALITY_RATE;
}

uint16_t calc_crew_losses(vessel_state_s *vs1, uint16_t integrity_dmg)
{
    if (vs1 == NULL || vs1->data == NULL || integrity_dmg == 0.0)
        return 0;

    return roundf(vs1->data->crew * ((float)integrity_dmg / vs1->data->hull) * CREW_FATALITY_RATE);
}

bool check_crew_surrender(vessel_state_s *v)
{
    if (v == NULL || v->data == NULL)
    {
        return false;
    }
    else if (v->data->crew < v->data->crew_max * CREW_SURRENDER_CAP)
    {
        v->destroyed = true;
        return true;
    }

    return false;
}
