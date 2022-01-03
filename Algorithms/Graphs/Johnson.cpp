#include <vector>
#include <climits>
#include <set>

const int kINF = INT_MAX;

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

std::vector <std::vector <int>> edges_to_list_without_cost(size_t size, const std::vector <Edge> &edges) {
    std::vector <std::vector <int>> vertex_list(size);

    for (const Edge &edge : edges) {
        vertex_list[edge.from()].push_back(edge.to());
    }

    return vertex_list;
}

std::vector <std::vector <Vertex>> edges_to_list_with_cost(size_t size, const std::vector <Edge> &edges) {
    std::vector <std::vector <Vertex>> vertex_list(size);

    for (const Edge &edge : edges) {
        vertex_list[edge.from_].emplace_back(edge.to_, edge.cost_);
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

    std::vector <std::vector <int>> list = edges_to_list_without_cost(distances.size(), edges);
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

// Работает на основе алгоритма Форда-беллмана
std::pair <std::vector <Edge>, std::vector <int>> convert_edges_for_dijkstra(int size, const std::vector <Edge> &edges) {
    std::vector <int> distances(size, 0);

    bool changed;
    int iters = 0;
    do {
        changed = false;
        for (const Edge &edge : edges) {
            if (distances[edge.from()] != kINF && distances[edge.from()] + edge.cost() < distances[edge.to()]) {
                distances[edge.to()] = distances[edge.from()] + edge.cost();
                changed = true;
            }
        }
        ++iters;
    } while(changed && iters <= size);

    // Учёт отрицательных циклов
    mark_smallest(edges, distances);

    // Перевешиваем рёбра для Дейкстры
    std::vector <Edge> new_edges;
    new_edges.reserve(edges.size());
    for (auto edge : edges) {
        if (distances[edge.from()] != -kINF && distances[edge.to()] != -kINF) {
            edge.cost_ += distances[edge.from()] - distances[edge.to()];
            new_edges.push_back(edge);
        }
    }

    // {новые_рёбра, перебалансировка}
    return {new_edges, distances};
}

std::vector <int> dijkstra(const std::vector <std::vector <Vertex>> &list_vertexes, int start = 0) {
    using namespace std;

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

std::vector <std::vector <int>> johnson(int size, const std::vector <Edge> &edges) {
    auto [new_edges, rebalanced_distances] = convert_edges_for_dijkstra(size, edges);
    std::vector <std::vector <int>> distances(size);
    //distances[from][to] = min_distance

    auto list_vertexes = edges_to_list_with_cost(size, new_edges);

    for (int from = 0; from < distances.size(); ++from) {
        distances[from] = dijkstra(list_vertexes, from);

        // Перебалансируем пути обратно
        for (int to = 0; to < distances[from].size(); ++to) {
            if (rebalanced_distances[from] == -kINF || rebalanced_distances[to] == -kINF) {
                // Достижимо из отрицательного цикла
                distances[from][to] = -kINF;
            } else if (distances[from][to] == kINF) {
                // Вообще не достижимо
                distances[from][to] = -1;
            } else {
                // Всё хорошо, балансируем обратно
                distances[from][to] += rebalanced_distances[to] - rebalanced_distances[from];
            }
        }
    }

    return distances;
}
