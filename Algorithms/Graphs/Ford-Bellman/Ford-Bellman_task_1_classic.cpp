// task - https://informatics.msk.ru/mod/statements/view.php?id=10907&chapterid=178#1

#include <vector>
#include <iostream>
#include <string>
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

const int kINF = 30'000;

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

inline void fast() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
}

template <class iterator>
void print(const iterator &begin_iterator, const iterator &end_iterator,
           const std::string &sep = " ", const std::string &end = "\n", std::ostream &out = std::cout) {
    for (iterator now = begin_iterator; now != end_iterator; ++now) {
        out << *now << sep;
    }
    out << end;
}

template <class container>
void print(const container &massive,
           const std::string &sep = " ", const std::string &end = "\n", std::ostream &out = std::cout) {
    print(massive.begin(), massive.end(), sep, end);
}

int main() {
    fast();

    size_t n, m;
    std::cin >> n >> m;

    std::vector <Edge> edges(m);
    for (auto &[from, to, cost] : edges) {
        std::cin >> from >> to >> cost;
        --from;
        --to;
    }

    print(ford_bellman(n, edges));

    return 0;
}