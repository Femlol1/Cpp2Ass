#include "Utilities.h"
#include "json.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>

#include <vector>
#include <string>

using json = nlohmann::json;
using namespace std;
using namespace std::filesystem;




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