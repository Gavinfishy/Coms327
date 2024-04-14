/****************************************
 * Adhere to statement in COPYWRITE.txt *
****************************************/

#include "db.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <vector>
#include <limits.h>

#define setIntValue(v, cell) \
    if (cell == "") v = INT_MAX; \
    else v = std::stoi(cell);

#define printIntValue(v)  \
    ((v == INT_MAX) ? "" : std::to_string(v))

void parse_db_file (std::ifstream &file, std::string filename) {
    int fieldIndex;
    std::string line;
    std::string cell;
    std::stringstream ss;

    std::getline(file, line);

    if (filename == "pokemon") {
        while (std::getline(file, line)) {
            std::stringstream ss(line);

            pokemon_t p;

            fieldIndex = 0;
            while (std::getline(ss, cell, ',') || fieldIndex <= 7) {

                switch (fieldIndex)
                {
                case 0:
                    setIntValue(p.id, cell);
                    break;
                case 1:
                    p.identifier = cell;
                    break;
                case 2:
                    setIntValue(p.species_id, cell);
                    break;
                case 3:
                    setIntValue(p.height, cell);
                    break;
                case 4:
                    setIntValue(p.weight, cell);
                    break;
                case 5:
                    setIntValue(p.base_xp, cell);
                    break;
                case 6:
                    setIntValue(p.order, cell);
                    break;
                case 7:
                    setIntValue(p.is_default, cell);
                    break;
                
                default:
                    break;
                }
                fieldIndex++;
            }
            pokemons.push_back(p);
        }
    }
    else if (filename == "moves") {
        while (std::getline(file, line)) {
            std::stringstream ss(line);

            moves_t p;

            fieldIndex = 0;
            while (std::getline(ss, cell, ',') || fieldIndex <= 14) {

                switch (fieldIndex)
                {
                case 0:
                    setIntValue(p.id, cell);
                    break;
                case 1:
                    p.identifier = cell;
                    break;
                case 2:
                    setIntValue(p.generation_id, cell);
                    break;
                case 3:
                    setIntValue(p.type_id, cell);
                    break;
                case 4:
                    setIntValue(p.power, cell);
                    break;
                case 5:
                    setIntValue(p.pp, cell);
                    break;
                case 6:
                    setIntValue(p.accuracy, cell);
                    break;
                case 7:
                    setIntValue(p.priority, cell);
                    break;
                case 8:
                    setIntValue(p.target_id, cell);
                    break;
                case 9:
                    setIntValue(p.damage_class_id, cell);
                    break;
                case 10:
                    setIntValue(p.effect_id, cell);
                    break;
                case 11:
                    setIntValue(p.effect_chance, cell);
                    break;
                case 12:
                    setIntValue(p.contest_type_id, cell);
                    break;
                case 13:
                    setIntValue(p.contest_effect_id, cell);
                    break;
                case 14:
                    setIntValue(p.super_contest_effect_id, cell);
                    break;
                
                default:
                    break;
                }
                fieldIndex++;
            }
            moves.push_back(p);
        }
    }
    else if (filename == "pokemon_moves") {
        while (std::getline(file, line)) {
            std::stringstream ss(line);

            pokemon_moves_t p;

            fieldIndex = 0;
            while (std::getline(ss, cell, ',') || fieldIndex <= 5) {

                switch (fieldIndex)
                {
                case 0:
                    setIntValue(p.pokemon_id, cell);
                    break;
                case 1:
                    setIntValue(p.version_group_id, cell);
                    break;
                case 2:
                    setIntValue(p.move_id, cell);
                    break;
                case 3:
                    setIntValue(p.pokemon_move_method_id, cell);
                    break;
                case 4:
                    setIntValue(p.level, cell);
                    break;
                case 5:
                    setIntValue(p.order, cell);
                    break;
                
                default:
                    break;
                }
                fieldIndex++;
            }
            pokemon_moves.push_back(p);
        }
    }
    else if (filename == "pokemon_species") {
        while (std::getline(file, line)) {
            std::stringstream ss(line);

            pokemon_species_t p;

            fieldIndex = 0;
            while (std::getline(ss, cell, ',') || fieldIndex <= 19) {

                switch (fieldIndex)
                {
                case 0:
                    setIntValue(p.id, cell);
                    break;
                case 1:
                    p.identifier = cell;
                    break;
                case 2:
                    setIntValue(p.generation_id, cell);
                    break;
                case 3:
                    setIntValue(p.evolves_from_species_id, cell);
                    break;
                case 4:
                    setIntValue(p.evolution_chain_id, cell);
                    break;
                case 5:
                    setIntValue(p.color_id, cell);
                    break;
                case 6:
                    setIntValue(p.shape_id, cell);
                    break;
                case 7:
                    setIntValue(p.habitat_id, cell);
                    break;
                case 8:
                    setIntValue(p.gender_rate, cell);
                    break;
                case 9:
                    setIntValue(p.capture_rate, cell);
                    break;
                case 10:
                    setIntValue(p.base_happiness, cell);
                    break;
                case 11:
                    setIntValue(p.is_baby, cell);
                    break;
                case 12:
                    setIntValue(p.hatch_counter, cell);
                    break;
                case 13:
                    setIntValue(p.has_gender_differences, cell);
                    break;
                case 14:
                    setIntValue(p.growth_rate_id, cell);
                    break;
                case 15:
                    setIntValue(p.forms_switchable, cell);
                    break;
                case 16:
                    setIntValue(p.is_legendary, cell);
                    break;
                case 17:
                    setIntValue(p.is_mythical, cell);
                    break;
                case 18:
                    setIntValue(p.order, cell);
                    break;
                case 19:
                    setIntValue(p.conquest_order, cell);
                    break;
                
                default:
                    break;
                }
                fieldIndex++;
            }
            pokemon_species.push_back(p);
        }
    }
    else if (filename == "experience") {
        while (std::getline(file, line)) {
            std::stringstream ss(line);

            experience_t p;

            fieldIndex = 0;
            while (std::getline(ss, cell, ',') || fieldIndex <= 2) {

                switch (fieldIndex)
                {
                case 0:
                    setIntValue(p.growth_rate_id, cell);
                    break;
                case 1:
                    setIntValue(p.level, cell);
                    break;
                case 2:
                    setIntValue(p.experience, cell);
                    break;
                
                default:
                    break;
                }
                fieldIndex++;
            }
            experience.push_back(p);
        }
    }
    else if (filename == "type_names") {
        while (std::getline(file, line)) {
            std::stringstream ss(line);

            type_names_t p;

            fieldIndex = 0;
            while (std::getline(ss, cell, ',') || fieldIndex <= 2) {

                switch (fieldIndex)
                {
                case 0:
                    setIntValue(p.type_id, cell);
                    break;
                case 1:
                    setIntValue(p.local_language_id, cell);
                    break;
                case 2:
                    p.name = cell;
                    break;
                
                default:
                    break;
                }
                fieldIndex++;
            }
            type_names.push_back(p);
        }
    }
    else if (filename == "pokemon_stats") {
        while (std::getline(file, line)) {
            std::stringstream ss(line);

            pokemon_stats_t p;

            fieldIndex = 0;
            while (std::getline(ss, cell, ',') || fieldIndex <= 3) {

                switch (fieldIndex)
                {
                case 0:
                    setIntValue(p.pokemon_id, cell);
                    break;
                case 1:
                    setIntValue(p.stat_id, cell);
                    break;
                case 2:
                    setIntValue(p.base_stat, cell);
                    break;
                case 3:
                    setIntValue(p.effort, cell);
                    break;
                
                default:
                    break;
                }
                fieldIndex++;
            }
            pokemon_stats.push_back(p);
        }
    }
    else if (filename == "stats") {
        while (std::getline(file, line)) {
            std::stringstream ss(line);

            stats_t p;

            fieldIndex = 0;
            while (std::getline(ss, cell, ',') || fieldIndex <= 4) {

                switch (fieldIndex)
                {
                case 0:
                    setIntValue(p.id, cell);
                    break;
                case 1:
                    setIntValue(p.damage_class_id, cell);
                    break;
                case 2:
                    p.identifier = cell;
                    break;
                case 3:
                    setIntValue(p.is_battle_only, cell);
                    break;
                case 4:
                    setIntValue(p.game_index, cell);
                    break;
                
                default:
                    break;
                }
                fieldIndex++;
            }
            stats.push_back(p);
        }
    }
    else if (filename == "pokemon_types") {
        while (std::getline(file, line)) {
            std::stringstream ss(line);

            pokemon_types_t p;

            fieldIndex = 0;
            while (std::getline(ss, cell, ',') || fieldIndex <= 2) {

                switch (fieldIndex)
                {
                case 0:
                    setIntValue(p.pokemon_id, cell);
                    break;
                case 1:
                    setIntValue(p.type_id, cell);
                    break;
                case 2:
                    setIntValue(p.slot, cell);
                    break;
                
                default:
                    break;
                }
                fieldIndex++;
            }
            pokemon_types.push_back(p);
        }
    }

    file.close();
}

void print_db (std::string &filename) {
    if (filename == "pokemon") {
        for (const pokemon_t& p : pokemons) {
            std::cout << "id: " << printIntValue(p.id) <<
                        " identifier: " << p.identifier <<  
                        " species id: " << printIntValue(p.species_id) <<
                        " height: " << printIntValue(p.height) <<
                        " weight: " << printIntValue(p.weight) <<
                        " base xp " << printIntValue(p.base_xp) <<
                        " order: " << printIntValue(p.order) <<
                        " is default: " << printIntValue(p.is_default) <<
                        std::endl;
        }
    }
    else if (filename == "moves") {
        for (const moves_t& p : moves) {
            std::cout << "id: " << printIntValue(p.id) <<
                        " identifier: " << p.identifier <<  
                        " generation id: " << printIntValue(p.generation_id) <<
                        " type id: " << printIntValue(p.type_id) <<
                        " power: " << printIntValue(p.power) <<
                        " pp: " << printIntValue(p.pp) <<
                        " accuracy: " << printIntValue(p.accuracy) <<
                        " priority: " << printIntValue(p.priority) <<
                        " target id: " << printIntValue(p.target_id) <<
                        " damgae class id: " << printIntValue(p.damage_class_id) <<
                        " effect id: " << printIntValue(p.effect_id) <<
                        " effect chance: " << printIntValue(p.effect_chance) <<
                        " contest type id: " << printIntValue(p.contest_type_id) <<
                        " contest effect id: " << printIntValue(p.contest_effect_id) <<
                        " super contest effect id: " << printIntValue(p.super_contest_effect_id) <<
                        std::endl;
        }
    }
    else if (filename == "pokemon_moves") {
        for (const pokemon_moves_t& p : pokemon_moves) {
            std::cout << "pokemon id: " << printIntValue(p.pokemon_id) <<
                        " version group id: " << printIntValue(p.version_group_id) <<  
                        " move id: " << printIntValue(p.move_id) <<
                        " pokemon move method id: " << printIntValue(p.pokemon_move_method_id) <<
                        " level: " << printIntValue(p.level) <<
                        " order: " << printIntValue(p.order) <<
                        std::endl;
        }
    }
    else if (filename == "pokemon_species") {
        for (const pokemon_species_t& p : pokemon_species) {
            std::cout << "id: " << printIntValue(p.id) <<
                        " identifier: " << p.identifier <<  
                        " generation id: " << printIntValue(p.generation_id) <<
                        " evolves from species id: " << printIntValue(p.evolves_from_species_id) <<
                        " evolution chain: " << printIntValue(p.evolution_chain_id) <<
                        " color id: " << printIntValue(p.color_id) <<
                        " shape id: " << printIntValue(p.shape_id) <<
                        " habitat id: " << printIntValue(p.habitat_id) <<
                        " gender rate: " << printIntValue(p.gender_rate) <<
                        " capture rate: " << printIntValue(p.capture_rate) <<
                        " base happiness: " << printIntValue(p.base_happiness) <<
                        " is baby: " << printIntValue(p.is_baby) <<
                        " hatch couter: " << printIntValue(p.hatch_counter) <<
                        " has gender differences: " << printIntValue(p.has_gender_differences) <<
                        " growth rate id: " << printIntValue(p.growth_rate_id) <<
                        " forms switchable " << printIntValue(p.forms_switchable) <<
                        " is legendary: " << printIntValue(p.is_legendary) <<
                        " is mythical: " << printIntValue(p.is_mythical) <<
                        " order: " << printIntValue(p.order) <<
                        " conquest order: " << printIntValue(p.conquest_order) <<
                        std::endl;
        }
    }
    else if (filename == "experience") {
        for (const experience_t& p : experience) {
            std::cout << "growth rate id: " << printIntValue(p.growth_rate_id) << 
                        " level: " << printIntValue(p.level) <<
                        " experience: " << printIntValue(p.experience) <<
                        std::endl;
        }
    }
    else if (filename == "type_names") {
        for (const type_names_t& p : type_names) {
            std::cout << "type id: " << printIntValue(p.type_id) << 
                        " local language id: " << printIntValue(p.local_language_id) <<
                        " name: " << p.name << 
                        std::endl;
        }
    }
    else if (filename == "pokemon_stats") {
        for (const pokemon_stats_t& p : pokemon_stats) {
            std::cout << "pokemon id: " << printIntValue(p.pokemon_id) <<
                        " stat id: " << printIntValue(p.stat_id) <<  
                        " base stat: " << printIntValue(p.base_stat) <<
                        " effort: " << printIntValue(p.effort) <<
                        std::endl;
        }
    }
    else if (filename == "stats") {
        for (const stats_t& p : stats) {
            std::cout << "id: " << printIntValue(p.id) <<
                        " damage class id: " << printIntValue(p.damage_class_id) <<  
                        " identifier: " << p.identifier <<  
                        " is battle only: " << printIntValue(p.is_battle_only) <<
                        " game index: " << printIntValue(p.game_index) <<
                        std::endl;
        }
    }
    else if (filename == "pokemon_types") {
        for (const pokemon_types_t& p : pokemon_types) {
            std::cout << "pokemon id: " << printIntValue(p.pokemon_id) <<  
                        " type id: " << printIntValue(p.type_id) <<  
                        " slot: " << printIntValue(p.slot) <<
                        std::endl;
        }
    }
}