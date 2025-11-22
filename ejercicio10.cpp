#include <string>
#include <iostream>

using namespace std;

bool esValida(int fila, int col, char** fc, int** distMinima, int pasos, int M, int N, char producto) {
    if (fila < 0 || fila >= M || col < 0 || col >= N) {
        return false;
    }
    // Corredor o el producto buscado
    if (fc[fila][col] != 'C' && fc[fila][col] != producto) {
        return false;
    }
    // Si ya llegamos a esta posición con menos pasos termino asi dejo de buscae
    if (distMinima[fila][col] <= pasos) {
        return false;
    }
    return true;
}

bool esSolucion(int fila, int col, char** fc, char producto) {
    return fc[fila][col] == producto;
}

void backtracking(int fila, int col, char** fc, int** distMinima, int pasos, int M, int N, char producto, int& minPasos) {
    // Terminar si ya hay una solucion con menos pasos
    if (pasos >= minPasos) {
        return;
    }

    if (distMinima[fila][col] > pasos) {
        distMinima[fila][col] = pasos;
    } else {
        // Si ya llegamos aca con menos pasos termino
        return;
    }
     
    if (esSolucion(fila, col, fc, producto)) {
        if (pasos < minPasos) {
            // actualizamos el mínimo
            minPasos = pasos;
        }
        return;
    }
    
    // Guardo movimientos posibles en una matriz
    int movimientos[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
    for (int i = 0; i < 4; i++) {
        int nuevaFila = fila + movimientos[i][0];
        int nuevaCol = col + movimientos[i][1];
        
        if (esValida(nuevaFila, nuevaCol, fc, distMinima, pasos + 1, M, N, producto)) {
            backtracking(nuevaFila, nuevaCol, fc, distMinima, pasos + 1, M, N, producto, minPasos);
        }
    }
}

int encontrarProducto(char** fc, int m, int n, char producto) {
    int minPasos = 10001;
    
    // Producto en el principio
    if (fc[0][0] == producto) {
        return 0;
    }
    
    // Crear matriz de distancias mínimas conocidas
    int** distMinima = new int*[m];
    for (int i = 0; i < m; i++) {
        distMinima[i] = new int[n];
        for (int j = 0; j < n; j++) {
            distMinima[i][j] = 10001;
        }
    }
    
    // El primer paso se cuenta al moverse a otra celda 
    backtracking(0, 0, fc, distMinima, 0, m, n, producto, minPasos);
    
    // Liberar memoria
    for (int i = 0; i < m; i++) {
        delete[] distMinima[i];
    }
    delete[] distMinima;
    
    return minPasos;
}

int main() {
    char producto;
    cin >> producto;
    
    int P;
    cin >> P;
    
    // Vector de matrices, Matrices de fullfilment centers
    char*** fcs = new char**[P+1];

    // Para liberar memoria, ver si podemos evitarlo
    int** dimensiones = new int*[P+1];

    // Pasos minimos en cada FC para encontrar el producto (arrancamos en 1)
    int* pasos = new int[P+1];
    
    for (int i = 1; i <= P; i++) {
        int m, n;
        cin >> m >> n;
        dimensiones[i] = new int[2];
        dimensiones[i][0] = m;
        dimensiones[i][1] = n;
        
        // Crear matriz para este FC
        fcs[i] = new char*[m];
        for (int fila = 0; fila < m; fila++) {
            fcs[i][fila] = new char[n];
            for (int col = 0; col < n; col++) {
                cin >> fcs[i][fila][col];
            }
        }
        
        // Encontrar el producto en este FC (Me guardo solo la menor cantidad de pasos)
        pasos[i] = encontrarProducto(fcs[i], m, n, producto);
    }
    
    // Encontrar el FC con menor cantidad de pasos
    int mejorFC = 1;
    int menorPasos = pasos[1];
    
    for (int i = 2; i <= P; i++) {
        if (pasos[i] < menorPasos) {
            menorPasos = pasos[i];
            mejorFC = i;
        }
    }
    
    cout << mejorFC << " " << menorPasos << endl;
    
    
    // Liberar memoria
    for (int i = 1; i <= P; i++) {
        for (int fila = 0; fila < dimensiones[i][0]; fila++) {
            delete[] fcs[i][fila];
        }
        delete[] fcs[i];
        delete[] dimensiones[i];
    }
    delete[] fcs;
    delete[] dimensiones;
    delete[] pasos;
    
    return 0;
}
