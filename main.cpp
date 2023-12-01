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

vector<string> inventory;

void grab(const string& item) {
    cout << "You grabbed: " << item << endl;


}

void look(const string& item){
    cout << "You look at: " << item << endl;
}

void move(const string& direction){
    cout << "You moved in " << direction << "direction" << endl;
}

void kill(const string& enemy){
    cout << "You fought " << enemy << ", you died lol" << endl;
}

void unknownCommand(const string& command) {
    cout << "Unknown command: " << command << endl;
}

typedef void (*CommandFunction)(const string&);

int main()
{
    ifstream fin("map1.json");
	json j; // object that represents the json data
	fin >> j; // read from file into j

    unordered_map<string, CommandFunction> commands;
    commands["grab"] = grab;
    commands["move"] = move;
    commands["kill"] = kill;
    commands["look"] = look;
    
    int numTypes = j.size();
	//cout << numTypes << endl;

	/*for(auto e : j.items()) {
		string s = e.key();
		cout << s << endl;
	}*/

	// This outputs the number of rooms and something about the 2nd room
	int numRooms = j["rooms"].size();
	cout << numRooms << endl;
	string room1desc =  j["rooms"][0]["desc"].get<string>();
	cout << room1desc << endl;

	// This retrieves the aggressiveness of the first enemy,;o
	// and the list of objects that kills it as a vector
	/*int agg = j["enemies"][0]["aggressiveness"].get<int>();
	cout << agg << endl;
	vector<string> v = j["enemies"][0]["killedby"].get<vector<string>>();
	for(string s : v) cout << s << endl;


	// if you want to handle fields that may or may not exist in the json
	// file, here is one way to do it:
	string s;
	try {
		s = j["enemies"][0]["intro_msg"].get<string>();
	}
	catch(const json::exception& e) {
		s = "some default message";
	}
	cout << s << endl;*/
    
    
    


    string input, command, argument;
    cout << "Enter some text: ";
    getline(cin, input);
    cout << "You entered: " << input << endl;
    stringstream ss(input);
    ss >> command >> argument;

    auto it = commands.find(command);
    if (it != commands.end()) {
        it->second(argument);
    } else {
        unknownCommand(command);
    }
    return 0;


    // program program;
    // program();
}

