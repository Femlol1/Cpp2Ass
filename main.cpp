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

int main()
{
	
    ifstream fin("map1.json");
	json j; // object that represents the json data
	fin >> j; // read from file into j

	Player player(j);
	//initialises the player to the starting room according to the map json
	player.curRoom = j["player"]["initialroom"].get<string>();

	// Creates a map with the commands, can be expanded as currently assumes knowledge of valid commands
    unordered_map<string, CommandFunction> commands;
    //commands["grab"] = &Player::grab;
    commands["move"] = &Player::move;
    //commands["kill"] = &Player::kill;
    //commands["look"] = &Player::look;
    
    int numTypes = j.size();
	//cout << numTypes << endl;

	/*for(auto e : j.items()) {
		string s = e.key();
		cout << s << endl;
	}*/

	// This outputs the number of rooms and something about the 2nd room
	//int numRooms = j["rooms"].size();
	//cout << numRooms << endl;
	//string room1desc =  j["rooms"][0]["desc"].get<string>();
	//cout << room1desc << endl;

	// This retrieves the aggressiveness of the first enemy,;o
	// and the list of objects that kills it as a vector
	//int agg = j["enemies"][0]["aggressiveness"].get<int>();
	//cout << agg << endl;
	//vector<string> v = j["enemies"][0]["killedby"].get<vector<string>>();
	//for(string s : v) cout << s << endl;

	//string s;
	//try {
	//	s = j["enemies"][0]["intro_msg"].get<string>();
	//}
	//catch(const json::exception& e) {
	//	s = "some default message";
	//}
	//cout << s << endl;

	string mapFileName = "map1.json";
    vector<MapObject> mapObjects = readMap(mapFileName);

    // Print the created objects
    for (const auto& obj : mapObjects) {
        cout << "Category: " << obj.category << ", ID: " << obj.id << ", Description: " << obj.description;
        if (!obj.initialRoom.empty()) {
            cout << ", Initial Room: " << obj.initialRoom;
        }
        cout << endl;
    }

    return 0;
    
    
    
	while (true){
		cout << j["rooms"][player.curRoom]["desc"].get<string>() << endl;

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
		break;
    }
	}
    return 0;


	//Redundant for now, can be implemented later to make the code cleaner
    // program program;
    // program();
}

