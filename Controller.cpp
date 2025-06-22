// Controller.cpp
#include "Controller.h"
#include "View.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include "Utils.h"
#include <cstdlib>
#include <iomanip> // Добавляем этот заголовочный файл

namespace MyGame {

    std::vector<Unit> LoadUnitsFromFile(const std::string& filename) {
        std::vector<Unit> units;
        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return units; // Return an empty vector in case of error
        }

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string value;
            std::vector<std::string> values;

            while (std::getline(ss, value, ',')) {
                values.push_back(value);
            }

            if (values.size() == 13) { // Обновлено количество значений
                Unit unit(
                    values[0],                   // unit_name
                    std::stoi(values[1]),         // hit_points
                    std::stoi(values[2]),         // max_hit_points
                    std::stoi(values[3]),         // min_attack
                    std::stoi(values[4]),         // max_attack
                    std::stof(values[5]),         // defense_percentage
                    values[6],                   // attack_type
                    values[7],                   // defense_type
                    values[8],                   // skill_name
                    values[9],                    // skill_description
                    std::stoi(values[10]),        // unit_cost
                    values[11],                   // attack_pattern
                    std::stoi(values[12])       // multi_attack_damage
                );

                units.push_back(unit);
            }
            else {
                std::cerr << "Invalid unit data: " << line << std::endl;
            }
        }

        file.close();
        return units;
    }

    std::vector<Artifact> LoadArtifactsFromFile(const std::string& filename) {
        std::vector<Artifact> artifacts;
        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) {
            std::cerr << "Error opening artifact file: " << filename << std::endl;
            return artifacts; // Return an empty vector in case of error
        }

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string value;
            std::vector<std::string> values;

            while (std::getline(ss, value, ',')) {
                values.push_back(value);
            }

            if (values.size() == 5) {
                Artifact artifact(
                    values[0],                // artifact_name
                    values[1],                // description
                    values[2],                // effect_type
                    std::stof(values[3]),      // effect_value
                    std::stoi(values[4])       // artifact_cost
                );

                artifacts.push_back(artifact);
            }
            else {
                std::cerr << "Invalid artifact data: " << line << std::endl;
            }
        }

        file.close();
        return artifacts;
    }

    Boss LoadBossFromFile(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) {
            std::cerr << "Error opening boss file: " << filename << std::endl;
            // Return a default Boss to prevent crash or handle error appropriately
            return { "Default Boss", 100, 100, 10, 15, 0.1f, "Physical", "Armor", "Strike", "Default attack",100, 10, 5,"Single",0 };
        }

        if (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string value;
            std::vector<std::string> values;

            while (std::getline(ss, value, ',')) {
                values.push_back(value);
            }

            if (values.size() == 15) {
                Boss boss(
                    values[0],                // unit_name
                    std::stoi(values[1]),      // hit_points
                    std::stoi(values[2]),      // max_hit_points
                    std::stoi(values[3]),      // min_attack
                    std::stoi(values[4]),      // max_attack
                    std::stof(values[5]),      // defense_percentage
                    values[6],                // attack_type
                    values[7],                // defense_type
                    values[8],                // skill_name
                    values[9],                // skill_description
                    std::stoi(values[10]),     // unit_cost
                    std::stoi(values[11]),     // heal_amount
                    std::stoi(values[12]),     // block_amount
                    values[13],                   // attack_pattern
                    std::stoi(values[14])       // multi_attack_damage
                );
                file.close();
                return boss;
            }
            else {
                std::cerr << "Invalid boss data: " << line << std::endl;
            }
        }
        else {
            std::cerr << "No boss data found in file.\n";
        }

        file.close();
        // Return a default Boss to prevent crash or handle error appropriately
        return { "Default Boss", 100, 100, 10, 15, 0.1f, "Physical", "Armor", "Strike", "Default attack", 100, 10, 5,"Single",0 };
    }

    std::vector<Unit> GenerateRandomEnemy(const std::vector<Unit>& available_units) {
        std::vector<Unit> enemy_team;
        int num_enemies = GetRandomNumber(1, 3); // Случайное количество врагов в команде

        for (int i = 0; i < num_enemies; ++i) {
            if (!available_units.empty()) {
                // Случайно выбираем юнита из списка доступных
                int unit_index = GetRandomNumber(0, available_units.size() - 1);
                Unit enemy = available_units[unit_index];

                enemy_team.push_back(enemy);
            }
            else {
                std::cerr << "No units available to generate enemy team!\n";
                return {}; // Возвращаем пустую команду
            }
        }
        return enemy_team;
    }

    void ApplyArtifactEffects(std::vector<Unit>& team, const Inventory& inventory) {
        for (auto& unit : team) {
            for (const auto& artifact : inventory.GetArtifacts()) {
                if (artifact.GetEffectType() == "ATTACK_BOOST") {
                    unit.BoostMinAttack(artifact.GetEffectValue());
                    unit.BoostMaxAttack(artifact.GetEffectValue());
                }
                else if (artifact.GetEffectType() == "DEFENSE_BOOST") {
                    unit.BoostDefense(artifact.GetEffectValue());
                }
                else if (artifact.GetEffectType() == "HP_BOOST") {
                    unit.BoostMaxHP(static_cast<int>(artifact.GetEffectValue()));
                }
            }
        }
    }

    Artifact GetRandomArtifact(const std::vector<Artifact>& available_artifacts) {
        if (available_artifacts.empty()) {
            std::cerr << "No artifacts available!\n";
            // Return a default artifact to prevent crash or handle error appropriately
            return { "Default Artifact", "No effect", "NONE", 0.0f, 0 };
        }

        int index = GetRandomNumber(0, available_artifacts.size() - 1);
        return available_artifacts[index];
    }

    int GetRandomAttackDamage(int min_attack, int max_attack) {
        return GetRandomNumber(min_attack, max_attack);
    }

    void Battle(std::vector<Unit>& player_team, Boss& boss, int floor) {
        using namespace MyGame;
        while (!player_team.empty() && boss.IsAlive()) {
            ClearScreen();
            PrintMessage("\n--- Floor " + std::to_string(floor) + " - Final Battle! --- ");
            DisplayTeam(player_team, true);
            std::cout << "\n";
            std::cout << "Boss: " << boss.GetName() << " ";
            DrawHealthBar(boss.GetHitPoints(), boss.GetMaxHitPoints());
            std::cout << "\n\n";

            // --- Your Turn ---
            PrintMessage("--- Your Turn --- ");
            std::cout << "Choose a unit to act with (1-" << player_team.size() << "): ";
            int player_choice;
            std::cin >> player_choice;

            while (std::cin.fail() || player_choice < 1 || player_choice > player_team.size()) {
                PrintMessage("Invalid input. Please enter a number between 1 and " + std::to_string(player_team.size()) + ": ");
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> player_choice;
            }

            if (player_choice > 0 && player_choice <= player_team.size()) {
                int unit_index = player_choice - 1;

                if (player_team[unit_index].GetAttackPattern() == "Heal") {
                    PrintMessage("Healer's actions: ");
                    PrintMessage("1. Heal Ally");
                    PrintMessage("2. Skip Turn");
                    std::cout << "Enter your choice: ";
                    int healer_choice;
                    std::cin >> healer_choice;

                    while (std::cin.fail() || (healer_choice != 1 && healer_choice != 2)) {
                        PrintMessage("Invalid input. Please enter 1 or 2: ");
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cin >> healer_choice;
                    }
                    if (healer_choice == 1) {
                        std::cout << "Choose an ally to heal (1-" << player_team.size() << "): ";
                        int target_choice;
                        std::cin >> target_choice;

                        while (std::cin.fail() || target_choice < 1 || target_choice > player_team.size()) {
                            PrintMessage("Invalid input. Please enter a number between 1 and " + std::to_string(player_team.size()) + ": ");
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cin >> target_choice;
                        }
                        int target_index = target_choice - 1;
                        int heal_amount = GetRandomNumber(20, 40); // Пример: Лечение на 20-40 HP
                        player_team[target_index].Heal(heal_amount);
                        PrintMessage(player_team[unit_index].GetName() + " heals " + player_team[target_index].GetName() + " for " + std::to_string(heal_amount) + " HP.");

                    }
                    else {
                        PrintMessage(player_team[unit_index].GetName() + " skips turn.");
                    }

                }
                else if (player_team[unit_index].GetAttackPattern() == "Multi") {
                    int damage = GetRandomAttackDamage(player_team[unit_index].GetMinAttack(), player_team[unit_index].GetMaxAttack());
                    std::cout << player_team[unit_index].GetName() << " attacks boss dealing " << damage << " damage.\n";
                    boss.TakeDamage(damage);


                }
                else {
                    // Если юнит не лекарь - атака
                    int damage = GetRandomAttackDamage(player_team[unit_index].GetMinAttack(), player_team[unit_index].GetMaxAttack());

                    PrintMessage(player_team[unit_index].GetName() + " attacks " + boss.GetName() + " and deals " + std::to_string(damage) + " damage.");
                    boss.TakeDamage(damage);
                }

                if (!boss.IsAlive()) {
                    PrintMessage(boss.GetName() + " is defeated!");
                    break;
                }
            }

            // --- Boss Turn ---
            if (boss.IsAlive() && !player_team.empty()) {
                ClearScreen();
                PrintMessage("\n--- Floor " + std::to_string(floor) + " - Final Battle! --- ");
                DisplayTeam(player_team, true);
                std::cout << "\n";
                std::cout << "Boss: " << boss.GetName() << " ";
                // Controller.cpp
                DrawHealthBar(boss.GetHitPoints(), boss.GetMaxHitPoints(), 10); // Добавлен третий аргумент
                std::cout << "\n\n";
                PrintMessage("--- Boss Turn --- ");

                int boss_action = boss.ChooseAction();

                if (boss.GetAttackPattern() == "Multi") {
                    int damage = GetRandomAttackDamage(boss.GetMinAttack(), boss.GetMaxAttack());
                    std::cout << boss.GetName() << " attacks all players dealing " << damage << " damage.\n";
                    for (size_t i = 0; i < player_team.size(); ++i) {
                        player_team[i].TakeDamage(damage);
                        if (!player_team[i].IsAlive()) {
                            PrintMessage(player_team[i].GetName() + " dies!");
                            player_team.erase(player_team.begin() + i);
                            i--; // Уменьшаем индекс, т.к. удалили элемент
                        }
                    }
                }
                else {
                    if (boss_action == 1) { // Single Target Attack
                        int player_target_index = GetRandomNumber(0, player_team.size() - 1);
                        // Получаем случайный урон
                        int damage = GetRandomAttackDamage(boss.GetMinAttack(), boss.GetMaxAttack());
                        PrintMessage(boss.GetName() + " attacks " + player_team[player_target_index].GetName() + " and deals " + std::to_string(damage) + " damage.");
                        player_team[player_target_index].TakeDamage(damage);
                        if (!player_team[player_target_index].IsAlive()) {
                            PrintMessage(player_team[player_target_index].GetName() + " dies!");
                            player_team.erase(player_team.begin() + player_target_index);
                        }
                    }
                    else if (boss_action == 2) { // AoE Attack
                        PrintMessage(boss.GetName() + " unleashes a devastating AoE attack!");
                        for (size_t i = 0; i < player_team.size(); ++i) {
                            // Получаем случайный урон
                            int damage = GetRandomAttackDamage(boss.GetMinAttack(), boss.GetMaxAttack()) / 2;
                            player_team[i].TakeDamage(damage);
                            PrintMessage(player_team[i].GetName() + " takes " + std::to_string(damage) + " damage.");

                            if (!player_team[i].IsAlive()) {
                                PrintMessage(player_team[i].GetName() + " dies!");
                                player_team.erase(player_team.begin() + i);
                                i--; // Уменьшаем индекс, т.к. удалили элемент
                            }
                        }
                    }
                    else if (boss_action == 3) { // Heal
                        int heal_amount = boss.GetHealAmount();
                        boss.Heal(heal_amount);

                        PrintMessage(boss.GetName() + " heals himself for " + std::to_string(heal_amount) + " HP.");
                    }
                    else if (boss_action == 4) { // Block
                        PrintMessage(boss.GetName() + " braces for impact, blocking incoming damage!");
                        // В этом примере, просто выводим сообщение. Снижение урона реализуется в начале следующего хода игрока.
                    }
                }

                PrintMessage("Press Enter to continue...");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }

            if (player_team.empty()) {
                PrintMessage("You lost!");
                return;
            }
            if (!boss.IsAlive()) {
                PrintMessage("You won!");
                return;
            }
        }
    }

    void Battle(std::vector<Unit>& player_team, std::vector<Unit> enemy_team, int floor) {
        using namespace MyGame;
        while (!player_team.empty() && !enemy_team.empty()) {
            ClearScreen();
            PrintMessage("\n--- Floor " + std::to_string(floor) + " --- ");
            DisplayTeam(player_team, true);
            std::cout << "\n";
            DisplayTeam(enemy_team, false);
            std::cout << "\n";

            PrintMessage("--- Your Turn --- ");
            std::cout << "Choose a unit to act with (1-" << player_team.size() << "): ";
            int player_choice;
            std::cin >> player_choice;

            while (std::cin.fail() || player_choice < 1 || player_choice > player_team.size()) {
                PrintMessage("Invalid input. Please enter a number between 1 and " + std::to_string(player_team.size()) + ": ");
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> player_choice;
            }

            if (player_choice > 0 && player_choice <= player_team.size()) {
                int unit_index = player_choice - 1;
                if (player_team[unit_index].GetAttackPattern() == "Heal") {
                    PrintMessage("Healer's actions: ");
                    PrintMessage("1. Heal Ally");
                    PrintMessage("2. Skip Turn");
                    std::cout << "Enter your choice: ";
                    int healer_choice;
                    std::cin >> healer_choice;

                    while (std::cin.fail() || (healer_choice != 1 && healer_choice != 2)) {
                        PrintMessage("Invalid input. Please enter 1 or 2: ");
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cin >> healer_choice;
                    }
                    if (healer_choice == 1) {
                        std::cout << "Choose an ally to heal (1-" << player_team.size() << "): ";
                        int target_choice;
                        std::cin >> target_choice;

                        while (std::cin.fail() || target_choice < 1 || target_choice > player_team.size()) {
                            PrintMessage("Invalid input. Please enter a number between 1 and " + std::to_string(player_team.size()) + ": ");
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cin >> target_choice;
                        }
                        int target_index = target_choice - 1;
                        int heal_amount = GetRandomNumber(20, 40); // Пример: Лечение на 20-40 HP
                        player_team[target_index].Heal(heal_amount);
                        PrintMessage(player_team[unit_index].GetName() + " heals " + player_team[target_index].GetName() + " for " + std::to_string(heal_amount) + " HP.");

                    }
                    else {
                        PrintMessage(player_team[unit_index].GetName() + " skips turn.");
                    }

                }
                else if (player_team[unit_index].GetAttackPattern() == "Multi") {
                    int damage = GetRandomAttackDamage(player_team[unit_index].GetMinAttack(), player_team[unit_index].GetMaxAttack());
                    std::cout << player_team[unit_index].GetName() << " attacks all enemies dealing " << damage << " damage.\n";
                    for (size_t i = 0; i < enemy_team.size(); ++i) {
                        enemy_team[i].TakeDamage(damage);
                        if (!enemy_team[i].IsAlive()) {
                            PrintMessage(enemy_team[i].GetName() + " dies!");
                            enemy_team.erase(enemy_team.begin() + i);
                            i--; // Уменьшаем индекс, т.к. удалили элемент
                        }
                    }

                }
                else {
                    // Если юнит не лекарь - атака
                    std::cout << "Choose a target to attack (1-" << enemy_team.size() << "): ";
                    int target_choice;
                    std::cin >> target_choice;

                    while (std::cin.fail() || target_choice < 1 || target_choice > enemy_team.size()) {
                        PrintMessage("Invalid input. Please enter a number between 1 and " + std::to_string(enemy_team.size()) + ": ");
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cin >> target_choice;
                    }

                    // Объявляем player_damage_dealt здесь, внутри блока if (player_choice > 0)
                    int player_damage_dealt = 0;
                    int target_index = target_choice - 1;
                    // Получаем случайный урон
                    int damage = GetRandomAttackDamage(player_team[unit_index].GetMinAttack(), player_team[unit_index].GetMaxAttack());
                    player_damage_dealt = damage;
                    PrintMessage(player_team[unit_index].GetName() + " attacks " + enemy_team[target_index].GetName() + " and deals " + std::to_string(damage) + " damage.");
                    enemy_team[target_index].TakeDamage(damage);

                    if (!enemy_team[target_index].IsAlive()) {
                        PrintMessage(enemy_team[target_index].GetName() + " dies!");
                        enemy_team.erase(enemy_team.begin() + target_index);
                    }

                    //Выводим информацию об уроне здесь, после проверки на валидность
                    PrintMessage("Your unit dealt " + std::to_string(player_damage_dealt) + " damage.");
                }

            }

            if (!enemy_team.empty() && !player_team.empty()) {
                ClearScreen();
                PrintMessage("\n--- Floor " + std::to_string(floor) + " --- ");
                DisplayTeam(player_team, true);
                std::cout << "\n";
                DisplayTeam(enemy_team, false);
                std::cout << "\n";
                PrintMessage("--- Enemy Turn --- ");

                // Инициализируем переменные здесь, чтобы они всегда были объявлены
                int enemy_damage_dealt = 0;

                int enemy_unit_index = GetRandomNumber(0, enemy_team.size() - 1);
                if (enemy_team[enemy_unit_index].GetAttackPattern() == "Heal") {
                    // Лечит самого поврежденного союзника
                    int target_index = 0;
                    for (size_t i = 1; i < enemy_team.size(); ++i) {
                        if (enemy_team[i].GetHitPoints() < enemy_team[target_index].GetHitPoints()) {
                            target_index = i;
                        }
                    }
                    int heal_amount = GetRandomNumber(20, 40);
                    enemy_team[target_index].Heal(heal_amount);
                    PrintMessage(enemy_team[enemy_unit_index].GetName() + " heals " + enemy_team[target_index].GetName() + " for " + std::to_string(heal_amount) + " HP.");

                }
                else if (enemy_team[enemy_unit_index].GetAttackPattern() == "Multi") {
                    int damage = GetRandomAttackDamage(enemy_team[enemy_unit_index].GetMinAttack(), enemy_team[enemy_unit_index].GetMaxAttack());
                    std::cout << enemy_team[enemy_unit_index].GetName() << " attacks all players dealing " << damage << " damage.\n";
                    for (size_t i = 0; i < player_team.size(); ++i) {
                        player_team[i].TakeDamage(damage);
                        if (!player_team[i].IsAlive()) {
                            PrintMessage(player_team[i].GetName() + " dies!");
                            player_team.erase(player_team.begin() + i);
                            i--; // Уменьшаем индекс, т.к. удалили элемент
                        }
                    }
                }

                else {
                    // Случайный выбор юнита для атаки
                    int player_target_index = GetRandomNumber(0, player_team.size() - 1);
                    int damage = GetRandomAttackDamage(enemy_team[enemy_unit_index].GetMinAttack(), enemy_team[enemy_unit_index].GetMaxAttack());
                    enemy_damage_dealt = damage;
                    PrintMessage(enemy_team[enemy_unit_index].GetName() + " attacks " + player_team[player_target_index].GetName() + " and deals " + std::to_string(damage) + " damage.");
                    player_team[player_target_index].TakeDamage(damage);

                    if (!player_team[player_target_index].IsAlive()) {
                        PrintMessage(player_team[player_target_index].GetName() + " dies!");
                        player_team.erase(player_team.begin() + player_target_index);
                    }
                }


                PrintMessage("Enemy unit dealt " + std::to_string(enemy_damage_dealt) + " damage.");

                std::cout << "Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }

            if (player_team.empty()) {
                PrintMessage("You lost!");
                return;
            }
            if (enemy_team.empty()) {
                PrintMessage("You won!");
                return;
            }
        }
    }

    void Shop(std::vector<Unit>& player_team, Inventory& player_inventory, int& gold, const std::vector<Unit>& available_units, const std::vector<Artifact>& available_artifacts) {
        // Случайный выбор юнита и артефакта (выносим за пределы цикла)
        Unit random_unit = available_units[GetRandomNumber(0, available_units.size() - 1)];
        Artifact random_artifact = GetRandomArtifact(available_artifacts);

        while (true) { // Добавляем цикл, чтобы оставаться в магазине
            ClearScreen(); // Очищаем экран перед каждым отображением меню
            PrintMessage("\n--- Welcome to the Shop! --- ");
            PrintMessage("You have " + std::to_string(gold) + " gold.");
            PrintMessage("What would you like to buy?");

            // Цены
            int artifact_price = random_artifact.GetCost();
            int unit_price = random_unit.GetCost();

            // List items for sale (Units, Items, Artifacts)
            std::cout << "1. " << random_artifact.GetName() << " (" << artifact_price << " gold) - " << random_artifact.GetDescription() << "\n";
            std::cout << "2. Hire " << random_unit.GetName() << " (" << unit_price << " gold)\n";
            std::cout << "3. Exit\n";

            int input_choice; //Объявляем переменную здесь
            std::cout << "Enter your choice: ";
            std::cin >> input_choice;

            while (std::cin.fail()) {
                PrintMessage("Invalid input. Please enter a number: ");
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> input_choice;
            }

            if (input_choice == 1) { // Artifact
                if (gold >= artifact_price) {
                    if (player_inventory.AddItem(random_artifact)) {
                        PrintMessage("You bought: " + random_artifact.GetName() + "!");
                        gold -= artifact_price;
                    }
                    else {
                        PrintMessage("Your inventory is full!");
                    }
                }
                else {
                    PrintMessage("Not enough gold!");
                }
            }
            else if (input_choice == 2) { // Hire Random Unit
                if (gold >= unit_price && player_team.size() < 3) {
                    player_team.push_back(random_unit);
                    PrintMessage("You hired a " + random_unit.GetName() + "!");
                    gold -= unit_price;
                }
                else if (player_team.size() >= 3) {
                    PrintMessage("Your team is full!");
                }
                else {
                    PrintMessage("Not enough gold!");
                }
            }
            else if (input_choice == 3) { // Exit
                break; // Выход из цикла магазина
            }
            else {
                PrintMessage("Invalid choice!");
            }

            PrintMessage("Press Enter to continue shopping...");
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
    }

    void HandleTeamManagement(std::vector<Unit>& player_team) {
        while (true) {
            MyGame::ClearScreen();
            PrintMessage("--- Team Management ---");
            MyGame::DisplayTeam(player_team, true);

            PrintMessage("\nWhat do you want to do?");
            PrintMessage("1. View Unit Details");
            PrintMessage("2. Dismiss Unit");
            PrintMessage("3. Back to Profile");
            std::cout << "Enter your choice: ";

            int input_choice; //объявляем здесь
            std::cin >> input_choice;

            while (std::cin.fail()) {
                PrintMessage("Invalid input. Please enter a number: ");
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> input_choice;
            }

            if (input_choice == 1) {
                // Просмотр информации о юните
                if (player_team.empty()) {
                    PrintMessage("Your team is empty!");
                }
                else {
                    std::cout << "Enter the number of the unit to view (1-" << player_team.size() << "): ";
                    int unit_index;
                    std::cin >> unit_index;

                    while (std::cin.fail() || unit_index < 1 || unit_index > player_team.size()) {
                        PrintMessage("Invalid input. Please enter a number between 1 and " + std::to_string(player_team.size()) + ": ");
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cin >> unit_index;
                    }

                    DisplayUnitDetails(player_team[unit_index - 1]); // Показываем детали выбранного юнита
                }

                PrintMessage("Press Enter to continue...");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();

            }
            else if (input_choice == 2) {
                // Увольнение юнита
                if (player_team.empty()) {
                    PrintMessage("Your team is empty!");
                }
                else {
                    std::cout << "Enter the number of the unit to dismiss (1-" << player_team.size() << "): ";
                    int unit_index;
                    std::cin >> unit_index;

                    while (std::cin.fail() || unit_index < 1 || unit_index > player_team.size()) {
                        PrintMessage("Invalid input. Please enter a number between 1 and " + std::to_string(player_team.size()) + ": ");
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cin >> unit_index;
                    }

                    std::cout << "Are you sure you want to dismiss " << player_team[unit_index - 1].GetName() << "? (y/n): ";
                    char confirm;
                    std::cin >> confirm;

                    if (confirm == 'y' || confirm == 'Y') {
                        PrintMessage(player_team[unit_index - 1].GetName() + " has been dismissed.");
                        player_team.erase(player_team.begin() + unit_index - 1);
                    }
                    else {
                        PrintMessage("Dismissal cancelled.");
                    }
                }

                PrintMessage("Press Enter to continue...");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();

            }
            else if (input_choice == 3) {
                break; // Возврат в профиль
            }
            else {
                PrintMessage("Invalid choice. Please try again.");
                PrintMessage("Press Enter to continue...");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }
        }
    }

    void DisplayPlayerProfile(std::vector<Unit>& player_team, Inventory& player_inventory, int gold) {
        while (true) {
            MyGame::ClearScreen();
            PrintMessage("--- Player Profile ---");
            PrintMessage("Gold: " + std::to_string(gold));

            std::cout << "\n";

            PrintMessage("--- Team ---");
            MyGame::DisplayTeam(player_team, true);
            std::cout << "\n";

            PrintMessage("--- Inventory ---");
            if (player_inventory.GetArtifacts().empty()) {
                PrintMessage("Inventory is empty.");
            }
            else {
                const auto& artifacts = player_inventory.GetArtifacts();
                for (size_t i = 0; i < artifacts.size(); ++i) {
                    std::cout << std::setw(2) << i + 1 << ". " << artifacts[i].GetName()
                        << " - " << artifacts[i].GetDescription() << "\n";
                }
            }

            std::cout << "\nWhat do you want to do?\n";
            PrintMessage("1. Manage Team");
            PrintMessage("2. Back to Main Menu");
            std::cout << "Enter your choice: ";

            int input_choice; //Объявляем переменную здесь

            std::cin >> input_choice;

            while (std::cin.fail()) {
                PrintMessage("Invalid input. Please enter a number: ");
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> input_choice;
            }

            if (input_choice == 1) {
                HandleTeamManagement(player_team);
            }
            else if (input_choice == 2) {
                break; // Возврат в главное меню
            }
            else {
                PrintMessage("Invalid choice. Please try again.");
                PrintMessage("Press Enter to continue...");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }
        }
    }

    void HandleMainMenu(std::vector<Unit>& player_team, Inventory& player_inventory, int& gold, int& floor, const std::vector<Unit>& available_units, const std::vector<Artifact>& available_artifacts) {
        while (true) {
            MyGame::ClearScreen();

            PrintMessage("\n--- Floor " + std::to_string(floor) + " --- ");
            PrintMessage("What do you do?");
            PrintMessage("1. Go to the next floor");
            PrintMessage("2. View Profile");
            std::cout << "Enter your choice: ";

            int input_choice; //Объявляем переменную здесь

            std::cin >> input_choice;

            while (std::cin.fail()) {
                PrintMessage("Invalid input. Please enter a number: ");
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> input_choice;
            }

            if (input_choice == 1) {
                floor++; // Переход на следующий этаж
                break;   // Выход из цикла меню
            }
            else if (input_choice == 2) {
                DisplayPlayerProfile(player_team, player_inventory, gold);
            }
            else {
                PrintMessage("Invalid choice. Please try again.");
                PrintMessage("Press Enter to continue...");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }
        }
    }
} // namespace MyGame