#ifndef MYGAME_CONTROLLER_H_
#define MYGAME_CONTROLLER_H_

#include <vector>
#include <string>

#include "Unit.h"
#include "Artifact.h"
#include "Inventory.h"
#include "Boss.h"

namespace MyGame {

    std::vector<Unit> LoadUnitsFromFile(const std::string& filename);
    std::vector<Artifact> LoadArtifactsFromFile(const std::string& filename);
    Boss LoadBossFromFile(const std::string& filename);
    std::vector<Unit> GenerateRandomEnemy(const std::vector<Unit>& available_units);
    void ApplyArtifactEffects(std::vector<Unit>& team, const Inventory& inventory);
    Artifact GetRandomArtifact(const std::vector<Artifact>& available_artifacts);
    int GetRandomAttackDamage(int min_attack, int max_attack);
    void Battle(std::vector<Unit>& player_team, Boss& boss, int floor);
    void Battle(std::vector<Unit>& player_team, std::vector<Unit> enemy_team, int floor);
    void Shop(std::vector<Unit>& player_team, Inventory& player_inventory, int& gold,
        const std::vector<Unit>& available_units,
        const std::vector<Artifact>& available_artifacts);
    void HandleTeamManagement(std::vector<Unit>& player_team);
    void DisplayPlayerProfile(std::vector<Unit>& player_team, Inventory& player_inventory, int gold);
    void HandleMainMenu(std::vector<Unit>& player_team, Inventory& player_inventory, int& gold, int& floor,
        const std::vector<Unit>& available_units,
        const std::vector<Artifact>& available_artifacts);

    void InitializeGame(std::vector<Unit>& player_team, Inventory& player_inventory,
        int& gold, std::vector<Unit>& available_units,
        std::vector<Artifact>& available_artifacts,
        Boss& final_boss);
    void HandleFloorEvent(std::vector<Unit>& player_team,
        Inventory& player_inventory, int& gold, int floor,
        const std::vector<Unit>& available_units,
        const std::vector<Artifact>& available_artifacts);
    void RunGameLoop(std::vector<Unit>& player_team,
        Inventory& player_inventory, int& gold, int& floor,
        const std::vector<Unit>& available_units,
        const std::vector<Artifact>& available_artifacts,
        Boss& final_boss);

}  

#endif  