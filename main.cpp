#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;
using  namespace std;

#include "program.cpp"

class Player{
public:
	vector<string> inventory;
	string curRoom;

	json& gameData; // Reference to the JSON data

    Player(json& jsonData) : gameData(jsonData) {}



	void grab(const string& item) {
		const auto& objects = gameData.at("objects");
        const auto& curRoomObj = gameData.at("rooms").at(curRoom).at("objects");

        cout << "DEBUG: Objects size: " << objects.size() << endl;
        cout << "DEBUG: Current Room Objects size: " << curRoomObj.size() << endl;

        const auto& foundObject = std::find_if(objects.begin(), objects.end(), [&](const auto& obj) {
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

	void look(const string& item){
    	cout << "You look at: " << item << endl;
	}

	 void move(const string& direction, const json& roomData) {
        // Check if the requested direction is a valid exit
        auto exits = roomData["exits"];
        auto exitIt = exits.find(direction);

        if (exitIt != exits.end()) {
            // Valid exit, update the current room
            string newRoom = exitIt.value().get<string>();
            curRoom = newRoom;
            cout << "You moved to " << newRoom << endl;
        } else {
            cout << "You can't move in that direction" << endl;
        }
	 }

	void kill(const string& enemy){
    	cout << "You fought " << enemy << ", you died lol" << endl;
	}

	void use(const string& item){
		cout << "you used" <<  item << ", to open" << endl;
	}



	void unknownCommand(const string& command) {
    	cout << "Unknown command: " << command << endl;
	}
};
typedef void (Player::*CommandFunction)(const string&, const json&);
struct MapObject {
    string category;
    string id;
    string description;
    string initialRoom;
};


// Function to read the map and create objects
vector<MapObject> readMap(const string& mapFileName) {
    vector<MapObject> objects;

    ifstream fin(mapFileName);
    if (!fin.is_open()) {
        cerr << "Error opening map file: " << mapFileName << endl;
        return objects;
    }

    json j;
    fin >> j;

    // Read Rooms
    for (const auto& room : j["rooms"]) {
        MapObject roomObject;
        roomObject.category = "Room";
        roomObject.id = room["id"];
        roomObject.description = room["desc"];
        objects.push_back(roomObject);
    }

    // Read Objects
    for (const auto& obj : j["objects"]) {
        MapObject objectObject;
        objectObject.category = "Object";
        objectObject.id = obj["id"];
        objectObject.description = obj["desc"];
        objectObject.initialRoom = obj["initialroom"];
        objects.push_back(objectObject);
    }

    // Add more categories as needed...

    return objects;
}

int main() {

    ifstream fin("map1.json");// this also reads the file but we need to choose which one we are going to use
    json j; // object that represents the json data
    fin >> j; // read from file into j

    Player player(j);
    //initialises the player to the starting room according to the map json
    player.curRoom = j["player"]["initialroom"].get<string>();
    cout << player.curRoom << endl;
    // Creates a map with the commands, can be expanded as currently assumes knowledge of valid commands
    unordered_map<string, CommandFunction> commands;
    //commands["grab"] = &Player::grab;
    commands["move"] = &Player::move;
    //commands["kill"] = &Player::kill;
    //commands["look"] = &Player::look;

//    string roomDesc = j["rooms"][player.curRoom]["desc"];

        string mapFileName = "map1.json";// this reads the file
        vector<MapObject> mapObjects = readMap(mapFileName);


        for (const auto &room: mapObjects) {
            string roomId = room.id;
            if (player.curRoom == roomId) {
                cout << "Match found! Room ID: " << roomId << endl;
                string roomDesc = room.description;
                cout << "Room Description: " << roomDesc << endl;
                break; // Break out of the loop once the matching room is found
            }
        }

        // Print the created objects
        for (const auto &obj: mapObjects) {
            //cout << "Category: " << obj.category << ", ID: " << obj.id << ", Description: " << obj.description;
            if (!obj.initialRoom.empty()) {
                cout << ", Initial Room: " << obj.initialRoom;
            }
            cout << endl;
        }




//	while (true){
//		cout << j["rooms"][player.curRoom]["desc"].get<string>() << endl;

        string input, command, argument;
        cout << "Enter some text: ";
        getline(cin, input);

        cout << "You entered: " << input << endl;

        stringstream ss(input);
        ss >> command >> argument;

        auto it = commands.find(command);
        if (it != commands.end()) {
            (player.*(it->second))(argument, j);
        } else {
            player.unknownCommand(command);
//		break;
        }

        return 0;


        //Redundant for now, can be implemented later to make the code cleaner
        // program program;
        // program();
    }
