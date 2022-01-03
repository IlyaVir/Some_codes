#pragma once

#include <vector>
#include <limits>
#include <list>

struct Edge {
    explicit Edge(size_t from_ = 0, size_t to_ = 0, int cost_ = 0) : from(from_), to(to_), cost(cost_) {}
    size_t from, to;
    int cost;
};

std::vector <std::list <size_t>> edges_to_adjacency_list(size_t size, const std::vector <Edge> &edges) {
    std::vector <std::list <size_t>> adjacency_list(size);

    for (const auto [from, to, cost] : edges) {
        adjacency_list[from].push_back(to);
    }

    return adjacency_list;
}

const int kINF = std::numeric_limits<int>::max();

void dfs_mark_minus_infinity(const std::vector <std::list <size_t>> &adjacency_list, size_t from,
                         std::vector <int> &distances) {
    distances[from] = -kINF;
    for (auto to : adjacency_list[from]) {
        if (distances[to] != -kINF) {
            dfs_mark_minus_infinity(adjacency_list, to, distances);
        }
    }
}

void mark_minus_infinity(const std::vector <Edge> &edges, std::vector <int> &distances) {
    auto adjacency_list = edges_to_adjacency_list(distances.size(), edges);
    for (const auto [from, to, cost] : edges) {
        if ((distances[from] == -kINF) ||
            (distances[from] != kINF && distances[from] + cost < distances[to])) {
            dfs_mark_minus_infinity(adjacency_list, from, distances);
        }
    }
}

std::vector <int> ford_bellman(size_t size, const std::vector <Edge> &edges, size_t start = 0) {
    std::vector <int> distances(size, kINF);
    distances[start] = 0;

    {
        bool changed = true;
        for (size_t iters = 0; iters < size && changed; ++iters) {
            changed = false;
            for (const auto [from, to, cost] : edges) {
                if (distances[from] != kINF && distances[from] + cost < distances[to]) {
                    distances[to] = distances[from] + cost;
                    changed = true;
                }
            }
        }
    }

    // Учёт отрицательных циклов
    mark_minus_infinity(edges, distances);

    return distances;
}
