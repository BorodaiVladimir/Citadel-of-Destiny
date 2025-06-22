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
    srand(time(0));

    // Load units from file
    std::vector<MyGame::Unit> available_units = MyGame::LoadUnitsFromFile("units.txt");
    if (available_units.empty()) {
        std::cerr << "Failed to load units. Exiting.\n";
        return 1;
    }

    // Load artifacts from file
    std::vector<MyGame::Artifact> available_artifacts = MyGame::LoadArtifactsFromFile("artifacts.txt");
    if (available_artifacts.empty()) {
        std::cerr << "Failed to load artifacts. Exiting.\n";
        return 1;
    }
    // Load boss from file
    MyGame::Boss final_boss = MyGame::LoadBossFromFile("boss.txt");


    std::vector<MyGame::Unit> player_team;
    if (available_units.size() >= 2) {
        player_team.push_back(available_units[0]); // Warrior
        player_team.push_back(available_units[1]); // Mage
    }
    else { PrintMessage("Not enough units loaded\n"); }


    MyGame::Inventory player_inventory; // Инвентарь игрока
    MyGame::ApplyArtifactEffects(player_team, player_inventory);

    int floor = 1;
    int gold = 100; // Начальное количество золота

    while (true) {
        // Вызываем основное меню, чтобы получить выбор игрока
        MyGame::HandleMainMenu(player_team, player_inventory, gold, floor, available_units, available_artifacts);

        // Дальше происходит случайное событие при переходе на новый этаж
        MyGame::ClearScreen();
        MyGame::PrintMessage("\n--- Floor " + std::to_string(floor) + " ---");

        if (floor > 10) {
            MyGame::PrintMessage("You reached the top of the tower! Prepare for the final battle!");
            MyGame::Battle(player_team, final_boss, floor); // Запускаем битву с боссом
            break;
        }
        else {
            int event = MyGame::GetRandomNumber(1, 4);

            if (event == 1) { // Бой
                MyGame::PrintMessage("You have been attacked!");
                MyGame::ApplyArtifactEffects(player_team, player_inventory);
                std::vector<MyGame::Unit> enemy_team = MyGame::GenerateRandomEnemy(available_units);
                MyGame::Battle(player_team, enemy_team, floor);
                if (player_team.empty()) break;

                gold += 20; // Награда за победу в бою
                MyGame::PrintMessage("You earned 20 gold!");

            }
            else if (event == 2) { // Сокровищница
                MyGame::PrintMessage("You found treasure!");
                if (MyGame::GetRandomNumber(1, 2) == 1) {
                    MyGame::Artifact new_artifact = MyGame::GetRandomArtifact(available_artifacts);
                    if (player_inventory.AddItem(new_artifact)) {
                        MyGame::PrintMessage("You found: " + new_artifact.GetName() + "!");
                    }
                    else {
                        MyGame::PrintMessage("Your inventory is full!");
                    }
                }
                gold += 10;
                MyGame::PrintMessage("You found 10 gold!");

            }
            else if (event == 3) { // Магазин
                MyGame::PrintMessage("You found a shop!");
                MyGame::Shop(player_team, player_inventory, gold, available_units, available_artifacts);
            }
            else { // Пустая комната
                MyGame::PrintMessage("You found an empty room.");
            }
        }

        MyGame::PrintMessage("Press Enter to continue...");
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    MyGame::PrintMessage("Game Over!");
    return 0;
}