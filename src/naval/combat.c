#include "combat.h"
#include "vessels.h"
#include <dynamic_array.h>
#include <stdio.h>

#define ROUND_CAP 10

void simulate_battle(darr_s *t1_raw, darr_s *t1_fleet, darr_s *t2_raw, darr_s *t2_fleet)
{
    uint16_t i;
    int round = 0;

    printf("\nPress any key to start\n");
    getchar();

    while (t1_fleet->size != 0 && t2_fleet->size != 0)
    {
        if (++round > 6)
            break;

        printf("Round: %d\n", round);

        for (i = 0; i < t1_fleet->size; i++)
            shooting(t1_fleet->data[i], t2_fleet);

        for (i = 0; i < t2_fleet->size; i++)
            shooting(t2_fleet->data[i], t1_fleet);

        i = 0;
        while (i < t1_fleet->size)
        {
            if (((vessel_state_s *)t1_fleet->data[i])->destroyed)
                casualties(t1_fleet, i);
            else
                i++;
        }

        i = 0;
        while (i < t2_fleet->size)
        {
            if (((vessel_state_s *)t2_fleet->data[i])->destroyed)
                casualties(t2_fleet, i);
            else
                i++;
        }
    }

    // Battle phases:
    // 3. Boarding
    // 4. Casualties
    // 5. Escaping
    // 6. Results
}

void shooting(vessel_state_s *vessel_state, darr_s *enemy_fleet)
{
    if (vessel_state->shocked || vessel_state->boarded || vessel_state->running)
        return;

    vessel_state_s *target_state = enemy_fleet->data[rand() % enemy_fleet->size];

    if (target_state->data->hull == 0)
        return;

    bool hit = false;
    uint16_t dmg = 0;

    for (uint8_t i = 0; i <= vessel_state->data->volleys - 1; i++)
    {
        if (target_state->data->hull == 0)
            return;

        int rand_val = (rand() % 100);
        hit = (rand_val + vessel_state->data->accuracy + vessel_state->data->speed -
               target_state->data->manoeuvre - target_state->data->speed) > 100;

        if (hit)
        {
            dmg += vessel_state->data->projectile_dmg > target_state->data->collision_dmg
                       ? vessel_state->data->projectile_dmg - target_state->data->collision_dmg
                       : 1;
        }
    }

    // TODO crit, shoke, burn
    if (target_state->data->hull <= dmg)
    {
        target_state->destroyed = true;
        target_state->data->hull = 0;
    }
    else
    {
        target_state->data->hull -= dmg;
    }
}

void casualties(darr_s *fleet, uint16_t index)
{
    free(((vessel_state_s *)fleet->data[index])->data);
    ((vessel_state_s *)fleet->data[index])->data = NULL;
    darr_remove_at(fleet, index);
}
