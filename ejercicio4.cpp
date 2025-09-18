#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/MinHeap.cpp"

using namespace std;

int main()
{
    int k;
    cin >> k;
    int *tamanios = new int[k];
    int **listas = new int *[k];

    for (int i = 0; i < k; i++)
    {
        int m;
        cin >> m;
        tamanios[i] = m;
        listas[i] = new int[m];
        for (int j = 0; j < m; j++)
        {
            cin >> listas[i][j];
        }
    }

    MinHeap *res = new MinHeap(k);
    for (int i = 0; i < k; i++)
    {
        if (tamanios[i] > 0)
        {
            elem e;
            e.valor = listas[i][0];
            e.lista = i;
            e.indice = 0;
            res->insertar(e);
        }
    }

    while (!res->estaVacio())
    {
        elem minimo = res->tope();
        res->eliminar();
        cout << minimo.valor << endl;

        int listaActual = minimo.lista;
        int indiceSig = minimo.indice + 1;
        if (indiceSig < tamanios[listaActual])
        {
            elem e;
            e.valor = listas[listaActual][indiceSig];
            e.lista = listaActual;
            e.indice = indiceSig;
            res->insertar(e);
        }
    }
    return 0;
}
