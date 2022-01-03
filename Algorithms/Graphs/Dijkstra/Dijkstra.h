#pragma once

#include <vector>
#include <set>
#include <limits>

const size_t kINF = std::numeric_limits<size_t>::max();

struct Vertex {
    explicit Vertex(size_t to = 0, size_t cost = 0) : to_(to), cost_(cost) {}
    size_t to_, cost_;

    size_t to() const {
        return to_;
    }

    size_t cost() const {
        return cost_;
    }
};

std::vector <size_t> dijkstra(const std::vector <std::vector <Vertex>> &list_vertexes, size_t start = 0) {
    // list_vertexes[from] = {to, cost}
    using namespace std;

    vector <size_t> distances(list_vertexes.size(), kINF);
    distances[start] = 0;
    set <pair <size_t, size_t>> next_vertexes;
    next_vertexes.insert({distances[start], start});

    while (!next_vertexes.empty()) {
        size_t from = next_vertexes.begin() -> second;
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
