#include "Boss.h"
#include "Utils.h"

#include <cstdlib>  
#include <string>  

namespace MyGame {

    Boss::Boss(const std::string& name, int hp, int max_hp, int min_atk, int max_atk,
        float def_perc, const std::string& atk_type,
        const std::string& def_type, const std::string& skill,
        const std::string& skill_desc, int unit_cost, int heal, int block,
        const std::string& atk_pattern, int multi_atk_dmg)
        : Unit(name, hp, max_hp, min_atk, max_atk, def_perc, atk_type, def_type,
            skill, skill_desc, unit_cost, atk_pattern, multi_atk_dmg),
        heal_amount_(heal),
        block_amount_(block) {}

    int Boss::GetHealAmount() const { return heal_amount_; }

    int Boss::GetBlockAmount() const { return block_amount_; }

    int Boss::ChooseAction() {
        const int kMaxBossActions = 4;
        int choice = GetRandomNumber(1, kMaxBossActions);
        return choice;
    }

}  