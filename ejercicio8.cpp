#include <iostream>

using namespace std;

// Variables globales
int *** pd; // inicializada a -1 en todas sus posiciones para indicar que el estado no ha sido calculado
int * fragmentos; // arreglo que contiene los colores de los fragmentos
int N; // cantidad de fragmentos
int maxAcPrevios; // máximo valor de acPrevios (para limitar la tercera dimensión de pd)

int max(int a, int b) {
    return a > b ? a : b;
}

int puntajeMaximo(int inicio, int fin, int acPrevios){
    // Casos base
    
    if(inicio > fin) return 0;
    
    // Asegurar que acPrevios esté dentro de los límites válidos
    if(acPrevios >= maxAcPrevios) {
        acPrevios = maxAcPrevios - 1;
    }
    
    if(pd[inicio][fin][acPrevios] != -1) {
        return pd[inicio][fin][acPrevios];
    }

    if(inicio == fin){
        return pd[inicio][fin][acPrevios] = (acPrevios + 1) * (acPrevios + 1);
    }

    int mejorPuntaje = 0;
    // Opcion de borrarlo
    int puntajeBorrarlo = (acPrevios + 1) * (acPrevios + 1) + puntajeMaximo(inicio + 1, fin, 0);
    mejorPuntaje = puntajeBorrarlo;

    // Opcion 2: No borrar ahora, sino unir con otro igual mas adelante
    // Buscamos mas adelante posiciones con el mismo color
    for(int indiceMismoColor = inicio + 1; indiceMismoColor <= fin; indiceMismoColor++){
        // si encontramos otro fragmento del mismo color:
        // a) primero eliminamos lo que hay en el medio (inicio+1 ... indiceMismoColor-1)
        // b) luego unimos inicio con indiceMismoColor (ahora con acPrevios+1)
        if(fragmentos[indiceMismoColor] == fragmentos[inicio]){
            int nextAcPrevios = (acPrevios + 1 < maxAcPrevios) ? acPrevios + 1 : maxAcPrevios - 1;
            int puntajeNoBorrarlo = puntajeMaximo(inicio + 1, indiceMismoColor - 1, 0)
                                    + puntajeMaximo(indiceMismoColor, fin, nextAcPrevios);
            mejorPuntaje = max(mejorPuntaje, puntajeNoBorrarlo);
        }
    }

    return pd[inicio][fin][acPrevios] = mejorPuntaje;
}

int main()
{
    cin >> N;
    
    fragmentos = new int[N];
    for (int i = 0; i < N; i++) {
        cin >> fragmentos[i];
    }
    
    // Determinar el máximo valor de acPrevios
    // En el peor caso, acPrevios puede ser hasta N, pero para optimizar memoria
    // limitamos a 101 si N es grande (ya que acPrevios rara vez excede mucho)
    maxAcPrevios = N;
    if(N > 100) {
        maxAcPrevios = 101;
    }
    
    // Crear e inicializar la tabla pd[inicio][fin][acPrevios]
    pd = new int**[N];
    for (int inicio = 0; inicio < N; inicio++) {
        pd[inicio] = new int*[N];
        for (int fin = 0; fin < N; fin++) {
            pd[inicio][fin] = new int[maxAcPrevios];
            for (int ac = 0; ac < maxAcPrevios; ac++) {
                pd[inicio][fin][ac] = -1;
            }
        }
    }
    
    int resultado = puntajeMaximo(0, N - 1, 0);
    
    cout << resultado << endl;
    
    // Liberar memoria
    for (int inicio = 0; inicio < N; inicio++) {
        for (int fin = 0; fin < N; fin++) {
            delete[] pd[inicio][fin];
        }
        delete[] pd[inicio];
    }
    delete[] pd;
    delete[] fragmentos;
    
    return 0;
}
