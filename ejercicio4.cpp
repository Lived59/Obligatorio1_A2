#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "tads/MinHeap.cpp"

using namespace std;

/*Implementar un algoritmo eficiente para fusionar 
//K listas ordenadas de forma creciente en una sola lista ordenada, utilizando un min heap.

Cada lista está ordenada de forma creciente 
y el resultado final debe ser una lista única que contenga 
todos los elementos de las K listas, también ordenada de forma creciente.*/

int main()
{
    // Cantidad de listas
    int k;
    cin >> k;

    // Creo un min Heap que sera el que luego retornare
    MinHeap * res = new MinHeap(k * 100); // Capacidad maxima k * 100

    for(int i = 0; i < k; i++){

        // Cantidad de elementos que tendra la lista
        int m;
        cin >> m;

        // Ingreso elementos a la lista 
        for(int j = 0; j < m; j++){
            int x;
            cin >> x;
            res -> insertar(x);
        }
    }


    while(!res->estaVacio()){
        cout << res -> tope() << endl;
        res -> eliminar();
    }
    return 0;
}