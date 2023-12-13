#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "Player.h"
#include "Utilities.h"
#include <filesystem>
#include <cstdlib>
#include <ctime>


#include "json.hpp"

using json = nlohmann::json;
using  namespace std;
using namespace std::filesystem;

#include "program.cpp"


typedef void (Player::*CommandFunction)(const string&, const json&);



int main() {
    // Random number generator, for fanciness it has a different seed each session
    srand(time(0));
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
//    readMap();
    json j; // object that represents the json data
    fin >> j; // read from file into j

    Player player(j);
    //initialises the player to the starting room according to the map json
    player.curRoom = j["player"]["initialroom"].get<string>();

    player.printRoomAndItems();

    


    // Creates a map with the commands, can be expanded as currently assumes knowledge of valid commands
    unordered_map<string, CommandFunction> commands;
    commands["grab"] = &Player::grab;
    commands["move"] = &Player::move;
    commands["go"] = &Player::move;
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
        ss >> command;
        ss.ignore();
        getline(ss, argument);


        //simple break command to prevent eternal looping and allows player to leave
        if (command == "exit") {
            break;
        }

        auto it = commands.find(command);
        if (it != commands.end()) {
            (player.*(it->second))(argument, j);
        } else {
            player.unknownCommand(command);
        }



        // Check kill objective
    auto objectiveType = j["objective"]["type"].get<std::string>();
    if (objectiveType == "kill") {
        // Update the requiredKills set
        auto requiredEnemies = j["objective"]["what"];
        for (const auto& enemy : requiredEnemies) {
            player.requiredKills.insert(enemy.get<std::string>());
        }
            std::unordered_set<std::string> intersection;
            std::set_intersection(player.getRequiredKills().begin(), player.getRequiredKills().end(), player.getDeadEnemies().begin(), player.getDeadEnemies().end(), std::inserter(intersection, intersection.begin()));

        if (intersection == player.getRequiredKills()) {
            cout << "Congratulations! You have completed the kill objective. You win!" << endl;
            break;  // Exit the game loop
        }
    }
    // else if (objectiveType == "collect"){
    //     if (objectiveType == "collect") {
    //     // Update the requiredItems set
    //     auto requiredItems = j["objective"]["what"];
    //     for (const auto& item : requiredItems) {
    //         player.collectItem(item.get<std::string>());
    //     }

    //     // Check if the player has collected all required items
    //     if (std::includes(player.collectedItems.begin(), player.collectedItems.end(),
    //                       requiredItems.begin(), requiredItems.end())) {
    //         cout << "Congratulations! You have completed the collect objective. You win!" << endl;
    //         break;  // Exit the game loop
    //     }
    // }
    // }
    else if (objectiveType == "room") {

        auto targetRoom = j["objective"]["what"][0].get<std::string>();
        if (player.curRoom == targetRoom){
            cout<< "Congratlations! You have reached the required room. You win!" << endl;
            break;
        }
    }
}
}