#include "Unit.h"

#include <string>  

namespace MyGame {

    Unit::Unit(const std::string& name, int hp, int max_hp, int min_atk, int max_atk,
        float def_perc, const std::string& atk_type, const std::string& def_type,
        const std::string& skill, const std::string& skill_desc, int unit_cost,
        const std::string& atk_pattern, int multi_atk_dmg)
        : unit_name_(name), hit_points_(hp), max_hit_points_(max_hp),
        min_attack_(min_atk), max_attack_(max_atk), defense_percentage_(def_perc),
        attack_type_(atk_type), defense_type_(def_type), skill_name_(skill),
        skill_description_(skill_desc), cost_(unit_cost),
        attack_pattern_(atk_pattern), multi_attack_damage_(multi_atk_dmg) {}

    std::string Unit::GetName() const { return unit_name_; }
    int Unit::GetHitPoints() const { return hit_points_; }
    int Unit::GetMaxHitPoints() const { return max_hit_points_; }
    int Unit::GetMinAttack() const { return min_attack_; }
    int Unit::GetMaxAttack() const { return max_attack_; }
    float Unit::GetDefensePercentage() const { return defense_percentage_; }
    std::string Unit::GetAttackType() const { return attack_type_; }
    std::string Unit::GetDefenseType() const { return defense_type_; }
    std::string Unit::GetSkillName() const { return skill_name_; }
    std::string Unit::GetSkillDescription() const { return skill_description_; }
    int Unit::GetCost() const { return cost_; }
    std::string Unit::GetAttackPattern() const { return attack_pattern_; }
    int Unit::GetMultiAttackDamage() const { return multi_attack_damage_; }

    void Unit::TakeDamage(int damage) {
        float damage_reduction = defense_percentage_;
        float actual_damage = damage * (1.0f - damage_reduction);

        hit_points_ -= static_cast<int>(actual_damage);
        if (hit_points_ < 0) {
            hit_points_ = 0;
        }
    }

    void Unit::Heal(int amount) {
        hit_points_ += amount;
        if (hit_points_ > max_hit_points_) {
            hit_points_ = max_hit_points_;
        }
    }

    bool Unit::IsAlive() const { return hit_points_ > 0; }

    void Unit::BoostMinAttack(float boost) { min_attack_ += static_cast<int>(boost); }
    void Unit::BoostMaxAttack(float boost) { max_attack_ += static_cast<int>(boost); }
    void Unit::BoostDefense(float boost) {
        defense_percentage_ += boost;
        if (defense_percentage_ > 1.0f) {
            defense_percentage_ = 1.0f;
        }
    }
    void Unit::BoostMaxHP(int boost) {
        max_hit_points_ += boost;
        hit_points_ += boost;
    }

}  