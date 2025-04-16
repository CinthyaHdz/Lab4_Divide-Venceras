#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

typedef vector<vector<int>> Matriz;

Matriz generarMatriz(int n) {
    Matriz matriz(n, vector<int>(n));
    for (int fila = 0; fila < n; ++fila)
        for (int columna = 0; columna < n; ++columna)
            matriz[fila][columna] = rand() % 10;
    return matriz;
}

Matriz multiplicarTradicional(const Matriz& A, const Matriz& B) {
    int n = A.size();
    Matriz C(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

void probarMultiplicacionTradicional(int n) {
    Matriz A = generarMatriz(n);
    Matriz B = generarMatriz(n);

    auto inicio = high_resolution_clock::now();
    Matriz C = multiplicarTradicional(A, B);
    auto fin = high_resolution_clock::now();

    duration<double, milli> duracion = fin - inicio;
    cout << "Tamano n = " << n << ", Tiempo de ejecucion (ms): " << duracion.count() << endl;
}

int main() {
    vector<int> tamanos = {2, 4, 8, 16, 32, 64, 128, 256, 512};

    for (int n : tamanos) {
        probarMultiplicacionTradicional(n);
    }

    return 0;
}
