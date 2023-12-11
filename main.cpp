#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "Player.h"
#include "Utilities.h"
#include <filesystem>

#include "json.hpp"

using json = nlohmann::json;
using  namespace std;
using namespace std::filesystem;

#include "program.cpp"

typedef void (Player::*CommandFunction)(const string&, const json&);
int main() {
//    vector<string> availableMaps = {"map1.json", "map2.json","map3.json", "map4.json", "map5.json", "map6.json" }; // List your map files here
    string directoryPath = "./"; // Set the directory path where map files are stored

    vector<string> availableMaps;
    for (const auto &entry: directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".json") {
            availableMaps.push_back(entry.path().filename().string());
        }
    }

    if (availableMaps.empty()) {
        cerr << "No map files found in the directory." << endl;
        return 1;
    }
    int mapChoice = -1;
    while (mapChoice < 1 || mapChoice > availableMaps.size()) {
        cout << "Available maps:" << endl;
        for (int i = 0; i < availableMaps.size(); ++i) {
            cout << i + 1 << ": " << availableMaps[i] << endl;
        }
        cout << "Choose a map number: ";
        cin >> mapChoice;

        // Clearing any error states of cin (like if the input was not a number)
        if (cin.fail()) {
            cin.clear(); // Reset the failbit
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear out the input buffer
            mapChoice = -1; // Reset choice to keep the loop going
            cout << "Invalid input. Please enter a number corresponding to the map choices." << endl;
        }
    }

    string mapFileName = availableMaps[mapChoice - 1];

    ifstream fin(mapFileName);

    // ifstream fin("map1.json");// this also reads the file but we need to choose which one we are going to use
    if (!fin) {
        cerr << "Failed to open map" << mapFileName << endl;
        return 1;
    }
    json j; // object that represents the json data
    fin >> j; // read from file into j

    Player player(j);
    //initialises the player to the starting room according to the map json
    player.curRoom = j["player"]["initialroom"].get<string>();

    // Find and print the initial room description
    auto roomIt = find_if(j["rooms"].begin(), j["rooms"].end(),
                          [&player](const json& room) { return room["id"] == player.curRoom; });

    if (roomIt != j["rooms"].end()) {
        cout << roomIt->at("desc") << endl;
        for (const auto& obj : j["objects"]) {
            if (obj["initialroom"] == player.curRoom) {
                cout << "There is a "<<obj["id"] <<", "<< obj["desc"] << endl;
            }
        }

        // Display enemies in the room
        for (const auto& enemy : j["enemies"]) {
            if (enemy["initialroom"] == player.curRoom) {
                cout << "There is a "<<enemy["id"] <<", "<< enemy["desc"] << endl;
            }
        }

    }

    // Creates a map with the commands, can be expanded as currently assumes knowledge of valid commands
    unordered_map<string, CommandFunction> commands;
    commands["grab"] = &Player::grab;
    commands["move"] = &Player::move;
    commands["kill"] = &Player::kill;
    commands["look"] = &Player::look;


    while (true) {
        string input, command, argument;
//        cout << "Enter some text: ";
        getline(cin, input);


        // Skip empty input
        if(input.empty()) {
            continue;
        }

        stringstream ss(input);
        ss >> command >> argument;

        auto it = commands.find(command);
        if (it != commands.end()) {
            (player.*(it->second))(argument, j);
        } else {
            player.unknownCommand(command);
        }

        // Find the current room in the JSON data
        auto roomIt = find_if(j["rooms"].begin(), j["rooms"].end(),
                              [&player](const json &room) { return room["id"] == player.curRoom; });

        // Print the room description here
        if (roomIt != j["rooms"].end()) {
            cout << roomIt->at("desc") << endl;
            for (const auto& obj : j["objects"]) {
                if (obj["initialroom"] == player.curRoom) {
                    cout << "There is a "<<obj["id"] <<", "<< obj["desc"] << endl;
                }
            }

            // Display enemies in the room
            for (const auto& enemy : j["enemies"]) {
                if (enemy["initialroom"] == player.curRoom) {
                    cout << "There is a "<<enemy["id"] <<", "<< enemy["desc"] << endl;
                }
            }
        } else {
            cout << "You are in an unknown location." << endl;
            break;
        }
    }
}