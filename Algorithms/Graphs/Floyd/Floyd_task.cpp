// task - https://informatics.msk.ru/mod/statements/view.php?chapterid=171#1

#include <vector>
#include <iostream>
#include <string>

std::vector <std::vector <int>> floyd(std::vector <std::vector <int>> matrix) {
    for (size_t middle = 0; middle < matrix.size(); ++middle) {
        for (size_t from = 0; from < matrix.size(); ++from) {
            for (size_t to = 0; to < matrix.size(); ++to) {
                matrix[from][to] = std::min(matrix[from][to], matrix[from][middle] + matrix[middle][to]);
            }
        }
    }

    return matrix;
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

    size_t vertexes;
    std::cin >> vertexes;
    std::vector <std::vector <int>> distances(vertexes, std::vector <int>(vertexes));
    for (auto &line : distances) {
        for (auto &cell : line) {
            std::cin >> cell;
        }
    }

    for (const auto &line : floyd(distances)) {
        print(line);
    }

    return 0;
}
