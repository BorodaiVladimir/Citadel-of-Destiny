#ifndef UNIT_H
#define UNIT_H

#include <string>

namespace MyGame {

    class Unit {
    private:
        std::string unit_name;
        int hit_points;
        int max_hit_points;
        int min_attack;
        int max_attack;
        float defense_percentage;
        std::string attack_type;
        std::string defense_type;
        std::string skill_name;
        std::string skill_description;
        int cost;
        std::string attack_pattern; // Single, Multi, Heal
        int multi_attack_damage;   // Урон для Multi атак

    public:
        Unit(std::string name, int hp, int max_hp, int min_atk, int max_atk, float def_perc,
            std::string atk_type, std::string def_type,
            std::string skill, std::string skill_desc, int unit_cost,
            std::string atk_pattern, int multi_atk_dmg);

        // Getters
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

        // Методы для усиления характеристик
        void BoostMinAttack(float boost);
        void BoostMaxAttack(float boost);
        void BoostDefense(float boost);
        void BoostMaxHP(int boost);
    };

} // namespace MyGame

#endif