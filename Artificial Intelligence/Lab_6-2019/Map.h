#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <algorithm>

#include "Graph.h"

class Graph;

extern void err(const char *);
extern void err(const std::string&);

class Map {
    private:
        int n = 0;

        int m = 0;

        std::vector<std::string> map;

        friend class Graph;
    public:
        Map() = default;

        bool open(const char *fileName);

        std::unique_ptr<Graph> buildGraph();
};

#endif // MAP_H
