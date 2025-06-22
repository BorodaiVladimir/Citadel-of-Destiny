#include "Unit.h"

namespace MyGame {
    Unit::Unit(std::string name, int hp, int max_hp, int min_atk, int max_atk, float def_perc,
        std::string atk_type, std::string def_type,
        std::string skill, std::string skill_desc, int unit_cost,
        std::string atk_pattern, int multi_atk_dmg)
        : unit_name(name), hit_points(hp), max_hit_points(max_hp),
        min_attack(min_atk), max_attack(max_atk), defense_percentage(def_perc), attack_type(atk_type),
        defense_type(def_type), skill_name(skill),
        skill_description(skill_desc), cost(unit_cost),
        attack_pattern(atk_pattern), multi_attack_damage(multi_atk_dmg) {}
    // Getters
    std::string Unit::GetName() const { return unit_name; }
    int Unit::GetHitPoints() const { return hit_points; }
    int Unit::GetMaxHitPoints() const { return max_hit_points; }
    int Unit::GetMinAttack() const { return min_attack; }
    int Unit::GetMaxAttack() const { return max_attack; }
    float Unit::GetDefensePercentage() const { return defense_percentage; }
    std::string Unit::GetAttackType() const { return attack_type; }
    std::string Unit::GetDefenseType() const { return defense_type; }
    std::string Unit::GetSkillName() const { return skill_name; }
    std::string Unit::GetSkillDescription() const { return skill_description; }
    int Unit::GetCost() const { return cost; }
    std::string Unit::GetAttackPattern() const { return attack_pattern; }
    int Unit::GetMultiAttackDamage() const { return multi_attack_damage; }
    void Unit::TakeDamage(int damage) {
        // Учитываем процентную защиту
        float damage_reduction = defense_percentage;
        float actual_damage = damage * (1.0f - damage_reduction);

        hit_points -= static_cast<int>(actual_damage);
        if (hit_points < 0) {
            hit_points = 0;
        }
    }
    void Unit::Heal(int amount) {
        hit_points += amount;
        if (hit_points > max_hit_points) {
            hit_points = max_hit_points;
        }
    }
    bool Unit::IsAlive() const { return hit_points > 0; }

    // Методы для усиления характеристик
    void Unit::BoostMinAttack(float boost) { min_attack += static_cast<int>(boost); }
    void Unit::BoostMaxAttack(float boost) { max_attack += static_cast<int>(boost); }
    void Unit::BoostDefense(float boost) { defense_percentage += boost; if (defense_percentage > 1.0f) defense_percentage = 1.0f; }
    void Unit::BoostMaxHP(int boost) { max_hit_points += boost; hit_points += boost; }
}