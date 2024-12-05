#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_string.h>

#include "json_character.h"

#include "shem_subfild.h"

const struct s_json_shem
{
    fild_t fild;
    uint8_t (*check_subfields)(const char *json, tok_t *tok, const fild_t *subfild, size_t count, uint8_t *t_count);
    const fild_t *subfild;
}json_shem [] =
{
    {{"name", JSMN_STRING, MAX_NAME_LENGTH}, NULL, NULL},
    {{"race", JSMN_STRING, MAX_NAME_LENGTH}, NULL, NULL},
    {{"ch_class", JSMN_STRING, MAX_NAME_LENGTH}, NULL, NULL},
    {{"level", JSMN_PRIMITIVE, 20}, NULL, NULL},
    {{"hit_points", JSMN_UNDEFINED, 2}, check_sb_hp_abil, sb_hit_points},
    {{"abilities", JSMN_UNDEFINED, 6}, check_sb_hp_abil, sb_abilities},
    {{"skills", JSMN_UNDEFINED, 18}, chack_sb_skill, sb_skills},
    {{NULL, JSMN_UNDEFINED, 0}, NULL, NULL}
};

static uint8_t jsoneq(const char *json, tok_t *tok, const struct s_json_shem *shem, uint8_t *t_count)
{
    if (tok->tokens[*t_count].type == JSMN_STRING &&
        ngx_strncmp(json + tok->tokens[*t_count].start, shem->fild.name, strlen(shem->fild.name)) == 0)
    {
        (*t_count)++;
        if (tok->num_tock <= (*t_count))
        {
            return 1;
        }

        if (shem->fild.type == JSMN_UNDEFINED)
        {
            if (shem->check_subfields(json, tok, shem->subfild, shem->fild.size, t_count) == 1)
            {
                return 1;
            }
        }
        else if (shem->fild.type == JSMN_STRING && tok->tokens[*t_count].type == JSMN_STRING)
        {
            if ( (size_t)(tok->tokens[*t_count].end - tok->tokens[*t_count].start) > shem->fild.size )
            {
                return 1;
            }
        }
        else if (shem->fild.type == JSMN_PRIMITIVE && tok->tokens[*t_count].type == JSMN_PRIMITIVE)
        {
            if ( (uint16_t)atoi(json + tok->tokens[*t_count].start) > shem->fild.size)
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }

        return 0;
    }
    return 1;
}

static uint8_t validate_character(const char *json, tok_t *tok)
{
    uint8_t t_count = 0;
    uint8_t shem_count = 0;

    while (json_shem[shem_count].fild.name != NULL || tok->num_tock > t_count)
    {
        t_count++;
        if (jsoneq(json, tok, &json_shem[shem_count], &t_count) == 1)
        {
            return 1;
        }
        shem_count++;
    }

    if (tok->num_tock != t_count)
    {
        return 1;
    }

    return 0;
}


uint8_t check_shem (const char *json, size_t len)
{
    jsmn_parser parser;
    tok_t tok = {0};

    jsmn_init(&parser);

    tok.num_tock = jsmn_parse(&parser, json, len, tok.tokens, MAX_TOKENS);

    if (tok.num_tock < 0)
    {
        return 1;
    }

    if (tok.num_tock < 1 || tok.tokens[0].type != JSMN_OBJECT)
    {
        return 1;
    }

    if (validate_character(json, &tok) == 1)
    {
        return 1;
    }

    return 0;
}