// task - https://informatics.msk.ru/mod/statements/view.php?id=193&chapterid=3494#1

#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <list>

const size_t kINF = 2009000999;

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

    size_t t;
    std::cin >> t;

    for (size_t never = 0; never < t; ++never) {
        size_t n, m;
        std::cin >> n >> m;

        std::vector <std::list <Vertex>> adjacency_list(n);
        for (size_t i = 0; i < m; ++i) {
            size_t from, to, cost;
            std::cin >> from >> to >> cost;
            adjacency_list[from].emplace_back(to, cost);
            adjacency_list[to].emplace_back(from, cost);
        }

        size_t start;
        std::cin >> start;
        print(dijkstra(adjacency_list, start));
    }

    return 0;
}
