#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <ostream>
#include <memory>

#include <iostream>

#include "Map.h"

class Map;

extern void err(const char *);
extern void err(const std::string&);

class Graph;

class MapColoring {
    private:
        int numColors;

        std::vector<int> colors;

        const Graph& graph;
    public:
        MapColoring(const Graph& g, const std::vector<int>& cols, int nc);

        friend std::ostream& operator << (std::ostream& out, const MapColoring& coloring);

        friend std::ostream& operator << (std::ostream& out, const MapColoring* coloring);
};

class Graph {
    private:
        using bktMaps = std::vector<std::unordered_map<int,int>>;
        using bktSets = std::vector<std::unordered_set<int>>;

        bool ok;

        int n = 0;

        std::unordered_map<char,int> countryCharToNode;

        std::unordered_map<int,char> nodeToCountryChar;

        std::vector<std::unordered_set<int>> g;

        int getCountryInt(char ch);

        void processCountry(const Map& map, int l, int c, int country, std::vector<std::vector<bool>>& visited);

        void bkt(int node, bktMaps& maps, bktSets& sets, std::vector<int>& csol, std::vector<int>& sol);

        friend class Map;

        friend class MapColoring;
    public:
        Graph() = default;

        bool build(Map& map);

        bool colorMap(std::unique_ptr<MapColoring>& coloring, int numColors);

        char getNodeCountry(int node) const;

        friend std::ostream& operator << (std::ostream& out, const Graph& graph);

        friend std::ostream& operator << (std::ostream& out, const Graph* graph);
};

#endif // GRAPH_H
