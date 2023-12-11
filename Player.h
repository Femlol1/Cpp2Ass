#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

class Player {
public:
    Player(json& jsonData);

    void grab(const std::string& item, const json& gameData);
    void look(const std::string& item, const json& roomData);
    void move(const std::string& direction, const json& j);
    void kill(const std::string& enemy, const json& gameData);
    void unknownCommand(const std::string& command);

    std::string curRoom;
private:
    std::vector<std::string> inventory;
    json& gameData;
};

#endif // PLAYER_H
