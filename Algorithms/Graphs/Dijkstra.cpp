#include "ReadWriter.h"
//string, fstream, iostream, vector, Edge.h - included in ReadWriter.h
#include <set>
//Можно создавать любые классы и методы для решения задачи

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

using namespace std;

//Основной метод решения задачи, параметры:
//N - количество вершин, M - количество ребер в графе
//edges - вектор ориентированных ребер, каждое ребро представлено 3-мя числами (А, В, W),
// где A и B - номера вершин, которые оно соединяет (Путь строго из А в В), и W - вес ребра
//передается по ссылке (&), чтобы не копировать, изменять вектор и его значения можно.
//Результат также в виде вектора кратчайших расстояний из 0-й вершины во все остальные начиная с 1-й, то есть N-1 значение должно быть
void solve(int N, int M, vector<Edge>& edges, vector<int>& result)
{
    //Советую разделить решение на логические блоки
    vector <vector <Vertex>> list_vertexes(N); // лист смежности
    for (const auto &edge : edges) { // конвертируем рёбра в лист смежности
        list_vertexes[edge.A].emplace_back(edge.B, edge.W);
    }

    // Алгоритм Дейкстры
    int start = 0;
    vector <int> distances(list_vertexes.size(), kINF); // kINF - бесконечность, т.е. кратчайшего пути не найдено
    distances[start] = 0;
    set <pair <int, int>> next_vertexes; // В next_vertexes хранятся в отсортированном виде пары
                                        // {Дистанция, Номер_вершины_до_которой_не_нашли_минимальное_расстояние}.
                                        // Таким образом первой в этой структуре будет ближайшая вершина, до
                                        // которой не известно расстояние
    next_vertexes.insert({distances[start], start});

    while (!next_vertexes.empty()) {
        int from = next_vertexes.begin() -> second;
        next_vertexes.erase(next_vertexes.begin()); // Взяли ближайшую вершину

        for (const Vertex &to_vertex : list_vertexes[from]) {
            // Обновляем расстояние с учётом взятой вершины
            if (distances[to_vertex.to()] > distances[from] + to_vertex.cost()) {
                next_vertexes.erase({distances[to_vertex.to()], to_vertex.to()});
                distances[to_vertex.to()] = distances[from] + to_vertex.cost();
                next_vertexes.insert({distances[to_vertex.to()], to_vertex.to()});
            }
        }
    }

    // Запишем результат в result
    result.reserve(distances.size() - 1);
    for (size_t vertex = 1; vertex < distances.size(); ++vertex) {
        result.push_back(distances[vertex]);
    }

}

int main()
{
    ReadWriter rw;
    //Входные параметры
    //N - количество вершин, M - количество ребер в графе
    int N, M;
    rw.read2Ints(N, M);

    //Вектор ребер, каждое ребро представлено 3-мя числами (А, В, W), где A и B - номера вершин, которые оно соединяет, и W - вес ребра
    //Основной структурой выбран вектор, так как из него проще добавлять и удалять элементы (а такие операции могут понадобиться).
    vector<Edge> edges;
    rw.readEgdes(M, edges);

    //Основной структурой для ответа выбран вектор, так как в него проще добавлять новые элементы.
    vector<int> result;

    //Алгоритм решения задачи
    solve(N, M, edges, result);

    //Выводим результаты
    rw.writeInt(result.size());
    rw.writeIntValues(result);

    return 0;
}