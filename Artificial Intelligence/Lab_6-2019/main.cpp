#include <string>
#include <cstdlib>
#include <iostream>
#include <memory>

#include "Map.h"
#include "Graph.h"

#define CHECK(funcCall) \
    if (!(funcCall)) { \
        std::cout << errMsg << '\n'; \
        exit(1); \
    }

// Extern variables and functions

std::string errMsg;

void err(const std::string& errM) {
    errMsg = errM;
}

void err(const char *errM) {
    err(std::string(errM));
}

// End of extern variables and functions

const char *mapFilePrefix = "ignored/";

void printDelimiterLine() {
    std::cout << "-------------------------------\n";
}

void tryColoring(int colors, Graph* graph) {
    printDelimiterLine();
    std::unique_ptr<MapColoring> solution;
    if (!graph->colorMap(solution, colors)) {
        std::cout << "Can't color map in " << colors << " colors!\n";
        return;
    }
    std::cout << "It is possible to colors map in " << colors << " colors!\n";
    std::cout << solution.get();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Invalid number of arguments! One expected, the name of the file map!\n";
        return 1;
    }
    std::string fileName = std::string(mapFilePrefix) + std::string(argv[1]);

    Map map;
    CHECK(map.open(fileName.c_str()));
    
    std::unique_ptr<Graph> graph;
    CHECK((graph = map.buildGraph()));

    printDelimiterLine();
    std::cout << graph.get();

    tryColoring(1, graph.get());
    tryColoring(2, graph.get());
    tryColoring(3, graph.get());
    tryColoring(4, graph.get());
    tryColoring(5, graph.get());

    return 0;
}
