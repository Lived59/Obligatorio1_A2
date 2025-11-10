#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

int lastMomentPD(int N, int S, int L, int tamanio[], int lineas[], int puntos[]) {

    int ***mat = new int **[N + 1];
    for (int i = 0; i <= N; i++) {
        mat[i] = new int *[S + 1];
        for (int j = 0; j <= S; j++) {
            mat[i][j] = new int[L + 1];
            for (int k = 0; k <= L; k++) {
                mat[i][j][k] = 0;
            }
        }
    }

    for (int i = 1; i <= N; i++) {
        for (int s = 0; s <= S; s++) {
            for (int l = 0; l <= L; l++) {
                int tamActual = tamanio[i - 1];
                int linActual = lineas[i - 1];
                int punActual = puntos[i - 1];

                if (tamActual > s || linActual > l) {
                    mat[i][s][l] = mat[i - 1][s][l];
                } else {
                    int sinTomar = mat[i - 1][s][l];
                    int tomar = punActual + mat[i - 1][s - tamActual][l - linActual];
                    mat[i][s][l] = (tomar > sinTomar) ? tomar : sinTomar;
                }
            }
        }
    }

    int resultado = mat[N][S][L];

    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= S; j++) {
            delete[] mat[i][j];
        }
        delete[] mat[i];
    }
    delete[] mat;

    return resultado;
}

int main() {
    int N, S, L;
    cin >> N >> S >> L;

    int *tamanio = new int[N];
    int *lineas = new int[N];
    int *puntos = new int[N];

    for (int i = 0; i < N; i++) {
        cin >> tamanio[i] >> lineas[i] >> puntos[i];
    }

    cout << lastMomentPD(N, S, L, tamanio, lineas, puntos) << endl;

    delete[] tamanio;
    delete[] lineas;
    delete[] puntos;

    return 0;
}