#pragma once

#include <vector>
#include <set>
#include <limits>
#include <list>

const size_t kINF = std::numeric_limits<size_t>::max();

struct Vertex {
    explicit Vertex(size_t to_ = 0, size_t cost_ = 0) : to(to_), cost(cost_) {}
    
    size_t to, cost;
};

std::vector <size_t> dijkstra(const std::vector <std::list <Vertex>> &adjacency_list, size_t start = 0) {
    // adjacency_list[from] = {to, cost}
    using namespace std;

    vector <size_t> distances(adjacency_list.size(), kINF);
    distances[start] = 0;
    set <pair <size_t, size_t>> next_vertexes;
    next_vertexes.insert({distances[start], start});

    while (!next_vertexes.empty()) {
        size_t from = next_vertexes.begin() -> second;
        next_vertexes.erase(next_vertexes.begin());

        for (const auto [to, cost] : adjacency_list[from]) {
            if (distances[to] > distances[from] + cost) {
                next_vertexes.erase({distances[to], to});
                distances[to] = distances[from] + cost;
                next_vertexes.insert({distances[to], to});
            }
        }
    }

    return distances;
}
