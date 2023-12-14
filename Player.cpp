#include "Player.h"
#include <iostream>
#include <vector>

using  namespace std;

Player::Player(json& jsonData) : gameData(jsonData) {
    // Initialize player with data from JSON
    curRoom = gameData["player"]["initialroom"].get<std::string>();
}

void Player::grab(const std::string& item, const json& gameData) {
    const auto& objects = gameData.at("objects");

    auto inInventory = std::find(inventory.begin(), inventory.end(), item);

    if (inInventory != inventory.end()) {
        cout << "You already have the " << item << " in your inventory." << endl;
    } else {
    auto foundObject = std::find_if(objects.begin(), objects.end(), [&](const auto& obj) {
        return obj.at("id") == item && obj.at("initialroom") == curRoom;
    });
    

    if (foundObject != objects.end()) {
        // Item found in the current room, add it to the inventory
        inventory.push_back(item);
        cout << "You grabbed " << item << endl;
    } else {
        cout << "The item "<< item <<" is not in the current room or doesn't exist." << endl;
    }
}
}


void Player::look(const std::string& item, const json& gameData) {

    const auto& objects = gameData.at("objects");

    // Check if the item is in the player's current room
    auto foundObjectInRoom = std::find_if(gameData["objects"].begin(), gameData["objects"].end(), [&](const auto& obj) {
        return obj.at("id") == item && obj.at("initialroom") == curRoom;
    });

    // Check if the item is in the player's inventory
    auto foundObjectInInventory = std::find(inventory.begin(), inventory.end(), item);

    // Check if the enemy is in the player's current room
    auto foundEnemy = std::find_if(gameData["enemies"].begin(), gameData["enemies"].end(), [&](const auto& enemy) {
        return enemy.at("id") == item && enemy.at("initialroom") == curRoom;
    });

     if (foundObjectInRoom != gameData["objects"].end()) {
        cout << foundObjectInRoom->at("desc").get<std::string>() << endl;
    } else if (foundObjectInInventory != inventory.end()) {
        cout << foundObjectInRoom->at("desc").get<std::string>() << endl;
    } else if (foundEnemy != gameData["enemies"].end()) {
        if (deadEnemies.find(foundEnemy->at("id")) != deadEnemies.end()) {
            cout << "Oh look, a dead " << foundEnemy->at("id").get<std::string>() << "." << endl;
        }
        else {
            cout << foundEnemy->at("desc").get<std::string>() << endl;
        }
    } else {
         printRoomAndItems();
        //cout << "There is nothing in the room" << endl;
    }
}



//void Player::look(const std::string& item, const json& roomData) {
//    for (const auto& obj : j["objects"]) {
//        if (obj["initialroom"] == curRoom) {
//            cout << "There is a "<<obj["id"] <<", "<< obj["desc"] << endl;
//        }
//    }
//
//    // Display enemies in the room
//    for (const auto& enemy : j["enemies"]) {
//        if (enemy["initialroom"] == player.curRoom) {
//            cout << "There is a "<<enemy["id"] <<", "<< enemy["desc"] << endl;
//        }
//    }
//}

void Player::move(const std::string& direction, const json& j) {

     // Check if there's an enemy in the current room
    auto foundEnemy = std::find_if(j["enemies"].begin(), j["enemies"].end(),
                                    [&](const auto& enemy) {
                                        return enemy.at("initialroom") == curRoom &&
                                               deadEnemies.find(enemy.at("id")) == deadEnemies.end();
                                    });

    // If an enemy is present, check for aggressiveness
    if (foundEnemy != j["enemies"].end() && foundEnemy->at("aggressiveness").get<int>() > 0 && rand() % 100 < foundEnemy->at("aggressiveness").get<int>()) {
        cout << "The " << foundEnemy->at("id").get<std::string>() << " attacked you while moving!" << endl;
        die();
        return;
    }

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
            //cout << "You moved to " << this->curRoom << endl;

            printRoomAndItems();
        } else {
            cout << "You can't move in that direction" << endl;
        }
    } else {

        cout << "You are in an unknown location." << endl;
    }
}

void Player::kill(const std::string& enemy, const json& gameData) {
    auto& enemies = gameData["enemies"];
    auto foundEnemy = std::find_if(enemies.begin(), enemies.end(), [&](const auto& e) {
        return e.at("id") == enemy && e.at("initialroom") == curRoom && deadEnemies.find(enemy) == deadEnemies.end();
    });

    if (foundEnemy != enemies.end()) {
        auto requiredItems = foundEnemy->value("killedby", json::array());

        if (requiredItems.empty() || std::all_of(requiredItems.begin(), requiredItems.end(), [this](const auto& item) {
            return std::find(inventory.begin(), inventory.end(), item) != inventory.end();
                                                   })) {
            cout << "You ";
            if (!requiredItems.empty()) {
                cout << "used ";
                for (const auto& item : requiredItems) {
                    cout << item.get<std::string>() << " ";
                }
                cout << "to kill the " << enemy << "." << endl;

            }else {
                cout << "killed the " << enemy << "." << endl;
                deadEnemies.insert(enemy); // Mark the enemy as dead in the set
            }
        } else {
            cout << "You don't have the required item" << endl;
            die(); // Player dies if they don't have the required item
        }
    } else {
        cout << "There is no " << enemy << " here to kill or it's already dead." << endl;
    }
}

//wanted to create a finction that allows the player to ask what the objective
void Player::mapObjective(){

}

void Player::printRoomAndItems() {
    auto roomIt = find_if(gameData["rooms"].begin(), gameData["rooms"].end(),
                          [this](const json& room) { return room["id"] == this->curRoom; });

    if (roomIt != gameData["rooms"].end()) {
        cout << roomIt->at("desc").get<std::string>() << endl;
        for (const auto& obj : gameData["objects"]) {
            if (obj["initialroom"] == curRoom) {
                cout << "There is a " << obj["id"].get<std::string>() << endl;
            }
        }

        for (const auto& enemy : gameData["enemies"]) {
        if (enemy["initialroom"] == curRoom) {
             // Check if the enemy is in the deadEnemies set
            if (deadEnemies.find(enemy["id"]) == deadEnemies.end()) {
                cout << "There is a " << enemy["id"].get<std::string>()  << endl;
            } else {
                cout << "There is a dead " << enemy["id"].get<std::string>()  << endl;
            }
    }
}
    }
}



// Allows the player to die internally, separate to the exit command
void Player::die() {
    cout << "You died, game over." << endl;
    exit(1);
}

//in case player gets stuck in a room, end the game
void Player::noExitGameOver() {
    cout << "You are stuck, game over." << endl;
    exit(1);
}


void Player::unknownCommand(const std::string& command) {
    cout << "I don't understand " << command << endl;
}



// there seem to be p