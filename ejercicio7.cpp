#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/tabla_de_hash_abierto"

using namespace std;

bool esSolucionTrivial(int inicio, int fin) {
    return inicio == fin;
}

long long resolverSolucionTrivial(int* arr, int inicio, int* temp) {
    temp[inicio] = arr[inicio];
    return 0;
}

long long merge(int* arr, int* temp, int inicio, int medio, int fin) {
    int i = inicio;
    int j = medio + 1;
    int k = inicio;

    long long inv = 0;

    while (i <= medio && j <= fin) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
            inv += (medio - i + 1);
        }
    }

    while (i <= medio) temp[k++] = arr[i++];
    while (j <= fin) temp[k++] = arr[j++];

    for (int x = inicio; x <= fin; x++)
        arr[x] = temp[x];

    return inv;
}

long long mergeSortCount(int* arr, int* temp, int inicio, int fin) {

    if (esSolucionTrivial(inicio, fin)) {
        return resolverSolucionTrivial(arr, inicio, temp);
    }

    int medio = (inicio + fin) / 2;

    long long solIzq = mergeSortCount(arr, temp, inicio, medio);
    long long solDer = mergeSortCount(arr, temp, medio + 1, fin);

    long long solMerge = merge(arr, temp, inicio, medio, fin);

    return solIzq + solDer + solMerge;
}

int main()
{
    int N;
    cin >> N;

    string *profesor = new string[N];
    string *ayudante = new string[N];

    for (int i = 0; i < N; i++)cin >> profesor[i];
    for (int i = 0; i < N; i++)cin >> ayudante[i];

    HashAbierto tabla(2 * N + 1);
    for (int i = 0; i < N; i++)tabla.insertar(profesor[i], i);

    int *posiciones = new int[N];
    for (int i = 0; i < N; i++)
        posiciones[i] = tabla.buscar(ayudante[i]);

    int *temp = new int[N];
    long long inversiones = mergeSortCount(posiciones, temp, 0, N - 1);
    cout << inversiones << endl;

    delete[] profesor;
    delete[] ayudante;
    delete[] posiciones;
    delete[] temp;

    return 0;
}