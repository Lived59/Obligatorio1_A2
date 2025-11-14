#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/tabla_de_hash_abierto"

using namespace std;

int mergeSortCount(int* arr, int* temp, int inicio, int fin){
    return 0;
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
    int inversiones = mergeSortCount(posiciones, temp, 0, N - 1);
    cout << inversiones << endl;

    delete[] profesor;
    delete[] ayudante;
    delete[] posiciones;
    delete[] temp;

    return 0;
}