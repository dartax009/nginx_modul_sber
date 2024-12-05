#ifndef SHEM_SUBFILD_H
#define SHEM_SUBFILD_H

#include <stdlib.h>

#include "json_character.h"


static const fild_t sb_hit_points[] = 
{
    {"max", JSMN_PRIMITIVE, 500},
    {"current", JSMN_PRIMITIVE, 500},
    {NULL, JSMN_UNDEFINED, 0}
};

static const fild_t sb_abilities[] =
{
    {"strength", JSMN_PRIMITIVE, 30},
    {"dexterity", JSMN_PRIMITIVE, 30},
    {"constitution", JSMN_PRIMITIVE, 30},
    {"intelligence", JSMN_PRIMITIVE, 30},
    {"wisdom", JSMN_PRIMITIVE, 30},
    {"charisma", JSMN_PRIMITIVE, 30},
    {NULL, JSMN_UNDEFINED, 0}
};

static const fild_t sb_skills[] =
{
    {"Athletics", JSMN_PRIMITIVE, 0},
    {"Acrobatics", JSMN_PRIMITIVE, 0},
    {"Agility_heands", JSMN_PRIMITIVE, 0},
    {"Stealth", JSMN_PRIMITIVE, 0},
    {"Analysis", JSMN_PRIMITIVE, 0},
    {"History", JSMN_PRIMITIVE, 0},
    {"Magic", JSMN_PRIMITIVE, 0},
    {"Nature", JSMN_PRIMITIVE, 0},
    {"Religion", JSMN_PRIMITIVE, 0},
    {"Perception", JSMN_PRIMITIVE, 0},
    {"Survival", JSMN_PRIMITIVE, 0},
    {"Medicine", JSMN_PRIMITIVE, 0},
    {"Insight", JSMN_PRIMITIVE, 0},
    {"Animal_care", JSMN_PRIMITIVE, 0},
    {"Performance", JSMN_PRIMITIVE, 0},
    {"Intimidation", JSMN_PRIMITIVE, 0},
    {"Deception", JSMN_PRIMITIVE, 0},
    {"Conviction", JSMN_PRIMITIVE, 0},
    {NULL, JSMN_UNDEFINED, 0}
};

static uint8_t check_sb_hp_abil (const char *json, tok_t *tok, const fild_t *subfild, size_t count, uint8_t *t_count)
{
    for (uint8_t i = 0; i < count; i++)
    {
        (*t_count)++;
        if (tok->num_tock <= (*t_count))
        {
            return 1;
        }

        if (subfild[i].name == NULL)
        {
            return 1;
        }

        if (tok->tokens[*t_count].type == JSMN_STRING &&
            ngx_strncmp(json + tok->tokens[*t_count].start, subfild[i].name, strlen(subfild[i].name)) == 0)
        {
            (*t_count)++;
            if (subfild[i].type != JSMN_PRIMITIVE)
            {
                return 1;
            }
            if ( (uint16_t)atoi(json + tok->tokens[*t_count].start) > subfild[i].size)
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

static uint8_t chack_sb_skill (const char *json, tok_t *tok, const fild_t *subfild, size_t count, uint8_t *t_count)
{
    uint8_t tmp_count = 0;
    for (uint8_t i = 0; i < count; i++)
    {
        tmp_count = 0;
        (*t_count)++;
        if (tok->num_tock <= (*t_count))
        {
            break;
        }

        if (tok->tokens[*t_count].type == JSMN_STRING)
        {
            while (subfild[tmp_count].name != NULL)
            {
                if (ngx_strncmp(json + tok->tokens[*t_count].start, subfild[tmp_count].name, strlen(subfild[tmp_count].name)) == 0)
                {
                    
                    break;
                }
                tmp_count++;
            }
            if (subfild[tmp_count].name == NULL)
            {
                (*t_count)--;
                return 0;
            }
        }
        else
        {
            return 1;
        }
    }
    return 0;
}


#endif