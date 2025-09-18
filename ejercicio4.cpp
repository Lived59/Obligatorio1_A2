#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/MinHeap.cpp"

using namespace std;

int *tamanios;
int **listas;

bool haySiguienteEnLista(elem actual, elem &siguiente)
{
    int listaActual = actual.lista;
    int indiceSig = actual.indice + 1;
    if (indiceSig  < tamanios[listaActual])
    {
        siguiente.valor = listas[listaActual][indiceSig];
        siguiente.lista = listaActual;
        siguiente.indice = indiceSig;
        return true;
    }
    return false;
}

int main()
{
    int k;
    cin >> k;
    tamanios = new int[k];
    listas = new int *[k];

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
        elem sig;
        res->eliminar();
        cout << minimo.valor << endl;
        if (haySiguienteEnLista(minimo, sig))
        {
            res->insertar(sig);
        }
    }
    return 0;
}
