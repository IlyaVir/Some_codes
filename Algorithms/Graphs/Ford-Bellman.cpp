#include "ReadWriter.h"
//string, fstream, iostream, vector, Edge.h - included in ReadWriter.h

//Можно создавать любые классы и методы для решения задачи

using namespace std;

// Бесконечность
const int kINF = 1e9;

// Конвертер ребёр в список смежности
vector <vector <int>> edges_to_list(int size, const vector <Edge> &edges) {
    vector <vector <int>> vertex_list(size);
    for (const Edge &e : edges) {
        vertex_list[e.A].push_back(e.B);
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
        if (distances[e.A] != kINF && distances[e.A] + e.W < distances[e.B]) {
            smallest[e.A] = smallest[e.B] = true;
            distances[e.A] = distances[e.A] + e.W;
        }
    }

    vector <vector <int>> list = edges_to_list(distances.size(), edges);
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

vector <int> ford_bellman(int size, const vector <Edge> &edges, int start = 0) {
    vector <int> distances(size, kINF);
    // kINF - бесконечность, т.е. до вершины кратчайший путь пока не найден
    distances[start] = 0;

    bool changed;
    int iters = 0;
    // iters - кол-во итераций
    do {
        changed = false;
        for (const Edge &e : edges) {
            if (distances[e.A] != kINF && distances[e.A] + e.W < distances[e.B]) {
                distances[e.B] = distances[e.A] + e.W;
                changed = true;
            }
        }
        ++iters;
    } while(changed && iters <= size);

    // Учёт отрицательных циклов
    mark_smallest(edges, distances);

    return distances;
}

//Основной метод решения задачи, параметры:
//N - количество вершин, M - количество ребер в графе
//edges - вектор ориентированных ребер, каждое ребро представлено 3-мя числами (А, В, W),
// где A и B - номера вершин, которые оно соединяет (Путь строго из А в В), и W - вес ребра
//передается по ссылке (&), чтобы не копировать, изменять вектор и его значения можно.
//Результат также в виде вектора кратчайших расстояний из 0-й вершины во все остальные начиная с 1-й, то есть N-1 значение должно быть
void solve(int N, int M, vector<Edge>& edges, vector<int>& result)
{
    //Советую разделить решение на логические блоки
    result.reserve(N);
    auto distances = ford_bellman(N, edges);
    for (int to = 1; to < distances.size(); ++to) {
        // Добавляем не бесконечные дистанции
        if (abs(distances[to]) != kINF) {
            result.push_back(distances[to]);
        }
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
    if (!result.empty()) {
        rw.writeInt(result.size());
        rw.writeIntValues(result);
    }

    return 0;
}