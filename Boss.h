#ifndef BOSS_H
#define BOSS_H

#include "Unit.h"

namespace MyGame {

    class Boss : public Unit {
    private:
        int heal_amount;
        int block_amount;

    public:
        Boss(std::string name, int hp, int max_hp, int min_atk, int max_atk, float def_perc,
            std::string atk_type, std::string def_type,
            std::string skill, std::string skill_desc, int unit_cost,
            int heal, int block, std::string atk_pattern, int multi_atk_dmg);

        int GetHealAmount() const;
        int GetBlockAmount() const;

        int ChooseAction(); // ќбъ€вим здесь, чтобы определить позже
    };

} // namespace MyGame

#endif
