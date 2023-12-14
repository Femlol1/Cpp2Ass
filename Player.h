#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <unordered_set>
#include "json.hpp"

using json = nlohmann::json;

class Player {
public:
    Player(json& jsonData);

    void grab(const std::string& item, const json& gameData);
    void look(const std::string& item, const json& gameData);
    void move(const std::string& direction, const json& j);
    void kill(const std::string& enemy, const json& gameData);
    void unknownCommand(const std::string& command);
    void die();
    const std::unordered_set<std::string>& getDeadEnemies() const {
        return deadEnemies;
    }

    const std::unordered_set<std::string>& getRequiredKills() const {
        return requiredKills;
    }

    const std::vector<std::string>& getInventory() const {
        return inventory;
    }

    // Function to add an item to the player's inventory
    void addToInventory(const std::string& item) {
        inventory.push_back(item);
    }

    void printRoomAndItems();
    void noExitGameOver();

    // Win condition storage
    std::unordered_set<std::string> requiredKills;
    std::unordered_set<std::string> requiredItems;
    std::unordered_map<std::string, std::vector<std::string>> collectedItems;
    std::vector<std::string> inventory;

    std::string curRoom;
private:
    json& gameData;
    std::unordered_set<std::string> deadEnemies;


    void mapObjective();
};

#endif // PLAYER_H
