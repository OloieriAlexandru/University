#include "Map.h"

bool Map::open(const char *fileName) {
    if (!fileName) {
        err("NULL fileName passed as argument for Map::open() call!");
        return false;
    }
    std::ifstream fileIn(fileName);
    if (!fileIn.is_open()) {
        err("Failed to open \"" + std::string(fileName) + "\"");
        return false;
    }
    
    std::string line;
    int maxCharsOnLine = -1;
    while (std::getline(fileIn, line)) {
        while (line.size() && (line.back() == ' ' || line.back() == '\t')) {
            line.pop_back();
        }
        if (line.size()) {
            map.push_back(line);
            maxCharsOnLine = std::max(maxCharsOnLine, (int)line.size());
        }
    }

    if (!map.size()) {
        err("Empty map file! \"" + std::string(fileName) + "\"");
        return false;
    }

    for (auto& l : map) {
        while (l.size() < maxCharsOnLine) {
            l.push_back(' ');
        }
    }

    n = map.size();
    m = maxCharsOnLine;
    return true;
}

std::unique_ptr<Graph> Map::buildGraph() {
    std::unique_ptr<Graph> res;

    Graph *g = new Graph();
    if (!g->build(*this)) {
        res.reset(NULL);
    } else {
        res.reset(g);
    }

    return res;
}
