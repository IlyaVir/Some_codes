#include <vector>
#include <set>
#include <climits>

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
