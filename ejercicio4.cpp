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
    int **elementos = new int *[k];
    int *pos = new int[k];

    for (int i = 0; i < k; i++)
    {
        int m;
        cin >> m;
        tamanios[i] = m;
        elementos[i] = new int[m];
        pos[i] = 0;

        // Ingreso elementos a la lista
        for (int j = 0; j < m; j++)
        {
            int x;
            cin >> x;
            elementos[i][j] = x;
        }
    }


    MinHeap *res = new MinHeap(k);
    for (int i = 0; i < k; i++)
    {
        if(tamanios[i] > 0) {
            res->insertar(elementos[i][0]);
        }
    }

    while (!res->estaVacio())
    {
        int minimo = res->tope();
        res->eliminar();
        cout << minimo << endl;
        bool encontrado = false;

        for(int i = 0; i < k && !encontrado; i++){
            if(pos[i] < tamanios[i] && elementos[i][pos[i]] == minimo){
                pos[i]++;
                if(pos[i] < tamanios[i]){
                    res->insertar(elementos[i][pos[i]]);
                }
                encontrado = true;
            }
        }
    }
    return 0;
}
                

