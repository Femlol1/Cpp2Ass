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

    void collectItem(const std::string& item) {
        collectedItems.insert(item);
    }

    void printRoomAndItems();

    // Win condition storage
    std::unordered_set<std::string> requiredKills;
    std::unordered_set<std::string> requiredItems;
    std::unordered_set<std::string> collectedItems;

    std::string curRoom;
private:
    std::vector<std::string> inventory;
    json& gameData;
    std::unordered_set<std::string> deadEnemies;


};

#endif // PLAYER_H
