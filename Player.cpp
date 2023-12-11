#include "Player.h"
#include "iostream"

using  namespace std;

Player::Player(json& jsonData) : gameData(jsonData) {
    // Initialize player with data from JSON
    curRoom = gameData["player"]["initialroom"].get<std::string>();
}

void Player::grab(const std::string& item, const json& gameData) {
    const auto& objects = gameData.at("objects");

    auto foundObject = std::find_if(objects.begin(), objects.end(), [&](const auto& obj) {
        return obj.at("id") == item && obj.at("initialroom") == curRoom;
    });

    if (foundObject != objects.end()) {
        // Item found in the current room, add it to the inventory
        inventory.push_back(item);
        cout << "You grabbed: " << item << endl;
    } else {
        cout << "The item is not in the current room or doesn't exist." << endl;
    }
}

void Player::look(const std::string& item, const json& roomData) {
    cout << "You look at: " << item << endl;
}

void Player::move(const std::string& direction, const json& j) {
    // Find the current room in the JSON data
    auto roomIt = find_if(j["rooms"].begin(), j["rooms"].end(),
                          [this](const json& room) { return room["id"] == this->curRoom; });

    if (roomIt != j["rooms"].end()) {
        // Check if the requested direction is a valid exit
        auto exits = roomIt->at("exits");
        auto exitIt = exits.find(direction);

        if (exitIt != exits.end()) {
            // Valid exit, update the current room
            this->curRoom = exitIt.value().get<string>();
            cout << "You moved to " << this->curRoom << endl;
        } else {
            cout << "You can't move in that direction" << endl;
        }
    } else {
        cout << "You are in an unknown location." << endl;
    }
}

void Player::kill(const std::string& enemy, const json& gameData) {
    const auto& enemies = gameData.at("enemies");
    auto foundEnemy = std::find_if(enemies.begin(), enemies.end(), [&](const auto& e) {
        return e.at("id") == enemy && e.at("initialroom") == curRoom;
    });

    if (foundEnemy != enemies.end()) {
        // Check if the player has the required item to kill the enemy
        auto requiredItem = foundEnemy->at("killedby")[0].get<string>();
        if (std::find(inventory.begin(), inventory.end(), requiredItem) != inventory.end()) {
            cout << "You used the " << requiredItem << " to kill the " << enemy << "." << endl;

            // Logic to remove the enemy from the game
        } else {
            cout << "You don't have the required "<< requiredItem<<" to kill the " << enemy << ". Game Over." << endl;

        }
    } else {
        cout << "There is no " << enemy << " here to kill." << endl;

    }
}

void Player::unknownCommand(const std::string& command) {
    cout << "Unknown command: " << command << endl;
}

