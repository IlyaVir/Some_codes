// task - https://informatics.msk.ru/mod/statements/view.php?id=193&chapterid=3494#1

#include <vector>
#include <set>
#include <string>
#include <iostream>

const size_t kINF = 2009000999;

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

        std::vector <std::vector <Vertex>> list_vertexes(n);
        for (size_t i = 0; i < m; ++i) {
            size_t from, to, cost;
            std::cin >> from >> to >> cost;
            list_vertexes[from].emplace_back(to, cost);
            list_vertexes[to].emplace_back(from, cost);
        }

        size_t start;
        std::cin >> start;
        print(dijkstra(list_vertexes, start));
    }

    return 0;
}
