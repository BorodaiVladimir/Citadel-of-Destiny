#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

#include "Controller.h"
#include "View.h"
#include "Unit.h"
#include "Artifact.h"
#include "Inventory.h"
#include "Utils.h"
#include "Boss.h"
  
int main() {
    using namespace MyGame;

    std::vector<MyGame::Unit> player_team;
    MyGame::Inventory player_inventory;
    int gold;
    std::vector<MyGame::Unit> available_units;
    std::vector<MyGame::Artifact> available_artifacts;
    MyGame::Boss final_boss;
    int floor = 1;

    InitializeGame(player_team, player_inventory, gold, available_units,
        available_artifacts, final_boss);

    RunGameLoop(player_team, player_inventory, gold, floor, available_units,
        available_artifacts, final_boss);

    PrintMessage("Game Over!");
    return 0;
}