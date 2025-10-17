#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/grafoListaAdy.cpp"
#include "tads/MFset.cpp"
#include "tads/MinHeap.cpp"

using namespace std;

GrafoLista *kruskal(GrafoLista *g)
{
    int V = g->cantidadVertices();
    int E = g->cantidadAristas();
    GrafoLista *ACM = new GrafoLista(V, false, true);
    MFSet *mfSet = new MFSet(V + 1);
    MinHeap cp(E);

    for (int i = 0; i < V; i++)
    {
        Arista *ady = g->adyacentes(i);
        while (ady)
        {
            elem e;
            e.valor = ady->peso;
            e.lista = ady->origen;
            e.indice = ady->destino;
            cp.insertar(e);
            ady = ady->sig;
        }
    }

    while (!cp.estaVacio())
    {
        elem a = cp.tope();
        cp.eliminar();
        if (mfSet->find(a.lista) != mfSet->find(a.indice))
        {
            mfSet->merge(a.lista, a.indice);

            ACM->agregarArista(a.lista, a.indice, a.valor);
        }
    }

    return ACM;
}

int main()
{
    int V, E;
    cin >> V >> E;
    GrafoLista *g = new GrafoLista(V, false, true);
    for (int i = 0; i < E; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        g->agregarArista(u, v, w);
    }

    GrafoLista *ACM = kruskal(g);

    int pesoTotal = 0;
    for (int u = 0; u < V; u++)
    {
        Arista *ady = ACM->adyacentes(u);
        while (ady)
        {
            if (u < ady->destino)
                pesoTotal += ady->peso;
            ady = ady->sig;
        }
    }

    cout << pesoTotal << endl;

    return 0;
}