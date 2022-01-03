#include <vector>
#include <climits>

struct Edge {
    explicit Edge(int from = 0, int to = 0, int cost = 0) : from_(from), to_(to), cost_(cost) {}
    int from_, to_, cost_;

    int from() const {
        return from_;
    }
    
    int to() const {
        return to_;
    }

    int cost() const {
        return cost_;
    }
};

// Бесконечность
const int kINF = INT_MAX;

std::vector <std::vector <int>> edges_to_list(size_t size, const std::vector <Edge> &edges) {
    std::vector <std::vector <int>> vertex_list(size);
    for (const Edge &edge : edges) {
        vertex_list[edge.from()].push_back(edge.to());
    }
    return vertex_list;
}

void mark_minus_infinity(const std::vector <std::vector <int>> &vertex_list, int from,
                         std::vector <bool> &minus_infinity) {
    for (int to : vertex_list[from]) {
        if (!minus_infinity[to]) {
            minus_infinity[to] = true;
            mark_minus_infinity(vertex_list, to, minus_infinity);
        }
    }
}

void mark_smallest(const std::vector <Edge> &edges, std::vector <int> &distances) {
    std::vector <bool> minus_infinity(distances.size(), false);

    // Вершины, которые обновятся, являются частью отрицательного цикла
    for (const Edge &edge : edges) {
        if (distances[edge.from()] != kINF && distances[edge.from()] + edge.cost() < distances[edge.to()]) {
            minus_infinity[edge.from()] = minus_infinity[edge.to()] = true;
            distances[edge.from()] = distances[edge.from()] + edge.cost();
        }
    }

    std::vector <std::vector <int>> list = edges_to_list(distances.size(), edges);
    for (int from = 0; from < minus_infinity.size(); ++from) {
        if (minus_infinity[from]) {
            mark_minus_infinity(list, from, minus_infinity);
        }
    }

    for (int i = 0; i < minus_infinity.size(); ++i) {
        if (minus_infinity[i]) {
            distances[i] = -kINF;
        }
    }
}

std::vector <int> ford_bellman(int size, const std::vector <Edge> &edges, int start = 0) {
    std::vector <int> distances(size, kINF);
    distances[start] = 0;

    bool changed = true;
    int iters = 0;
    while(changed && iters <= size) {
        changed = false;
        for (const Edge &edge : edges) {
            if (distances[edge.from()] != kINF && distances[edge.from()] + edge.cost() < distances[edge.to()]) {
                distances[edge.to()] = distances[edge.from()] + edge.cost();
                changed = true;
            }
        }
        ++iters;
    }

    // Учёт отрицательных циклов
    mark_smallest(edges, distances);

    return distances;
}
