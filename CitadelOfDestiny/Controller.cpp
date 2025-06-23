#include "Controller.h"
#include "View.h"
#include "Utils.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

namespace MyGame {

    using std::cerr;
    using std::cin;
    using std::cout;
    using std::endl;
    using std::ifstream;
    using std::stoi;
    using std::stof;
    using std::string;
    using std::stringstream;
    using std::vector;
    using std::setw;

    vector<Unit> LoadUnitsFromFile(const string& filename) {
        vector<Unit> units;
        ifstream file(filename);
        string line;

        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return units;
        }

        while (getline(file, line)) {
            stringstream ss(line);
            string value;
            vector<string> values;

            while (getline(ss, value, ',')) {
                values.push_back(value);
            }

            if (values.size() == 13) {
                Unit unit(values[0], stoi(values[1]), stoi(values[2]), stoi(values[3]),
                    stoi(values[4]), stof(values[5]), values[6], values[7], values[8],
                    values[9], stoi(values[10]), values[11], stoi(values[12]));
                units.push_back(unit);
            }
            else {
                cerr << "Invalid unit data: " << line << endl;
            }
        }

        file.close();
        return units;
    }

    vector<Artifact> LoadArtifactsFromFile(const string& filename) {
        vector<Artifact> artifacts;
        ifstream file(filename);
        string line;

        if (!file.is_open()) {
            cerr << "Error opening artifact file: " << filename << endl;
            return artifacts;
        }

        while (getline(file, line)) {
            stringstream ss(line);
            string value;
            vector<string> values;

            while (getline(ss, value, ',')) {
                values.push_back(value);
            }

            if (values.size() == 5) {
                Artifact artifact(values[0], values[1], values[2], stof(values[3]), stoi(values[4]));
                artifacts.push_back(artifact);
            }
            else {
                cerr << "Invalid artifact data: " << line << endl;
            }
        }

        file.close();
        return artifacts;
    }

    Boss LoadBossFromFile(const string& filename) {
        ifstream file(filename);
        string line;

        if (!file.is_open()) {
            cerr << "Error opening boss file: " << filename << endl;
            
            return { "Default Boss", 100, 100, 10, 15, 0.1f, "Physical", "Armor", "Strike", "Default attack",100, 10, 5,"Single",0 };
        }

        if (getline(file, line)) {
            stringstream ss(line);
            string value;
            vector<string> values;

            while (getline(ss, value, ',')) {
                values.push_back(value);
            }

            if (values.size() == 15) {
                Boss boss(values[0], stoi(values[1]), stoi(values[2]), stoi(values[3]),
                    stoi(values[4]), stof(values[5]), values[6], values[7], values[8],
                    values[9], stoi(values[10]), stoi(values[11]), stoi(values[12]), values[13], stoi(values[14]));
                file.close();
                return boss;
            }
            else {
                cerr << "Invalid boss data: " << line << endl;
            }
        }
        else {
            cerr << "No boss data found in file.\n";
        }

        file.close();
        
        return { "Default Boss", 100, 100, 10, 15, 0.1f, "Physical", "Armor", "Strike", "Default attack", 100, 10, 5,"Single",0 };
    }

    vector<Unit> GenerateRandomEnemy(const vector<Unit>& available_units) {
        vector<Unit> enemy_team;
        const int kMinEnemies = 1;
        const int kMaxEnemies = 3;
        int num_enemies = GetRandomNumber(kMinEnemies, kMaxEnemies);

        for (int i = 0; i < num_enemies; ++i) {
            if (!available_units.empty()) {
                int unit_index = GetRandomNumber(0, available_units.size() - 1);
                Unit enemy = available_units[unit_index];
                enemy_team.push_back(enemy);
            }
            else {
                cerr << "No units available to generate enemy team!\n";
                return {};
            }
        }
        return enemy_team;
    }

    void ApplyArtifactEffects(vector<Unit>& team, const Inventory& inventory) {
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

    Artifact GetRandomArtifact(const vector<Artifact>& available_artifacts) {
        if (available_artifacts.empty()) {
            cerr << "No artifacts available!\n";
            return { "Default Artifact", "No effect", "NONE", 0.0f, 0 };
        }

        int index = GetRandomNumber(0, available_artifacts.size() - 1);
        return available_artifacts[index];
    }

    int GetRandomAttackDamage(int min_attack, int max_attack) {
        return GetRandomNumber(min_attack, max_attack);
    }

    void Battle(vector<Unit>& player_team, Boss& boss, int floor) {
        using namespace MyGame;
        while (!player_team.empty() && boss.IsAlive()) {
            
            PrintMessage("\n--- Floor " + std::to_string(floor) + " - Final Battle! --- ");
            DisplayTeam(player_team, true);
            cout << "\n";
            cout << "Boss: " << boss.GetName() << " ";
            DrawHealthBar(boss.GetHitPoints(), boss.GetMaxHitPoints(), 10);
            cout << "\n\n";

            // --- Your Turn ---
            PrintMessage("--- Your Turn --- ");
            cout << "Choose a unit to act with (1-" << player_team.size() << "): ";
            int player_choice;
            cin >> player_choice;

            while (cin.fail() || player_choice < 1 || player_choice > player_team.size()) {
                PrintMessage("Invalid input. Please enter a number between 1 and " + std::to_string(player_team.size()) + ": ");
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> player_choice;
            }

            if (player_choice > 0 && player_choice <= player_team.size()) {
                int unit_index = player_choice - 1;

                if (player_team[unit_index].GetAttackPattern() == "Heal") {
                    PrintMessage("Healer's actions: ");
                    PrintMessage("1. Heal Ally");
                    PrintMessage("2. Skip Turn");
                    cout << "Enter your choice: ";
                    int healer_choice;
                    cin >> healer_choice;

                    while (cin.fail() || (healer_choice != 1 && healer_choice != 2)) {
                        PrintMessage("Invalid input. Please enter 1 or 2: ");
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        cin >> healer_choice;
                    }
                    if (healer_choice == 1) {
                        cout << "Choose an ally to heal (1-" << player_team.size() << "): ";
                        int target_choice;
                        cin >> target_choice;

                        while (cin.fail() || target_choice < 1 || target_choice > player_team.size()) {
                            PrintMessage("Invalid input. Please enter a number between 1 and " + std::to_string(player_team.size()) + ": ");
                            cin.clear();
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            cin >> target_choice;
                        }
                        int target_index = target_choice - 1;
                        const int kHealAmountMin = 10;
                        const int kHealAmountMax = 20;
                        int heal_amount = GetRandomNumber(kHealAmountMin, kHealAmountMax); 
                        player_team[target_index].Heal(heal_amount);
                        PrintMessage(player_team[unit_index].GetName() + " heals " + player_team[target_index].GetName() + " for " + std::to_string(heal_amount) + " HP.");

                    }
                    else {
                        PrintMessage(player_team[unit_index].GetName() + " skips turn.");
                    }

                }
                else if (player_team[unit_index].GetAttackPattern() == "Multi") {
                    int damage = GetRandomAttackDamage(player_team[unit_index].GetMinAttack(), player_team[unit_index].GetMaxAttack());
                    cout << player_team[unit_index].GetName() << " attacks boss dealing " << damage << " damage.\n";
                    boss.TakeDamage(damage);


                }
                else {
                    int damage = GetRandomAttackDamage(player_team[unit_index].GetMinAttack(), player_team[unit_index].GetMaxAttack());

                    PrintMessage(player_team[unit_index].GetName() + " attacks " + boss.GetName() + " and deals " + std::to_string(damage) + " damage.");
                    boss.TakeDamage(damage);
                }

                if (!boss.IsAlive()) {
                    PrintMessage("Boss is defeated!");
                    break;
                }
            }

            // --- Boss Turn ---
            if (boss.IsAlive() && !player_team.empty()) {
                
                PrintMessage("\n--- Floor " + std::to_string(floor) + " - Final Battle! --- ");
                DisplayTeam(player_team, true);
                cout << "\n";
                cout << "Boss: " << boss.GetName() << " ";
                DrawHealthBar(boss.GetHitPoints(), boss.GetMaxHitPoints(), 10);
                cout << "\n\n";
                PrintMessage("--- Boss Turn --- ");

                int boss_action = boss.ChooseAction();

                if (boss.GetAttackPattern() == "Multi") {
                    int damage = GetRandomAttackDamage(boss.GetMinAttack(), boss.GetMaxAttack());
                    cout << boss.GetName() << " attacks all players dealing " << damage << " damage.\n";
                    for (size_t i = 0; i < player_team.size(); ++i) {
                        player_team[i].TakeDamage(damage);
                        if (!player_team[i].IsAlive()) {
                            PrintMessage(player_team[i].GetName() + " dies!");
                            player_team.erase(player_team.begin() + i);
                            i--;
                        }
                    }
                }
                else {
                    if (boss_action == 1) { // Одиночная атака
                        int player_target_index = GetRandomNumber(0, player_team.size() - 1);
                        int damage = GetRandomAttackDamage(boss.GetMinAttack(), boss.GetMaxAttack());
                        PrintMessage(boss.GetName() + " attacks " + player_team[player_target_index].GetName() + " and deals " + std::to_string(damage) + " damage.");
                        player_team[player_target_index].TakeDamage(damage);
                        if (!player_team[player_target_index].IsAlive()) {
                            PrintMessage(player_team[player_target_index].GetName() + " dies!");
                            player_team.erase(player_team.begin() + player_target_index);
                        }
                    }
                    else if (boss_action == 2) { // AoE 
                        PrintMessage(boss.GetName() + " unleashes a devastating AoE attack!");
                        for (size_t i = 0; i < player_team.size(); ++i) {
                            int damage = GetRandomAttackDamage(boss.GetMinAttack(), boss.GetMaxAttack()) / 2;
                            player_team[i].TakeDamage(damage);
                            PrintMessage(player_team[i].GetName() + " takes " + std::to_string(damage) + " damage.");

                            if (!player_team[i].IsAlive()) {
                                PrintMessage(player_team[i].GetName() + " dies!");
                                player_team.erase(player_team.begin() + i);
                                i--;
                            }
                        }
                    }
                    else if (boss_action == 3) { // Хил
                        int heal_amount = boss.GetHealAmount();
                        boss.Heal(heal_amount);

                        PrintMessage(boss.GetName() + " heals himself for " + std::to_string(heal_amount) + " HP.");
                    }   
                }

                PrintMessage("Press Enter to continue...");
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin.get();
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

    void Battle(vector<Unit>& player_team, vector<Unit> enemy_team, int floor) {
        using namespace MyGame;
        while (!player_team.empty() && !enemy_team.empty()) {
            
            PrintMessage("\n--- Floor " + std::to_string(floor) + " --- ");
            DisplayTeam(player_team, true);
            cout << "\n";
            DisplayTeam(enemy_team, false);
            cout << "\n";

            PrintMessage("--- Your Turn --- ");
            cout << "Choose a unit to act with (1-" << player_team.size() << "): ";
            int player_choice;
            cin >> player_choice;

            while (cin.fail() || player_choice < 1 || player_choice > player_team.size()) {
                PrintMessage("Invalid input. Please enter a number between 1 and " +
                    std::to_string(player_team.size()) + ": ");
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> player_choice;
            }

            if (player_choice > 0 && player_choice <= player_team.size()) {
                int unit_index = player_choice - 1;
                if (player_team[unit_index].GetAttackPattern() == "Heal") {
                    PrintMessage("Healer's actions: ");
                    PrintMessage("1. Heal Ally");
                    PrintMessage("2. Skip Turn");
                    cout << "Enter your choice: ";
                    int healer_choice;
                    cin >> healer_choice;

                    while (cin.fail() || (healer_choice != 1 && healer_choice != 2)) {
                        PrintMessage("Invalid input. Please enter 1 or 2: ");
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        cin >> healer_choice;
                    }
                    if (healer_choice == 1) {
                        cout << "Choose an ally to heal (1-" << player_team.size() << "): ";
                        int target_choice;
                        cin >> target_choice;

                        while (cin.fail() || target_choice < 1 ||
                            target_choice > player_team.size()) {
                            PrintMessage("Invalid input. Please enter a number between 1 and " +
                                std::to_string(player_team.size()) + ": ");
                            cin.clear();
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            cin >> target_choice;
                        }
                        int target_index = target_choice - 1;
                        const int kHealAmountMin = 10;
                        const int kHealAmountMax = 20;
                        int heal_amount = GetRandomNumber(kHealAmountMin, kHealAmountMax);  
                        player_team[target_index].Heal(heal_amount);
                        PrintMessage(player_team[unit_index].GetName() + " heals " +
                            player_team[target_index].GetName() + " for " +
                            std::to_string(heal_amount) + " HP.");

                    }
                    else {
                        PrintMessage(player_team[unit_index].GetName() + " skips turn.");
                    }

                }
                else if (player_team[unit_index].GetAttackPattern() == "Multi") {
                    int damage = GetRandomAttackDamage(player_team[unit_index].GetMinAttack(),
                        player_team[unit_index].GetMaxAttack());
                    cout << player_team[unit_index].GetName()
                        << " attacks all enemies dealing " << damage << " damage.\n";
                    for (size_t i = 0; i < enemy_team.size(); ++i) {
                        enemy_team[i].TakeDamage(damage);
                        if (!enemy_team[i].IsAlive()) {
                            PrintMessage(enemy_team[i].GetName() + " dies!");
                            enemy_team.erase(enemy_team.begin() + i);
                            i--;  
                        }
                    }

                }
                else {
                    cout << "Choose a target to attack (1-" << enemy_team.size() << "): ";
                    int target_choice;
                    cin >> target_choice;

                    while (cin.fail() || target_choice < 1 ||
                        target_choice > enemy_team.size()) {
                        PrintMessage("Invalid input. Please enter a number between 1 and " +
                            std::to_string(enemy_team.size()) + ": ");
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        cin >> target_choice;
                    }

                    int player_damage_dealt = 0;
                    int target_index = target_choice - 1;
                    int damage = GetRandomAttackDamage(
                        player_team[unit_index].GetMinAttack(),
                        player_team[unit_index].GetMaxAttack());
                    player_damage_dealt = damage;
                    PrintMessage(player_team[unit_index].GetName() + " attacks " +
                        enemy_team[target_index].GetName() + " and deals " +
                        std::to_string(damage) + " damage.");
                    enemy_team[target_index].TakeDamage(damage);

                    if (!enemy_team[target_index].IsAlive()) {
                        PrintMessage(enemy_team[target_index].GetName() + " dies!");
                        enemy_team.erase(enemy_team.begin() + target_index);
                    }

                    PrintMessage("Your unit dealt " + std::to_string(player_damage_dealt) +
                        " damage.");
                }
            }

            if (!enemy_team.empty() && !player_team.empty()) {
                
                PrintMessage("\n--- Floor " + std::to_string(floor) + " --- ");
                DisplayTeam(player_team, true);
                cout << "\n";
                DisplayTeam(enemy_team, false);
                cout << "\n";
                PrintMessage("--- Enemy Turn --- ");

                int enemy_damage_dealt = 0;

                int enemy_unit_index = GetRandomNumber(0, enemy_team.size() - 1);
                if (enemy_team[enemy_unit_index].GetAttackPattern() == "Heal") {
                    int target_index = 0;
                    for (size_t i = 1; i < enemy_team.size(); ++i) {
                        if (enemy_team[i].GetHitPoints() <
                            enemy_team[target_index].GetHitPoints()) {
                            target_index = i;
                        }
                    }
                    const int kHealAmountMin = 10;
                    const int kHealAmountMax = 20;
                    int heal_amount =
                        GetRandomNumber(kHealAmountMin, kHealAmountMax);
                    enemy_team[target_index].Heal(heal_amount);
                    PrintMessage(enemy_team[enemy_unit_index].GetName() + " heals " +
                        enemy_team[target_index].GetName() + " for " +
                        std::to_string(heal_amount) + " HP.");

                }
                else if (enemy_team[enemy_unit_index].GetAttackPattern() == "Multi") {
                    int damage = GetRandomAttackDamage(
                        enemy_team[enemy_unit_index].GetMinAttack(),
                        enemy_team[enemy_unit_index].GetMaxAttack());
                    cout << enemy_team[enemy_unit_index].GetName()
                        << " attacks all players dealing " << damage << " damage.\n";
                    for (size_t i = 0; i < player_team.size(); ++i) {
                        player_team[i].TakeDamage(damage);
                        if (!player_team[i].IsAlive()) {
                            PrintMessage(player_team[i].GetName() + " dies!");
                            player_team.erase(player_team.begin() + i);
                            i--;  
                        }
                    }
                }

                else {
                    int player_target_index = GetRandomNumber(0, player_team.size() - 1);
                    int damage = GetRandomAttackDamage(
                        enemy_team[enemy_unit_index].GetMinAttack(),
                        enemy_team[enemy_unit_index].GetMaxAttack());
                    enemy_damage_dealt = damage;
                    PrintMessage(enemy_team[enemy_unit_index].GetName() + " attacks " +
                        player_team[player_target_index].GetName() + " and deals " +
                        std::to_string(damage) + " damage.");
                    player_team[player_target_index].TakeDamage(damage);

                    if (!player_team[player_target_index].IsAlive()) {
                        PrintMessage(player_team[player_target_index].GetName() + " dies!");
                        player_team.erase(player_team.begin() + player_target_index);
                    }
                }

                PrintMessage("Enemy unit dealt " + std::to_string(enemy_damage_dealt) +
                    " damage.");

                cout << "Press Enter to continue...";
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin.get();
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

    void Shop(vector<Unit>& player_team, Inventory& player_inventory, int& gold,
        const vector<Unit>& available_units,
        const vector<Artifact>& available_artifacts) {
        Unit random_unit = available_units[GetRandomNumber(0, available_units.size() - 1)];
        Artifact random_artifact = GetRandomArtifact(available_artifacts);

        while (true) {
            
            PrintMessage("\n--- Welcome to the Shop! --- ");
            PrintMessage("You have " + std::to_string(gold) + " gold.");
            PrintMessage("What would you like to buy?");

            int artifact_price = random_artifact.GetCost();
            int unit_price = random_unit.GetCost();

            cout << "1. " << random_artifact.GetName() << " (" << artifact_price
                << " gold) - " << random_artifact.GetDescription() << "\n";
            cout << "2. Hire " << random_unit.GetName() << " (" << unit_price << " gold)\n";
            cout << "3. Exit\n";

            int input_choice;
            cout << "Enter your choice: ";
            cin >> input_choice;

            while (cin.fail()) {
                PrintMessage("Invalid input. Please enter a number: ");
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> input_choice;
            }

            if (input_choice == 1) {  // Артифакт
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
            else if (input_choice == 2) {  // Нанимаем юнита
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
            else if (input_choice == 3) {  // Выход
                break;  
            }
            else {
                PrintMessage("Invalid choice!");
            }

            PrintMessage("Press Enter to continue shopping...");
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cin.get();
        }
    }

    void HandleTeamManagement(vector<Unit>& player_team) {
        while (true) {
            
            PrintMessage("--- Team Management ---");
            MyGame::DisplayTeam(player_team, true);

            PrintMessage("\nWhat do you want to do?");
            PrintMessage("1. View Unit Details");
            PrintMessage("2. Dismiss Unit");
            PrintMessage("3. Back to Profile");
            cout << "Enter your choice: ";

            int input_choice;  
            cin >> input_choice;

            while (cin.fail()) {
                PrintMessage("Invalid input. Please enter a number: ");
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> input_choice;
            }

            if (input_choice == 1) {
                if (player_team.empty()) {
                    PrintMessage("Your team is empty!");
                }
                else {
                    cout << "Enter the number of the unit to view (1-" << player_team.size()
                        << "): ";
                    int unit_index;
                    cin >> unit_index;

                    while (cin.fail() || unit_index < 1 || unit_index > player_team.size()) {
                        PrintMessage("Invalid input. Please enter a number between 1 and " +
                            std::to_string(player_team.size()) + ": ");
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        cin >> unit_index;
                    }

                    DisplayUnitDetails(player_team[unit_index - 1]);
                }

                PrintMessage("Press Enter to continue...");
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin.get();

            }
            else if (input_choice == 2) {
                // Увольнение юнита
                if (player_team.empty()) {
                    PrintMessage("Your team is empty!");
                }
                else {
                    cout << "Enter the number of the unit to dismiss (1-" << player_team.size()
                        << "): ";
                    int unit_index;
                    cin >> unit_index;

                    while (cin.fail() || unit_index < 1 || unit_index > player_team.size()) {
                        PrintMessage("Invalid input. Please enter a number between 1 and " +
                            std::to_string(player_team.size()) + ": ");
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        cin >> unit_index;
                    }

                    cout << "Are you sure you want to dismiss " << player_team[unit_index - 1].GetName()
                        << "? (y/n): ";
                    char confirm;
                    cin >> confirm;

                    if (confirm == 'y' || confirm == 'Y') {
                        PrintMessage(player_team[unit_index - 1].GetName() + " has been dismissed.");
                        player_team.erase(player_team.begin() + unit_index - 1);
                    }
                    else {
                        PrintMessage("Dismissal cancelled.");
                    }
                }

                PrintMessage("Press Enter to continue...");
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin.get();

            }
            else if (input_choice == 3) {
                break;  // Возврат в профиль
            }
            else {
                PrintMessage("Invalid choice. Please try again.");
                PrintMessage("Press Enter to continue...");
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin.get();
            }
        }
    }

    void DisplayPlayerProfile(vector<Unit>& player_team, Inventory& player_inventory,
        int gold) {
        while (true) {
            
            PrintMessage("--- Player Profile ---");
            PrintMessage("Gold: " + std::to_string(gold));

            cout << "\n";

            PrintMessage("--- Team ---");
            MyGame::DisplayTeam(player_team, true);
            cout << "\n";

            PrintMessage("--- Inventory ---");
            if (player_inventory.GetArtifacts().empty()) {
                PrintMessage("Inventory is empty.");
            }
            else {
                const auto& artifacts = player_inventory.GetArtifacts();
                for (size_t i = 0; i < artifacts.size(); ++i) {
                    cout << setw(2) << i + 1 << ". " << artifacts[i].GetName() << " - "
                        << artifacts[i].GetDescription() << "\n";
                }
            }

            cout << "\nWhat do you want to do?\n";
            PrintMessage("1. Manage Team");
            PrintMessage("2. Back to Main Menu");
            cout << "Enter your choice: ";

            int input_choice; 

            cin >> input_choice;

            while (cin.fail()) {
                PrintMessage("Invalid input. Please enter a number: ");
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> input_choice;
            }

            if (input_choice == 1) {
                HandleTeamManagement(player_team);
            }
            else if (input_choice == 2) {
                break;
            }
            else {
                PrintMessage("Invalid choice. Please try again.");
                PrintMessage("Press Enter to continue...");
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin.get();
            }
        }
    }

    void HandleMainMenu(vector<Unit>& player_team, Inventory& player_inventory, int& gold,
        int& floor, const vector<Unit>& available_units,
        const vector<Artifact>& available_artifacts) {
        while (true) {
            

            PrintMessage("\n--- Floor " + std::to_string(floor) + " --- ");
            PrintMessage("What do you do?");
            PrintMessage("1. Go to the next floor");
            PrintMessage("2. View Profile");
            cout << "Enter your choice: ";

            int input_choice;

            cin >> input_choice;

            while (cin.fail()) {
                PrintMessage("Invalid input. Please enter a number: ");
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> input_choice;
            }

            if (input_choice == 1) {
                floor++;  // Переход на следующий этаж
                break;    // Выход из цикла меню
            }
            else if (input_choice == 2) {
                DisplayPlayerProfile(player_team, player_inventory, gold);
            }
            else {
                PrintMessage("Invalid choice. Please try again.");
                PrintMessage("Press Enter to continue...");
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin.get();
            }
        }
    }

    void InitializeGame(std::vector<MyGame::Unit>& player_team,
        MyGame::Inventory& player_inventory, int& gold,
        std::vector<MyGame::Unit>& available_units,
        std::vector<MyGame::Artifact>& available_artifacts,
        MyGame::Boss& final_boss) {
        using namespace MyGame;
        srand(time(0));

        // Load units from file
        available_units = MyGame::LoadUnitsFromFile("units.txt");
        if (available_units.empty()) {
            std::cerr << "Failed to load units. Exiting.\n";
            exit(1); // Exit function
        }

        // Load artifacts from file
        available_artifacts = MyGame::LoadArtifactsFromFile("artifacts.txt");
        if (available_artifacts.empty()) {
            std::cerr << "Failed to load artifacts. Exiting.\n";
            exit(1); // Exit function
        }
        // Load boss from file
        final_boss = MyGame::LoadBossFromFile("boss.txt");

        if (available_units.size() >= 2) {
            player_team.push_back(available_units[0]);  // Warrior
            player_team.push_back(available_units[1]);  // Mage
        }
        else {
            PrintMessage("Not enough units loaded\n");
        }

        MyGame::ApplyArtifactEffects(player_team, player_inventory);

        gold = 100;  // Initial gold
    }

    void HandleFloorEvent(std::vector<MyGame::Unit>& player_team,
        MyGame::Inventory& player_inventory, int& gold, int floor,
        const std::vector<MyGame::Unit>& available_units,
        const std::vector<MyGame::Artifact>& available_artifacts) {
        using namespace MyGame;

        const int kMaxEvents = 4;
        int event = MyGame::GetRandomNumber(1, kMaxEvents);

        if (event == 1) {  // Бой
            MyGame::PrintMessage("You have been attacked!");
            MyGame::ApplyArtifactEffects(player_team, player_inventory);
            std::vector<MyGame::Unit> enemy_team =
                MyGame::GenerateRandomEnemy(available_units);
            MyGame::Battle(player_team, enemy_team, floor);
            if (player_team.empty()) return;

            const int kBattleReward = 20;
            gold += kBattleReward;  // Награда за победу в бою
            MyGame::PrintMessage("You earned 20 gold!");

        }
        else if (event == 2) {  // Сокровищница
            MyGame::PrintMessage("You found treasure!");
            if (MyGame::GetRandomNumber(1, 2) == 1) {
                MyGame::Artifact new_artifact =
                    MyGame::GetRandomArtifact(available_artifacts);
                if (player_inventory.AddItem(new_artifact)) {
                    MyGame::PrintMessage("You found: " + new_artifact.GetName() + "!");
                }
                else {
                    MyGame::PrintMessage("Your inventory is full!");
                }
            }
            const int kTreasureReward = 10;
            gold += kTreasureReward;
            MyGame::PrintMessage("You found 10 gold!");

        }
        else if (event == 3) {  // Магазин
            MyGame::PrintMessage("You found a shop!");
            MyGame::Shop(player_team, player_inventory, gold, available_units,
                available_artifacts);
        }
        else {  // Пустая комната
            MyGame::PrintMessage("You found an empty room.");
        }
    }

    void RunGameLoop(std::vector<MyGame::Unit>& player_team,
        MyGame::Inventory& player_inventory, int& gold, int& floor,
        const std::vector<MyGame::Unit>& available_units,
        const std::vector<MyGame::Artifact>& available_artifacts,
        MyGame::Boss& final_boss) {
        using namespace MyGame;
        while (true) {
            // Вызываем основное меню, чтобы получить выбор игрока
            HandleMainMenu(player_team, player_inventory, gold, floor, available_units,
                available_artifacts);

            // Дальше происходит случайное событие при переходе на новый этаж
            PrintMessage("\n--- Floor " + std::to_string(floor) + " ---");

            if (floor > 10) {
                PrintMessage("You reached the top of the tower! Prepare for the "
                    "final battle!");
                Battle(player_team, final_boss, floor);  // Запускаем битву с боссом
                break;
            }
            else {
                HandleFloorEvent(player_team, player_inventory, gold, floor, available_units, available_artifacts);
            }

            PrintMessage("Press Enter to continue...");
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
    }
} 