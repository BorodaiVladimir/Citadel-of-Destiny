// Boss.cpp
#include "Boss.h"
#include <cstdlib>
#include "Utils.h" // Добавляем этот заголовочный файл

namespace MyGame {
    Boss::Boss(std::string name, int hp, int max_hp, int min_atk, int max_atk, float def_perc,
        std::string atk_type, std::string def_type,
        std::string skill, std::string skill_desc, int unit_cost,
        int heal, int block, std::string atk_pattern, int multi_atk_dmg)
        : Unit(name, hp, max_hp, min_atk, max_atk, def_perc, atk_type, def_type, skill, skill_desc, unit_cost, atk_pattern, multi_atk_dmg),
        heal_amount(heal), block_amount(block) {}

    int Boss::GetHealAmount() const { return heal_amount; }
    int Boss::GetBlockAmount() const { return block_amount; }

    int Boss::ChooseAction() {
        int choice = MyGame::GetRandomNumber(1, 4); // 1 - Атака, 2 - Атака по всем, 3 - Лечение, 4 - Блок
        return choice;
    }
}