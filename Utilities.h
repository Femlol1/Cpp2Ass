#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>

struct MapObject {
    std::string category;
    std::string id;
    std::string description;
    std::string initialRoom;
    std::string aggressiveness;
    std::string killedby;
};

std::vector<MapObject> readMap(const std::string& mapFileName);

#endif // UTILITIES_H