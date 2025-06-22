#include "View.h"
#include <cstdlib>
#include <iomanip>

namespace MyGame {

    const std::string kRedColor = "\033[31m";
    const std::string kResetColor = "\033[0m";

    void ClearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void PrintMessage(const std::string& message) {
        std::cout << message << std::endl;
    }

    void DrawHealthBar(int current_hp, int max_hp, int bar_length) {
        float health_ratio = (float)current_hp / max_hp;
        int filled_bars = (int)(health_ratio * bar_length);
        int empty_bars = bar_length - filled_bars;

        std::cout << "[";
        for (int i = 0; i < filled_bars; ++i) {
            std::cout << kRedColor << "#" << kResetColor; // Красим заполненные ячейки
        }
        for (int i = 0; i < empty_bars; ++i) {
            std::cout << " ";
        }
        std::cout << "] (" << kRedColor << current_hp << "/" << max_hp << kResetColor << ")"; // Красим значения HP
    }

    void DisplayTeam(const std::vector<Unit>& team, bool is_player) {
        PrintMessage((is_player ? "Your Team:\n" : "Enemy Team:\n"));
        for (size_t i = 0; i < team.size(); ++i) {
            std::cout << std::setw(2) << i + 1 << ". " << std::setw(10) << team[i].GetName() << " ";
            DrawHealthBar(team[i].GetHitPoints(), team[i].GetMaxHitPoints());
            std::cout << "\n";
        }
    }

    void DisplayUnitDetails(const Unit& unit) {
        PrintMessage("--- Unit Details --- ");
        PrintMessage("Name: " + unit.GetName());
        std::cout << "HP: " << unit.GetHitPoints() << "/" << unit.GetMaxHitPoints() << "\n";
        PrintMessage("Min Attack: " + std::to_string(unit.GetMinAttack()));
        PrintMessage("Max Attack: " + std::to_string(unit.GetMaxAttack()));
        PrintMessage("Defense Percentage: " + std::to_string(unit.GetDefensePercentage()));
        PrintMessage("Attack Type: " + unit.GetAttackType());
        PrintMessage("Defense Type: " + unit.GetDefenseType());
        PrintMessage("Skill: " + unit.GetSkillName() + " - " + unit.GetSkillDescription());
    }

} // namespace MyGame