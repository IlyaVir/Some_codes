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

struct Edge {
    explicit Edge(int from = 0, int to = 0, int cost = 0) : from_(from), to_(to), cost_(cost) {}
    int from_, to_, cost_;
};

// Конвертер ребёр в список смежности без стоимости
vector <vector <int>> edges_to_list_without_cost(int size, const vector <Edge> &edges) {
    vector <vector <int>> vertex_list(size);
    for (const Edge &edge : edges) {
        vertex_list[edge.from_].push_back(edge.to_);
    }
    return vertex_list;
}

// Конвертер ребёр в список смежности со стоимостью
vector <vector <Vertex>> edges_to_list_with_cost(int size, const vector <Edge> &edges) {
    vector <vector <Vertex>> vertex_list(size);
    for (const Edge &edge : edges) {
        vertex_list[edge.from_].emplace_back(edge.to_, edge.cost_);
    }
    return vertex_list;
}

// dfs метит все вершины, достижимые из отрицательного цикла
void dfs(const vector <vector <int>> &vertex_list, int from, vector <bool> &smallest) {
    for (int to : vertex_list[from]) {
        if (!smallest[to]) {
            smallest[to] = true;
            dfs(vertex_list, to, smallest);
        }
    }
}

void mark_smallest(const vector <Edge> &edges, vector <int> &distances) {
    vector <bool> smallest(distances.size(), false);
    // В smallest отмечаем вершины, достижимые из отрицательного цикла

    // Вершины, которые обновятся, являются частю отрицательного цикла
    for (const Edge &e : edges) {
        if (distances[e.from_] != kINF && distances[e.from_] + e.cost_ < distances[e.to_]) {
            smallest[e.from_] = smallest[e.to_] = true;
            distances[e.to_] = distances[e.from_] + e.cost_;
        }
    }

    vector <vector <int>> list = edges_to_list_without_cost(distances.size(), edges);
    // dfs'ом пометим вершины, достижимые из отрицательного цикла
    for (int from = 0; from < smallest.size(); ++from) {
        if (smallest[from]) {
            dfs(list, from, smallest);
        }
    }

    // Присвоим вершинам, достижимым из отрицательного цикла, -бесконечность
    for (int i = 0; i < smallest.size(); ++i) {
        if (smallest[i]) {
            distances[i] = -kINF;
        }
    }
}

// Работает на основе алгоритма Форда-беллмана
pair <vector <Edge>, vector <int>> convert_edges_for_dijkstra(int size, const vector <Edge> &edges) {
    vector <int> distances(size, 0);
    // Расстояние от фантомной вершины до остальных равно 0

    bool changed;
    int iters = 0;
    // iters - кол-во итераций
    do {
        changed = false;
        for (const Edge &e : edges) {
            if (distances[e.from_] != kINF && distances[e.from_] + e.cost_ < distances[e.to_]) {
                distances[e.to_] = distances[e.from_] + e.cost_;
                changed = true;
            }
        }
        ++iters;
    } while(changed && iters <= size);

    // Учёт отрицательных циклов
    mark_smallest(edges, distances);

    // Перевешиваем рёбра для Дейкстры
    vector <Edge> new_edges;
    new_edges.reserve(edges.size());
    for (auto edge : edges) {
        if (distances[edge.from_] != -kINF && distances[edge.to_] != -kINF) {
            edge.cost_ += distances[edge.from_] - distances[edge.to_];
            new_edges.push_back(edge);
        }
    }

    // {новые_рёбра, перебалансировка}
    return {new_edges, distances};
}

vector <int> dijkstra(const vector <vector <Vertex>> &list_vertexes, int start = 0) {
    vector <int> distances(list_vertexes.size(), kINF);
    // kINF - бесконечность, т.е. кратчайшего пути не найдено
    distances[start] = 0;
    set <pair <int, int>> next_vertexes;
    // В next_vertexes хранятся в отсортированном виде пары
    // {Дистанция, Номер_вершины_до_которой_не_нашли_минимальное_расстояние}.
    // Таким образом первой в этой структуре будет ближайшая вершина, до
    // которой не известно расстояние
    next_vertexes.insert({distances[start], start});

    while (!next_vertexes.empty()) {
        int from = next_vertexes.begin() -> second;
        next_vertexes.erase(next_vertexes.begin());
        // Взяли ближайшую вершину

        for (const Vertex &to_vertex : list_vertexes[from]) {
            // Обновляем расстояние с учётом взятой вершины
            if (distances[to_vertex.to()] > distances[from] + to_vertex.cost()) {
                next_vertexes.erase({distances[to_vertex.to()], to_vertex.to()});
                distances[to_vertex.to()] = distances[from] + to_vertex.cost();
                next_vertexes.insert({distances[to_vertex.to()], to_vertex.to()});
            }
        }
    }

    return distances;
}

vector <vector <int>> johnson(int size, const vector <Edge> &edges) {
    auto [new_edges, rebalance] = convert_edges_for_dijkstra(size, edges);
    vector <vector <int>> distances(size);
    //distances[from][to] = min_distance

    // Формируем список смежности
    auto list_vertexes = edges_to_list_with_cost(size, new_edges);

    for (int from = 0; from < distances.size(); ++from) {
        distances[from] = dijkstra(list_vertexes, from);

        // Перебалансируем пути обратно
        for (int to = 0; to < distances[from].size(); ++to) {
            if (rebalance[from] == -kINF || rebalance[to] == -kINF) {
                // Достижимо из отрицательного цикла
                distances[from][to] = -kINF;
            } else if (distances[from][to] == kINF) {
                // Вообще не достижимо
                distances[from][to] = -1;
            } else {
                // Всё хорошо, балансируем обратно
                distances[from][to] += rebalance[to] - rebalance[from];
            }
        }
    }

    return distances;
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int n, m;
    cin >> n >> m;
    vector <Edge> edges(m);
    for (auto &edge : edges) {
        cin >> edge.from_ >> edge.to_ >> edge.cost_;
    }

    auto distances = johnson(n, edges);
    for (size_t from = 0; from < distances.size(); ++from) {
        for (size_t to = 0; to < distances[from].size(); ++to) {
            if (from != to && distances[from][to] != -kINF) {
                cout << from << ' ' << to << ' ' << distances[from][to] << '\n';
            }
        }
    }

    return 0;
}
