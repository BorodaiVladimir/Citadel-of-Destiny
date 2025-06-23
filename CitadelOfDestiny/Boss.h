// Boss.h
#ifndef MYGAME_BOSS_H_
#define MYGAME_BOSS_H_

#include "Unit.h"  // Необходимо для Unit

namespace MyGame {

    class Boss : public Unit {
    public:
        // Existing constructor
        Boss(const std::string& name, int hp, int max_hp, int min_atk, int max_atk,
            float def_perc, const std::string& atk_type, const std::string& def_type,
            const std::string& skill, const std::string& skill_desc, int unit_cost,
            int heal, int block, const std::string& atk_pattern,
            int multi_atk_dmg);

        // Default constructor (added)
        Boss() : Unit("Default Boss", 100, 100, 10, 15, 0.1f, "Physical", "Armor",
            "Strike", "Default attack", 100, "Single", 0),
            heal_amount_(10),
            block_amount_(5) {} // Initialization list

        int GetHealAmount() const;
        int GetBlockAmount() const;
        int ChooseAction();

    private:
        int heal_amount_;
        int block_amount_;
    };

}  // namespace MyGame

#endif  // MYGAME_BOSS_H_