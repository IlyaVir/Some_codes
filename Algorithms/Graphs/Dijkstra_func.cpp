#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <set>

using namespace std;

const int kINF = 1e9;

struct Vertex {
    explicit Vertex(int to = 0, int cost = 0) : to_(to), cost_(cost) {}
    int to_, cost_;

    int to() const {
        return to_;
    }

    int cost() const {
        return cost_;
    }
};

vector <int> dijkstra(const vector <vector <Vertex>> &list_vertexes, int start = 0) {
    vector <int> distances(list_vertexes.size(), kINF);
    distances[start] = 0;
    set <pair <int, int>> next_vertexes;
    next_vertexes.insert({distances[start], start});

    while (!next_vertexes.empty()) {
        int from = next_vertexes.begin() -> second;
        next_vertexes.erase(next_vertexes.begin());

        for (const Vertex &to_vertex : list_vertexes[from]) {
            if (distances[to_vertex.to()] > distances[from] + to_vertex.cost()) {
                next_vertexes.erase({distances[to_vertex.to()], to_vertex.to()});
                distances[to_vertex.to()] = distances[from] + to_vertex.cost();
                next_vertexes.insert({distances[to_vertex.to()], to_vertex.to()});
            }
        }
    }

    return distances;
}
