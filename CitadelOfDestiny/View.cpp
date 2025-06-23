#include "View.h"

#include <cstdlib>  
#include <iomanip>  // для std::setw()
#include <iostream>  
#include <vector>   

namespace MyGame {

    using std::cout;
    using std::endl;
    using std::setw;
    using std::string;
    using std::vector;

    const string kRedColor = "\033[31m";
    const string kResetColor = "\033[0m";

    

    void PrintMessage(const string& message) { cout << message << endl; }

    void DrawHealthBar(int current_hp, int max_hp, int bar_length) {
        float health_ratio = static_cast<float>(current_hp) / max_hp;
        int filled_bars = static_cast<int>(health_ratio * bar_length);
        int empty_bars = bar_length - filled_bars;

        cout << "[";
        for (int i = 0; i < filled_bars; ++i) {
            cout << kRedColor << "#" << kResetColor;
        }
        for (int i = 0; i < empty_bars; ++i) {
            cout << " ";
        }
        cout << "] (" << kRedColor << current_hp << "/" << max_hp << kResetColor << ")";
    }

    void DisplayTeam(const vector<Unit>& team, bool is_player) {
        PrintMessage(is_player ? "Your Team:\n" : "Enemy Team:\n");
        for (size_t i = 0; i < team.size(); ++i) {
            cout << setw(2) << i + 1 << ". " << setw(10) << team[i].GetName() << " ";
            DrawHealthBar(team[i].GetHitPoints(), team[i].GetMaxHitPoints());
            cout << endl;
        }
    }

    void DisplayUnitDetails(const Unit& unit) {
        PrintMessage("--- Unit Details --- ");
        PrintMessage("Name: " + unit.GetName());
        cout << "HP: " << unit.GetHitPoints() << "/" << unit.GetMaxHitPoints() << endl;
        PrintMessage("Min Attack: " + std::to_string(unit.GetMinAttack()));
        PrintMessage("Max Attack: " + std::to_string(unit.GetMaxAttack()));
        PrintMessage("Defense Percentage: " + std::to_string(unit.GetDefensePercentage()));
        PrintMessage("Attack Type: " + unit.GetAttackType());
        PrintMessage("Defense Type: " + unit.GetDefenseType());
        PrintMessage("Skill: " + unit.GetSkillName() + " - " + unit.GetSkillDescription());
    }

}  