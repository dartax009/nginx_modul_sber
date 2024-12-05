#ifndef JSON_CHARACTER_H
#define JSON_CHARACTER_H

#include <stdint.h>
#include "../jsmn.h"

#define MAX_SKILLS 10
#define MAX_WEAPONS 10
#define MAX_ITEMS 10
#define MAX_ABILITIES 10
#define MAX_NAME_LENGTH 100
#define MAX_DESCRIPTION_LENGTH 256

#define MAX_TOKENS (2 + \
                    2 + /* name */ \
                    2 + /* race */ \
                    2 + /* ch_class */ \
                    2 + /* level */ \
                    2 * 2 + /* hit_points (2 int) */ \
                    2 * 6 + /* abilities (6 int) */ \
                    (MAX_SKILLS * 2) + 18 /* skills */)

typedef struct {
    int max;
    int current;
} HitPoints;

typedef struct {
    int strength;
    int dexterity;
    int constitution;
    int intelligence;
    int wisdom;
    int charisma;
} Abilities;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char race[MAX_NAME_LENGTH];
    char ch_class[MAX_NAME_LENGTH];
    int level;
    HitPoints hit_points;
    Abilities abilities;
    char skills[MAX_SKILLS][MAX_NAME_LENGTH];
} Character;

typedef struct s_fild
{
    char *name;
    jsmntype_t type;
    size_t size;
}fild_t;

typedef struct s_tok
{
    int num_tock;
    jsmntok_t tokens[MAX_TOKENS];
}tok_t;

#endif