#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <limits>

using namespace std;

class Graph {
private:
    int** Matrix;

public:
    int V;

    Graph(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            file >> V;
            Matrix = new int*[V];
            for (int i = 0; i < V; ++i) {
                Matrix[i] = new int[V];
                for (int j = 0; j < V; ++j) {
                    file >> Matrix[i][j];
                }
            }
            file.close();
        } else {
            cout << "Файл не найден\n";
            V = 0;
            Matrix = nullptr;
        }
    }

    ~Graph() {
        for (int i = 0; i < V; ++i) {
            delete[] Matrix[i];
        }
        delete[] Matrix;
    }

    void addVertex() {
        int** newMatrix = new int*[V + 1];
        for (int i = 0; i < V + 1; ++i) {
            newMatrix[i] = new int[V + 1];
            for (int j = 0; j < V + 1; ++j) {
                if (i < V && j < V) {
                    newMatrix[i][j] = Matrix[i][j];
                } else {
                    newMatrix[i][j] = 0;
                }
            }
        }
        for (int i = 0; i < V; ++i) {
            delete[] Matrix[i];
        }
        delete[] Matrix;
        Matrix = newMatrix;
        ++V;
    }

    void removeVertex(int v) {
        v--;
        if (v >= V) return;
        int** newMatrix = new int*[V - 1];
        for (int i = 0, ni = 0; i < V; ++i) {
            if (i == v) continue;
            newMatrix[ni] = new int[V - 1];
            for (int j = 0, nj = 0; j < V; ++j) {
                if (j == v) continue;
                newMatrix[ni][nj] = Matrix[i][j];
                nj++;
            }
            ni++;
        }
        for (int i = 0; i < V; ++i) {
            delete[] Matrix[i];
        }
        delete[] Matrix;
        Matrix = newMatrix;
        --V;
    }

    void addEdge(int u, int v, int weight) {
        u--;
        v--;
        if (u < V && v < V) {
            Matrix[u][v] = weight;
        }
    }

    void removeEdge(int u, int v) {
        u--;
        v--;
        if (u < V && v < V) {
            Matrix[u][v] = 0;
        }
    }

    void print() {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                cout << Matrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    bool detectCycleUtil(int v, vector<bool>& visited, vector<bool>& recStack) {
        if (!visited[v]) {
            visited[v] = true;
            recStack[v] = true;

            for (int i = 0; i < V; ++i) {
                if (Matrix[v][i]) {
                    if (!visited[i] && detectCycleUtil(i, visited, recStack)) {
                        return true;
                    } else if (recStack[i]) {
                        return true;
                    }
                }
            }
        }
        recStack[v] = false;
        return false;
    }
    // Обход в глубину
    bool detectCycle() {
        vector<bool> visited(V, false);
        vector<bool> recStack(V, false);

        for (int i = 0; i < V; ++i) {
            if (detectCycleUtil(i, visited, recStack)) {
                return true;
            }
        }
        return false;
    }

    void topologicalSortUtil(int v, vector<bool>& visited, vector<int>& Stack) {
        visited[v] = true;

        for (int i = 0; i < V; ++i) {
            if (Matrix[v][i] && !visited[i]) {
                topologicalSortUtil(i, visited, Stack);
            }
        }
        Stack.push_back(v);
    }

    void topologicalSort() {
        if (detectCycle()) {
            cout << "Граф содержит цикл.\n";
            return;
        }

        vector<int> Stack;
        vector<bool> visited(V, false);
        for (int i = 0; i < V; ++i) {
            if (!visited[i]) {
                topologicalSortUtil(i, visited, Stack);
            }
        }
        reverse(Stack.begin(), Stack.end());
        for (const auto& v : Stack) {
            cout << v + 1 << " ";
        }
        cout << endl;
    }

    void deikstra(int element) {
        element -= 1;
        vector<int> D(V, numeric_limits<int>::max());
        vector<bool> visited(V, false);
        D[element] = 0;

        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (Matrix[i][j] < 0) {
                    cout << "Граф содержит отрицательный вес дуги: " << i + 1 << " -> " << j + 1 << " = " << Matrix[i][j] << endl;
                    return;
                }
            }
        }

        for (int count = 0; count < V - 1; ++count) {
            int u = minDistance(D, visited);
            visited[u] = true;

            for (int v = 0; v < V; ++v) {
                if (!visited[v] && Matrix[u][v] && D[u] != numeric_limits<int>::max() && D[u] + Matrix[u][v] < D[v]) {
                    D[v] = D[u] + Matrix[u][v];
                }
            }
        }
        printSolution(D, element);
    }

    int minDistance(const vector<int>& dist, const vector<bool>& sptSet) {
        int min = numeric_limits<int>::max(), min_index = -1;
        for (int v = 0; v < V; ++v) {
            if (!sptSet[v] && dist[v] <= min) {
                min = dist[v], min_index = v;
            }
        }
        return min_index;
    }

    void printSolution(const vector<int>& dist, int element) {
        for (int i = 0; i < V; ++i) {
            if (dist[i] != numeric_limits<int>::max())
                cout << element + 1 << " -> " << i + 1 << " = " << dist[i] << endl;
            else
                cout << element + 1 << " -> " << i + 1 << " = маршрута нет\n";
        }
    }
};

int main() {
    Graph g("graph.txt");

    if (g.V == 0) {
        cout << "В графе нет вершин\n";
        return 1;
    }

    cout << "Граф:\n";
    g.print();

    cout << "\nТопологическая сортировка:\n";
    g.topologicalSort();

    cout << "\nАлгоритм Дейкстры:\n";
    g.deikstra(1);

    g.addVertex();
    cout << "\nГраф после добавления точки:\n";
    g.print();

    g.addEdge(6, 2, 10);
    cout << "\nГраф после добавления дуги (6 -> 2 с весом 10):\n";
    g.print();
    
    g.addEdge(6, 2, 10);
    cout << "\nГраф после добавления дуги (6 -> 2 с весом 10):\n";
    g.print();

    g.removeEdge(1, 4);
    cout << "\nГраф после удаления дуги (1 -> 4):\n";
    g.print();

    g.removeVertex(1);
    cout << "\nГраф после удаления точки 1:\n";
    g.print();

    return 0;
}
