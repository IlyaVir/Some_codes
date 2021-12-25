#include "ReadWriter.h"
//string, fstream, iostream, vector, Edge.h - included in ReadWriter.h

//Можно создавать любые классы и методы для решения задачи

using namespace std;

//Так как здесь есть несколько способов вывода ответа, то я предлагаю вам самим реализовать метод вывода,
//определенный в классе ReadWriter.
//Можно прямо сюда закинуть матрицу, и потом вычислять какие значения записывать в файл,
//или сначала сформировать вывод в каком-то вспомогательном методе и затем здесь только отправлять в файл.
    void ReadWriter::writeValues(std::vector<std::vector<int>> result)
    {
        if (!fout.is_open())
            throw std::ios_base::failure("file not open");

		//Для записи в файл используйте конструкции:
		//fout << value1 << value2 << value3;

        for (size_t from = 0; from < result.size(); ++from) {
            for (size_t to = 0; to < result[from].size(); ++to) {
                if (from != to) {
                    fout << from << ' ' << to << ' ' << result[from][to] << '\n';
                }
            }
        }
    }


//Основной метод решения задачи, параметры:
//N - количество вершин, M - количество ребер в графе
//edges - вектор ориентированных ребер, каждое ребро представлено 3-мя числами (А, В, W),
// где A и B - номера вершин, которые оно соединяет (Путь строго из А в В), и W - вес ребра
//передается по ссылке (&), чтобы не копировать, изменять вектор и его значения можно.

void solve(int N, int M, vector<Edge>& edges, vector<vector<int>>& matrix)
{
    for (size_t from_to = 0; from_to < matrix.size(); ++from_to) {
        matrix[from_to][from_to] = 0; // путь из вершины в саму себя равен нулю
    }

    for (const auto &edge : edges) {
        matrix[edge.A][edge.B] = edge.W; // заносим в матрицу все пути из одного ребра
    }

    for (size_t middle = 0; middle < matrix.size(); ++middle) {
        for (size_t from = 0; from < matrix.size(); ++from) {
            for (size_t to = 0; to < matrix.size(); ++to) {
                if (matrix[from][middle] >= 0 && matrix[middle][to] >= 0) { // возможный путь существуют
                    if (matrix[from][to] >= 0) { // исходный путь существует
                        matrix[from][to] = min(matrix[from][to], matrix[from][middle] + matrix[middle][to]);
                    } else { // исходного пути не существует
                        matrix[from][to] = matrix[from][middle] + matrix[middle][to];
                    }
                }
            }
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
    vector<vector<int>> result(N, vector <int> (N, -1)); // (-1) означает, что пути не существует

    //Алгоритм решения задачи
    solve(N, M, edges, result);
	//Здесь можно вызвать ещё какой-то метод, если вам требуется.

    rw.writeValues(result);

    return 0;
}