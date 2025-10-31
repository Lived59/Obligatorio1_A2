#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/MaxHeap.cpp"

using namespace std;

struct Pozo
{
    int inicio;
    int final;
};

struct Mejora
{
    int posicion;
    int valor;
};

int main()
{
    int N;
    cin >> N;
    int M;
    cin >> M;
    int F;
    cin >> F;
    Pozo *pozos = new Pozo[N];
    Mejora *mejoras = new Mejora[M];
    for (int i = 0; i < N; i++)
    {
        cin >> pozos[i].inicio >> pozos[i].final;
    }
    for (int i = 0; i < M; i++)
    {
        cin >> mejoras[i].posicion >> mejoras[i].valor;
    }

    int pos = 1;
    int potencia = 1;
    int mejorasUsadas = 0;
    int indiceMejora = 0;
    int indicePozo = 0;
    MaxHeap heap(M);

    while (pos < F)
    {
        // Si el próximo pozo empieza en pos + 1
        //Me fijo si sigue habiendo pozos (índicePozo < N) 
        if (indicePozo < N && pozos[indicePozo].inicio == pos + 1)
        {
            int largoPozo = pozos[indicePozo].final - pozos[indicePozo].inicio + 1;

            // Agregar al heap todas las mejoras que se encuentren antes o en la entrada del pozo
            while (indiceMejora < M && mejoras[indiceMejora].posicion <= pos)
            {
                heap.insertar(mejoras[indiceMejora].valor);
                indiceMejora++;
            }

            // Tomar mejoras hasta poder cruzar el pozo
            while (potencia < largoPozo + 1)
            {
                if (heap.estaVacio())
                {
                    cout << "Imposible" << endl;
                    return 0;
                }
                potencia += heap.tope();
                heap.eliminar();
                mejorasUsadas++;
            }

            // Cruzar pozo
            pos = pozos[indicePozo].final + 1;
            indicePozo++;
        }
        // No hay pozo, avanzar 1 a 1
        else
        {
            pos++;
        }
    }

    cout << mejorasUsadas << endl;

    delete[] pozos;
    delete[] mejoras;
    return 0;
}