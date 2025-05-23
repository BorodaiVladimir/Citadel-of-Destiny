#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>
#include <limits>

struct Unit {
    std::string name;
    int hp;
    int maxHp;
    int attack;
    int defense;
    std::string attack_type;
    std::string defense_type;
    std::string skill_name;
    std::string skill_description;
};

struct Artifact {
    std::string name;
    std::string description;
    std::string effect_type;  
    float effect_value;
};

const std::string RED_COLOR = "\033[31m";
const std::string RESET_COLOR = "\033[0m";

const int INVENTORY_SIZE = 5;

struct Inventory {
    std::vector<Artifact> artifacts;

    bool addItem(const Artifact& artifact) {
        if (artifacts.size() < INVENTORY_SIZE) {
            artifacts.push_back(artifact);
            return true;
        }
        return false;
    }

    void removeItem(int index) {
        if (index >= 0 && index < artifacts.size()) {
            artifacts.erase(artifacts.begin() + index);
        }
    }
};

struct Boss : public Unit {
    int heal_amount;    
    int block_amount;   

    Boss(std::string name, int hp, int maxHp, int attack, int defense,
        std::string attack_type, std::string defense_type,
        std::string skill_name, std::string skill_description,
        int healAmount, int blockAmount)
        : Unit{ name, hp, maxHp, attack, defense, attack_type, defense_type, skill_name, skill_description },
        heal_amount(healAmount), block_amount(blockAmount) {}

    int chooseAction(); 
};


int getRandomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}

int Boss::chooseAction() {
    int choice = getRandomNumber(1, 4);
    return choice;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

Unit createWarrior() {
    return { "Warrior", 50, 50, 10, 5, "Physical", "Armor", "Charge", "Deals bonus damage on attack" };
}

Unit createMage() {
    return { "Mage", 30, 30, 15, 2, "Magical", "Magic Resist", "Fireball", "Deals damage to multiple enemies" };
}

Unit createHealer() {
    return { "Healer", 40, 40, 5, 3, "Magical", "Magic Resist", "Heal", "Restores HP to an ally" };
}

std::vector<Unit> generateRandomEnemy() {
    std::vector<Unit> enemyTeam;
    int numEnemies = getRandomNumber(1, 3);
    for (int i = 0; i < numEnemies; ++i) {
        Unit goblin = { "Goblin", 30, 30, 8, 2, "Physical", "Armor", "Strike", "Simple attack" };
        enemyTeam.push_back(goblin);
    }
    return enemyTeam;
}

void drawHealthBar(int currentHp, int maxHp, int barLength = 10) {
    float healthRatio = (float)currentHp / maxHp;
    int filledBars = (int)(healthRatio * barLength);
    int emptyBars = barLength - filledBars;

    std::cout << "[";
    for (int i = 0; i < filledBars; ++i) {
        std::cout << RED_COLOR << "#" << RESET_COLOR; 
    }
    for (int i = 0; i < emptyBars; ++i) {
        std::cout << " ";
    }
    std::cout << "] (" << RED_COLOR << currentHp << "/" << maxHp << RESET_COLOR << ")"; 
}

void displayTeam(const std::vector<Unit>& team, bool isPlayer = true) {
    std::cout << (isPlayer ? "Your Team:\n" : "Enemy Team:\n");
    for (size_t i = 0; i < team.size(); ++i) {
        std::cout << std::setw(2) << i + 1 << ". " << std::setw(10) << team[i].name << " ";
        drawHealthBar(team[i].hp, team[i].maxHp);
        std::cout << "\n";
    }
}

void applyArtifactEffects(std::vector<Unit>& team, const Inventory& inventory) {
    for (const auto& artifact : inventory.artifacts) {
        if (artifact.effect_type == "HP_BOOST") {
            for (auto& unit : team) {
                unit.maxHp = static_cast<int>(unit.maxHp * (1.0f + artifact.effect_value));
                unit.hp = static_cast<int>(unit.hp * (1.0f + artifact.effect_value));
                if (unit.hp > unit.maxHp) {
                    unit.hp = unit.maxHp;
                }
            }
        }
    }
}

Artifact getRandomArtifact() {
    Artifact ancientCuirass = { "Ancient Cuirass", "Increases max HP of all units.", "HP_BOOST", 0.10f };
    return ancientCuirass;
}

void battle(std::vector<Unit>& playerTeam, Boss& boss, int floor) { 
    while (!playerTeam.empty() && boss.hp > 0) { 

        clearScreen();
        std::cout << "\n--- Floor " << floor << " - Final Battle! --- \n";
        displayTeam(playerTeam, true);
        std::cout << "\n";
        std::cout << "Boss: " << boss.name << " ";
        drawHealthBar(boss.hp, boss.maxHp);
        std::cout << "\n\n";

        // --- Your Turn ---
        std::cout << "--- Your Turn --- \n";
        std::cout << "Choose a unit to attack with (1-" << playerTeam.size() << ", 0 to skip): ";
        int playerChoice;
        std::cin >> playerChoice;

        while (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> playerChoice;
        }

        int playerDamageDealt = 0;
        if (playerChoice > 0 && playerChoice <= playerTeam.size()) {
            int unitIndex = playerChoice - 1;

            int damage = playerTeam[unitIndex].attack - boss.defense;
            if (damage < 0) damage = 0;
            playerDamageDealt = damage;

            std::cout << playerTeam[unitIndex].name << " attacks " << boss.name << " and deals " << damage << " damage.\n";
            boss.hp -= damage;

            if (boss.hp <= 0) {
                std::cout << boss.name << " is defeated!\n";
                break;  
            }
        }
        else if (playerChoice != 0) {
            std::cout << "Invalid unit selection.\n";
        }

        std::cout << "Your unit dealt " << playerDamageDealt << " damage.\n"; 

        if (boss.hp > 0 && !playerTeam.empty()) {
            clearScreen();
            std::cout << "\n--- Floor " << floor << " - Final Battle! --- \n";
            displayTeam(playerTeam, true);
            std::cout << "\n";
            std::cout << "Boss: " << boss.name << " ";
            drawHealthBar(boss.hp, boss.maxHp);
            std::cout << "\n\n";
            std::cout << "--- Boss Turn --- \n";

            int bossAction = boss.chooseAction();

            if (bossAction == 1) { 
                int playerTargetIndex = getRandomNumber(0, playerTeam.size() - 1);
                int damage = boss.attack - playerTeam[playerTargetIndex].defense;
                if (damage < 0) damage = 0;

                std::cout << boss.name << " attacks " << playerTeam[playerTargetIndex].name << " and deals " << damage << " damage.\n";
                playerTeam[playerTargetIndex].hp -= damage;

                if (playerTeam[playerTargetIndex].hp <= 0) {
                    std::cout << playerTeam[playerTargetIndex].name << " dies!\n";
                    playerTeam.erase(playerTeam.begin() + playerTargetIndex);
                }
            }
            else if (bossAction == 2) { 
                std::cout << boss.name << " unleashes a devastating AoE attack!\n";
                for (size_t i = 0; i < playerTeam.size(); ++i) {
                    int damage = (boss.attack / 2) - playerTeam[i].defense;  
                    if (damage < 0) damage = 0;
                    playerTeam[i].hp -= damage;
                    std::cout << playerTeam[i].name << " takes " << damage << " damage.\n";

                    if (playerTeam[i].hp <= 0) {
                        std::cout << playerTeam[i].name << " dies!\n";
                        playerTeam.erase(playerTeam.begin() + i);
                        i--;
                    }
                }
            }
            else if (bossAction == 3) { 
                int healAmount = boss.heal_amount;
                boss.hp += healAmount;
                if (boss.hp > boss.maxHp) boss.hp = boss.maxHp;

                std::cout << boss.name << " heals himself for " << healAmount << " HP.\n";
            }
            else if (bossAction == 4) {
                std::cout << boss.name << " braces for impact, blocking incoming damage!\n";
            }

            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }

        if (playerTeam.empty()) {
            std::cout << "\nYou lost!\n";
            return;
        }
        if (boss.hp <= 0) {
            std::cout << "\nYou won!\n";
            return;
        }
    }
}

void battle(std::vector<Unit>& playerTeam, std::vector<Unit> enemyTeam, int floor) {
    while (!playerTeam.empty() && !enemyTeam.empty()) {
        clearScreen();
        std::cout << "\n--- Floor " << floor << " --- \n";
        displayTeam(playerTeam, true);
        std::cout << "\n";
        displayTeam(enemyTeam, false);
        std::cout << "\n";

        std::cout << "--- Your Turn --- \n";
        std::cout << "Choose a unit to attack with (1-" << playerTeam.size() << ", 0 to skip): ";
        int playerChoice;
        std::cin >> playerChoice;

        while (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> playerChoice;
        }

        if (playerChoice > 0 && playerChoice <= playerTeam.size()) {
            int unitIndex = playerChoice - 1;

            std::cout << "Choose a target to attack (1-" << enemyTeam.size() << "): ";
            int targetChoice;
            std::cin >> targetChoice;

            while (std::cin.fail()) {
                std::cout << "Invalid input. Please enter a number: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> targetChoice;
            }

            int playerDamageDealt = 0;

            if (targetChoice > 0 && targetChoice <= enemyTeam.size()) {
                int targetIndex = targetChoice - 1;

                int damage = playerTeam[unitIndex].attack - enemyTeam[targetIndex].defense;
                if (damage < 0) damage = 0;
                playerDamageDealt = damage;

                std::cout << playerTeam[unitIndex].name << " attacks " << enemyTeam[targetIndex].name << " and deals " << damage << " damage.\n";
                enemyTeam[targetIndex].hp -= damage;

                if (enemyTeam[targetIndex].hp <= 0) {
                    std::cout << enemyTeam[targetIndex].name << " dies!\n";
                    enemyTeam.erase(enemyTeam.begin() + targetIndex);
                }
            }
            else {
                std::cout << "Invalid target selection.\n";
            }

            std::cout << "Your unit dealt " << playerDamageDealt << " damage.\n";

        }
        else if (playerChoice != 0) {
            std::cout << "Invalid unit selection.\n";
        }

        if (!enemyTeam.empty() && !playerTeam.empty()) {
            clearScreen();
            std::cout << "\n--- Floor " << floor << " --- \n";
            displayTeam(playerTeam, true);
            std::cout << "\n";
            displayTeam(enemyTeam, false);
            std::cout << "\n";
            std::cout << "--- Enemy Turn --- \n";

            int enemyDamageDealt = 0;

            int enemyUnitIndex = getRandomNumber(0, enemyTeam.size() - 1);
            int playerTargetIndex = getRandomNumber(0, playerTeam.size() - 1);

            int damage = enemyTeam[enemyUnitIndex].attack - playerTeam[playerTargetIndex].defense;
            if (damage < 0) damage = 0;
            enemyDamageDealt = damage;

            std::cout << enemyTeam[enemyUnitIndex].name << " attacks " << playerTeam[playerTargetIndex].name << " and deals " << damage << " damage.\n";
            playerTeam[playerTargetIndex].hp -= damage;

            if (playerTeam[playerTargetIndex].hp <= 0) {
                std::cout << playerTeam[playerTargetIndex].name << " dies!\n";
                playerTeam.erase(playerTeam.begin() + playerTargetIndex);
            }

            std::cout << "Enemy unit dealt " << enemyDamageDealt << " damage.\n";

            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }

        if (playerTeam.empty()) {
            std::cout << "\nYou lost!\n";
            return;
        }
        if (enemyTeam.empty()) {
            std::cout << "\nYou won!\n";
            return;
        }
    }
}

void shop(std::vector<Unit>& playerTeam, Inventory& playerInventory, int& gold) {
    while (true) {
        clearScreen();
        std::cout << "\n--- Welcome to the Shop! --- \n";
        std::cout << "You have " << gold << " gold.\n";
        std::cout << "What would you like to buy?\n";

        std::cout << "1. Artifact (50 gold)\n";
        std::cout << "2. Hire Warrior (75 gold)\n";
        std::cout << "3. Hire Mage (75 gold)\n";
        std::cout << "4. Exit\n";

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        while (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> choice;
        }

        if (choice == 1) {
            if (gold >= 50) {
                Artifact newArtifact = getRandomArtifact();
                if (playerInventory.addItem(newArtifact)) {
                    std::cout << "You bought: " << newArtifact.name << "!\n";
                    gold -= 50;
                }
                else {
                    std::cout << "Your inventory is full!\n";
                }
            }
            else {
                std::cout << "Not enough gold!\n";
            }
        }
        else if (choice == 2) {
            if (gold >= 75 && playerTeam.size() < 3) {
                playerTeam.push_back(createWarrior());
                std::cout << "You hired a Warrior!\n";
                gold -= 75;
            }
            else if (playerTeam.size() >= 3) {
                std::cout << "Your team is full!\n";
            }
            else {
                std::cout << "Not enough gold!\n";
            }
        }
        else if (choice == 3) {
            if (gold >= 75 && playerTeam.size() < 3) {
                playerTeam.push_back(createMage());
                std::cout << "You hired a Mage!\n";
                gold -= 75;
            }
            else if (playerTeam.size() >= 3) {
                std::cout << "Your team is full!\n";
            }
            else {
                std::cout << "Not enough gold!\n";
            }
        }
        else if (choice == 4) {
            break;
        }
        else {
            std::cout << "Invalid choice!\n";
        }

        std::cout << "Press Enter to continue shopping...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
}

void displayUnitDetails(const Unit& unit) {
    std::cout << "--- Unit Details --- \n";
    std::cout << "Name: " << unit.name << "\n";
    std::cout << "HP: " << unit.hp << "/" << unit.maxHp << "\n";
    std::cout << "Attack: " << unit.attack << "\n";
    std::cout << "Defense: " << unit.defense << "\n";
    std::cout << "Attack Type: " << unit.attack_type << "\n";
    std::cout << "Defense Type: " << unit.defense_type << "\n";
    std::cout << "Skill: " << unit.skill_name << " - " << unit.skill_description << "\n";
}

void handleTeamManagement(std::vector<Unit>& playerTeam) {
    while (true) {
        clearScreen();
        std::cout << "--- Team Management ---\n";
        displayTeam(playerTeam, true);

        std::cout << "\nWhat do you want to do?\n";
        std::cout << "1. View Unit Details\n";
        std::cout << "2. Dismiss Unit\n";
        std::cout << "3. Back to Profile\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        while (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> choice;
        }

        if (choice == 1) {
            if (playerTeam.empty()) {
                std::cout << "Your team is empty!\n";
            }
            else {
                std::cout << "Enter the number of the unit to view (1-" << playerTeam.size() << "): ";
                int unitIndex;
                std::cin >> unitIndex;

                while (std::cin.fail() || unitIndex < 1 || unitIndex > playerTeam.size()) {
                    std::cout << "Invalid input. Please enter a number between 1 and " << playerTeam.size() << ": ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin >> unitIndex;
                }

                displayUnitDetails(playerTeam[unitIndex - 1]);
            }

            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();

        }
        else if (choice == 2) {
            if (playerTeam.empty()) {
                std::cout << "Your team is empty!\n";
            }
            else {
                std::cout << "Enter the number of the unit to dismiss (1-" << playerTeam.size() << "): ";
                int unitIndex;
                std::cin >> unitIndex;

                while (std::cin.fail() || unitIndex < 1 || unitIndex > playerTeam.size()) {
                    std::cout << "Invalid input. Please enter a number between 1 and " << playerTeam.size() << ": ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin >> unitIndex;
                }

                std::cout << "Are you sure you want to dismiss " << playerTeam[unitIndex - 1].name << "? (y/n): ";
                char confirm;
                std::cin >> confirm;

                if (confirm == 'y' || confirm == 'Y') {
                    std::cout << playerTeam[unitIndex - 1].name << " has been dismissed.\n";
                    playerTeam.erase(playerTeam.begin() + unitIndex - 1);
                }
                else {
                    std::cout << "Dismissal cancelled.\n";
                }
            }

            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();

        }
        else if (choice == 3) {
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again.\n";
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
    }
}

void displayPlayerProfile(std::vector<Unit>& playerTeam, Inventory& playerInventory, int gold) {
    while (true) {
        clearScreen();
        std::cout << "--- Player Profile ---\n";
        std::cout << "Gold: " << gold << "\n\n";

        std::cout << "--- Team ---\n";
        displayTeam(playerTeam, true);
        std::cout << "\n";

        std::cout << "--- Inventory ---\n";
        if (playerInventory.artifacts.empty()) {
            std::cout << "Inventory is empty.\n";
        }
        else {
            for (size_t i = 0; i < playerInventory.artifacts.size(); ++i) {
                std::cout << std::setw(2) << i + 1 << ". " << playerInventory.artifacts[i].name
                    << " - " << playerInventory.artifacts[i].description << "\n";
            }
        }

        std::cout << "\nWhat do you want to do?\n";
        std::cout << "1. Manage Team\n";
        std::cout << "2. Back to Main Menu\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        while (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> choice;
        }

        if (choice == 1) {
            handleTeamManagement(playerTeam);
        }
        else if (choice == 2) {
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again.\n";
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
    }
}

void handleMainMenu(std::vector<Unit>& playerTeam, Inventory& playerInventory, int& gold, int& floor) {
    while (true) {
        clearScreen();

        std::cout << "\n--- Floor " << floor << " --- \n";
        std::cout << "What do you do?\n";
        std::cout << "1. Go to the next floor\n";
        std::cout << "2. View Profile\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        while (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> choice;
        }

        if (choice == 1) {
            floor++;
            break;
        }
        else if (choice == 2) {
            displayPlayerProfile(playerTeam, playerInventory, gold);
        }
        else {
            std::cout << "Invalid choice. Please try again.\n";
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
    }
}

int main() {
    srand(time(0));

    std::vector<Unit> playerTeam;
    playerTeam.push_back(createWarrior());
    playerTeam.push_back(createMage());

    Inventory playerInventory;

    int floor = 1;
    int gold = 100;

    Boss finalBoss = { "The Destroyer", 150, 150, 20, 10, "Physical", "Armor", "Obliterate", "Destroys everything!", 30, 8 };

    while (true) {
        handleMainMenu(playerTeam, playerInventory, gold, floor);
        clearScreen();
        std::cout << "\n--- Floor " << floor << " ---\n";

        if (floor > 10) {
            std::cout << "You reached the top of the tower! Prepare for the final battle!\n";
            battle(playerTeam, finalBoss, floor);
            break;
        }
        else {
            int event = getRandomNumber(1, 4);

            if (event == 1) {
                std::cout << "You have been attacked!\n";
                applyArtifactEffects(playerTeam, playerInventory);
                std::vector<Unit> enemyTeam = generateRandomEnemy();
                battle(playerTeam, enemyTeam, floor);
                if (playerTeam.empty()) break;

                gold += 20;
                std::cout << "You earned 20 gold!\n";

            }
            else if (event == 2) {
                std::cout << "You found treasure!\n";
                if (getRandomNumber(1, 2) == 1) {
                    Artifact newArtifact = getRandomArtifact();
                    if (playerInventory.addItem(newArtifact)) {
                        std::cout << "You found: " << newArtifact.name << "!\n";
                    }
                    else {
                        std::cout << "Your inventory is full!\n";
                    }
                }
                gold += 10;
                std::cout << "You found 10 gold!\n";

            }
            else if (event == 3) {
                std::cout << "You found a shop!\n";
                shop(playerTeam, playerInventory, gold);
            }
            else {
                std::cout << "You found an empty room.\n";
            }
        }

        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    std::cout << "Game Over!\n";
    return 0;
}