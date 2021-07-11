#include "Graph.h"

const int dcount = 8;
const int dl[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
const int dc[] = { 0, 1, 1, 1, 0, -1, -1, -1 };

bool interior(int i, int j, int n, int m) {
    return i >= 0 && i < n && j >= 0 && j < m;
}

bool goodSymbol(char ch) {
    switch (ch) {
        case ' ':
        case '\t':
        case '\0':
        case '\n':
            return false;
    }
    return true;
}

MapColoring::MapColoring(const Graph& g, const std::vector<int>& cols, int nc)
    : graph(g) {
    numColors = nc;
    colors = cols;
}

std::ostream& operator << (std::ostream& out, const MapColoring& coloring) {
    for (int i=0;i<coloring.colors.size();++i) {
        out << coloring.graph.getNodeCountry(i) << " - " << coloring.colors[i]
            << " ( " << i << " )\n";
    }
    return out;
}

std::ostream& operator << (std::ostream& out, const MapColoring* coloring) {
    out << *coloring;
    return out;   
}

int Graph::getCountryInt(char ch) {
    if (!countryCharToNode.count(ch)) {
        nodeToCountryChar[n] = ch;
        countryCharToNode[ch] = n++;
        g.push_back(std::unordered_set<int>());
    }
    return countryCharToNode[ch];
}

void Graph::processCountry(const Map& map, int l, int c, int country, std::vector<std::vector<bool>>& visited) {
    visited[l][c] = true;

    for (int k=0;k<dcount;++k) {
        int nl = l + dl[k];
        int nc = c + dc[k];
        
        if (!interior(nl, nc, map.n, map.m)) {
            continue;
        }
        if (visited[nl][nc]) {
            continue;
        }
        if (!goodSymbol(map.map[nl][nc])) {
            continue;
        }

        int ncountry = getCountryInt(map.map[nl][nc]);
        if (ncountry == country) {
            processCountry(map, nl, nc, country, visited);
        } else {
            g[country].insert(ncountry);
            g[ncountry].insert(country);
        }
    }
}

bool Graph::build(Map& map) {
    std::vector<std::vector<bool>> visited(map.n, std::vector<bool>(map.m, false));

    for (int i=0;i<map.n;++i) {
        for (int j=0;j<map.m;++j) {
            if (visited[i][j] || !goodSymbol(map.map[i][j])) {
                continue;
            }
            processCountry(map, i, j, getCountryInt(map.map[i][j]), visited);
        }
    }
    
    return true;
}

void Graph::bkt(int node, bktMaps& maps, bktSets& sets, 
    std::vector<int>& csol, std::vector<int>& sol) {
    if (ok) {
        return;
    }
    if (node == n) {
        ok = true;
        sol = csol;
        return;
    }
    for (auto possibleColor : sets[node]) {
        csol[node] = possibleColor;

        // remove the color from all the neighbors
        for (auto x : g[node]) {
            if (x <= node) {
                continue;
            }
            --maps[x][possibleColor];
            if (!maps[x][possibleColor]) {
                sets[x].erase(possibleColor);
            }
        }
        bkt(node+1, maps, sets, csol, sol);
        if (ok) {
            return;
        }

        // add the color back to all the neighbors
        for (auto x : g[node]) {
            if (x <= node) {
                continue;
            }
            ++maps[x][possibleColor];
            if (maps[x][possibleColor] == 1) {
                sets[x].insert(possibleColor);
            }
        }
    }
}

bool Graph::colorMap(std::unique_ptr<MapColoring>& coloring, int numColors) {
    std::unordered_map<int,int> mp;
    std::unordered_set<int> st;
    for (int i=0;i<numColors;++i) {
        mp[i] = 1;
        st.insert(i);
    }
    
    bktMaps maps;
    bktSets sets;
    for (int i=0;i<n;++i) {
        maps.push_back(mp);
        sets.push_back(st);
    }

    ok = false;
    std::vector<int> csol(n, -1);
    std::vector<int> sol;

    bkt(0, maps, sets, csol, sol);

    if (!ok) {
        return false;
    }

    coloring.reset( new MapColoring(*this, sol, numColors) );
    return true;
}

char Graph::getNodeCountry(int node) const {
    auto it = nodeToCountryChar.find(node);
    if (it == nodeToCountryChar.end()) {
        return '?';
    }
    return (*it).second;
}

std::ostream& operator << (std::ostream& out, const Graph& graph) {
    out << "Graph nodes count: " << graph.n << "\n";
    
    out << "Node mapping: \n";
    for (auto country : graph.countryCharToNode) { 
        out << country.first << " - " << country.second << " ( "
            << (int)country.first << " )\n";
    }

    out << "Adjacency lists: \n";
    int index = 0;
    for (auto& list : graph.g) {
        out << index << ": ";
        if (!list.size()) {
            continue;
        }
        auto iter = list.begin();
        out << *iter;
        while (++iter != list.end()) {
            out << ", " << *iter;
        }
        out << '\n';
        ++index;
    }
    return out;
}

std::ostream& operator << (std::ostream& out, const Graph* graph) {
    out << *graph;
    return out;
}
