#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

int ***pd; // Inicializado en -1
int max(int a, int b);

int puntajeMaximo(int inicio, int fin, int acPrevios, int * fragmentos)
{
    // Casos base
    if (inicio > fin)
        return 0;
    if (pd[inicio][fin][acPrevios] != -1)
    {
        return pd[inicio][fin][acPrevios];
    }
    if(inicio == fin){
        return pd[inicio][fin][acPrevios] = (acPrevios + 1) * (acPrevios + 1);
    }

    int mejorPuntaje = 0;
    //Opcion de borrarlo
    int puntajeBorrarlo = (acPrevios + 1) * (acPrevios + 1) + puntajeMaximo(inicio + 1, fin, 0);
    mejorPuntaje = puntajeBorrarlo;
    //Opcion de no borrarlo

    for(int indiceMismoColor = inicio + 1; indiceMismoColor <= fin; indiceMismoColor++){
        //Si encuentro otro fragmento del mismo color:
        //  a) Primero eliminamos lo que hay en el medio [inicio+1...indiceMismoColor-1]
        //  b) Luego unimos inicio con indiceMismoColor (ahora con acPrevios+1)
        if(fragmentos[indiceMismoColor] == fragmentos[inicio]){
           int puntajeNoBorrarlo = puntajeMaximo(inicio+1, indiceMismoColor-1, 0) + puntajeMaximo(indiceMismoColor, fin, acPrevios + 1);
           mejorPuntaje = max(mejorPuntaje, puntajeNoBorrarlo);
        }
    }
    return pd[inicio][fin][acPrevios] = mejorPuntaje;
}

int main()
{
    int cantFragmentos;
    cin >> cantFragmentos;
    int * fragmentos = new int[cantFragmentos];
    for(int i = 0; i < cantFragmentos; i++){
        cin >> fragmentos[i];
    }
    cout << puntajeMaximo(0, cantFragmentos, 0, pd);
    return 0;
}