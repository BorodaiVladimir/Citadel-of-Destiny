#ifndef MYGAME_UNIT_H_
#define MYGAME_UNIT_H_

#include <string>  

namespace MyGame {

    class Unit {
    public:
        Unit(const std::string& name, int hp, int max_hp, int min_atk, int max_atk,
            float def_perc, const std::string& atk_type, const std::string& def_type,
            const std::string& skill, const std::string& skill_desc, int unit_cost,
            const std::string& atk_pattern, int multi_atk_dmg);

        std::string GetName() const;
        int GetHitPoints() const;
        int GetMaxHitPoints() const;
        int GetMinAttack() const;
        int GetMaxAttack() const;
        float GetDefensePercentage() const;
        std::string GetAttackType() const;
        std::string GetDefenseType() const;
        std::string GetSkillName() const;
        std::string GetSkillDescription() const;
        int GetCost() const;
        std::string GetAttackPattern() const;
        int GetMultiAttackDamage() const;
        void TakeDamage(int damage);
        void Heal(int amount);
        bool IsAlive() const;

        void BoostMinAttack(float boost);
        void BoostMaxAttack(float boost);
        void BoostDefense(float boost);
        void BoostMaxHP(int boost);

    private:
        std::string unit_name_;
        int hit_points_;
        int max_hit_points_;
        int min_attack_;
        int max_attack_;
        float defense_percentage_;
        std::string attack_type_;
        std::string defense_type_;
        std::string skill_name_;
        std::string skill_description_;
        int cost_;
        std::string attack_pattern_;
        int multi_attack_damage_;
    };

}  

#endif  