#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

int * mergeSort(int * valores, int inicio, int fin){
    if(inicio == fin){
        int * ret = new int[1];
        ret[0] = valores[inicio];
        return ret;
    }
    int medio = (inicio + fin)/2;

    int *ord1 = mergeSort(valores, inicio, medio);
    int *ord2 = mergeSort(valores, medio + 1, fin);

    //return intercalar(ord1, ord2);
}
int main()
{
    int N;
    cin >> N;
    string *profesor = new string[N];
    string *ayudante = new string[N];
    for (int i = 0; i < N; i++) cin >> profesor[i];
    for (int i = 0; i < N; i++) cin >> ayudante[i];


    //Tabla de Hash
    //Merge-sort contando inversiones
    
    return 0;
}