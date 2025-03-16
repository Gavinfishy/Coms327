/****************************************
 * Adhere to statement in COPYWRITE.txt *
****************************************/

#ifndef DB_H
#define DB_H

#include <string>
#include <vector>

typedef struct pokemon_s {
    int id;
    std::string identifier; 
    int species_id;
    int height;
    int weight;
    int base_xp;
    int order;
    int is_default;
} pokemon_t;

typedef struct moves_s {
    int id;
    std::string identifier;
    int generation_id;
    int type_id;
    int power;
    int pp;
    int accuracy;
    int priority;
    int target_id;
    int damage_class_id;
    int effect_id;
    int effect_chance;
    int contest_type_id;
    int contest_effect_id;
    int super_contest_effect_id;
} moves_t;

typedef struct pokemon_moves_s {
    int pokemon_id;
    int version_group_id;
    int move_id;
    int pokemon_move_method_id;
    int level;
    int order;
} pokemon_moves_t;

typedef struct pokemon_species_s {
    int id;
    std::string identifier;
    int generation_id;
    int evolves_from_species_id;
    int evolution_chain_id;
    int color_id;
    int shape_id;
    int habitat_id;
    int gender_rate;
    int capture_rate;
    int base_happiness;
    int is_baby;
    int hatch_counter;
    int has_gender_differences;
    int growth_rate_id;
    int forms_switchable;
    int is_legendary;
    int is_mythical;
    int order;
    int conquest_order;
} pokemon_species_t;

typedef struct experience_s {
    int growth_rate_id;
    int level;
    int experience;
} experience_t;

typedef struct type_names_s {
    int type_id;
    int local_language_id;
    std::string name;
} type_names_t;

typedef struct pokemon_stats_s {
    int pokemon_id;
    int stat_id;
    int base_stat;
    int effort;
} pokemon_stats_t;

typedef struct stats_s {
    int id;
    int damage_class_id;
    std::string identifier;
    int is_battle_only;
    int game_index;
} stats_t;

typedef struct pokemon_types_s {
    int pokemon_id;
    int type_id;
    int slot;
} pokemon_types_t;

extern std::vector<pokemon_t> pokemons;
extern std::vector<moves_t> moves;
extern std::vector<pokemon_moves_t> pokemon_moves;
extern std::vector<pokemon_species_t> pokemon_species;
extern std::vector<experience_t> experience;
extern std::vector<type_names_t> type_names;
extern std::vector<pokemon_stats_t> pokemon_stats;
extern std::vector<stats_t> stats;
extern std::vector<pokemon_types_t> pokemon_types;

void parse_db_file (std::ifstream &file, std::string filename);

void print_db (std::string &filename);

/*
std::cout << "Users (range-based for):" << std::endl;
for (const UserData& user : users) {
  std::cout << "  Name: " << user.name << ", Age: " << user.age << std::endl;
}

*/

#endif