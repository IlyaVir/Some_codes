#include <vector>

std::vector <std::vector <int>> floyd(std::vector <std::vector <int>> matrix) {
    // matrix[i][j] < 0 => Пути/ребра не существует
    for (size_t middle = 0; middle < matrix.size(); ++middle) {
        for (size_t from = 0; from < matrix.size(); ++from) {
            for (size_t to = 0; to < matrix.size(); ++to) {
                if (matrix[from][middle] >= 0 && matrix[middle][to] >= 0) {
                    if (matrix[from][to] >= 0) {
                        matrix[from][to] = std::min(matrix[from][to], matrix[from][middle] + matrix[middle][to]);
                    } else {
                        matrix[from][to] = matrix[from][middle] + matrix[middle][to];
                    }
                }
            }
        }
    }

    return matrix;
}
