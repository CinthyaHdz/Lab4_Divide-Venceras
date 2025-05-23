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
        for (int col = 0; col < n; ++col)
            matriz[fila][col] = rand() % 10;
    return matriz;
}

Matriz sumar(const Matriz& A, const Matriz& B) {
    int n = A.size();
    Matriz resultado(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            resultado[i][j] = A[i][j] + B[i][j];
    return resultado;
}

Matriz restar(const Matriz& A, const Matriz& B) {
    int n = A.size();
    Matriz resultado(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            resultado[i][j] = A[i][j] - B[i][j];
    return resultado;
}

void dividirMatriz(const Matriz& matriz, Matriz& A11, Matriz& A12, Matriz& A21, Matriz& A22) {
    int n = matriz.size() / 2;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            A11[i][j] = matriz[i][j];
            A12[i][j] = matriz[i][j + n];
            A21[i][j] = matriz[i + n][j];
            A22[i][j] = matriz[i + n][j + n];
        }
}

Matriz combinarMatriz(const Matriz& C11, const Matriz& C12, const Matriz& C21, const Matriz& C22) {
    int n = C11.size();
    Matriz resultado(n * 2, vector<int>(n * 2));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            resultado[i][j] = C11[i][j];
            resultado[i][j + n] = C12[i][j];
            resultado[i + n][j] = C21[i][j];
            resultado[i + n][j + n] = C22[i][j];
        }
    return resultado;
}

Matriz multiplicarStrassen(const Matriz& A, const Matriz& B) {
    int n = A.size();
    Matriz C(n, vector<int>(n, 0));

    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
    } else {
        int mitad = n / 2;

        Matriz A11(mitad, vector<int>(mitad)), A12(mitad, vector<int>(mitad)),
               A21(mitad, vector<int>(mitad)), A22(mitad, vector<int>(mitad));
        Matriz B11(mitad, vector<int>(mitad)), B12(mitad, vector<int>(mitad)),
               B21(mitad, vector<int>(mitad)), B22(mitad, vector<int>(mitad));

        dividirMatriz(A, A11, A12, A21, A22);
        dividirMatriz(B, B11, B12, B21, B22);

        Matriz M1 = multiplicarStrassen(sumar(A11, A22), sumar(B11, B22));
        Matriz M2 = multiplicarStrassen(sumar(A21, A22), B11);
        Matriz M3 = multiplicarStrassen(A11, restar(B12, B22));
        Matriz M4 = multiplicarStrassen(A22, restar(B21, B11));
        Matriz M5 = multiplicarStrassen(sumar(A11, A12), B22);
        Matriz M6 = multiplicarStrassen(restar(A21, A11), sumar(B11, B12));
        Matriz M7 = multiplicarStrassen(restar(A12, A22), sumar(B21, B22));

        Matriz C11 = sumar(restar(sumar(M1, M4), M5), M7);
        Matriz C12 = sumar(M3, M5);
        Matriz C21 = sumar(M2, M4);
        Matriz C22 = sumar(restar(sumar(M1, M3), M2), M6);

        C = combinarMatriz(C11, C12, C21, C22);
    }

    return C;
}

void probarMultiplicacionStrassen(int n) {
    Matriz A = generarMatriz(n);
    Matriz B = generarMatriz(n);

    auto inicio = high_resolution_clock::now();
    Matriz C = multiplicarStrassen(A, B);
    auto fin = high_resolution_clock::now();

    duration<double, milli> duracion = fin - inicio;
    cout << "Strassen - Tamano n = " << n << ", Tiempo de ejecucion (ms): " << duracion.count() << endl;
}

int main() {
    vector<int> tamanos = {2, 4, 8, 16, 32, 64, 128, 256};

    for (int n : tamanos) {
        probarMultiplicacionStrassen(n);
    }

    return 0;
}