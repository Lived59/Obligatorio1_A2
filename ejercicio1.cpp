#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "./tads/avl.cpp"
#include "./tads/avl2.cpp"

using namespace std;

void add(AVL *avl, int id, string nombre, int puntaje)
{
    
    avl->insertar(id, nombre, puntaje);
}

string find(AVL *avl, int id)
{
    if (!avl->buscar(id))
    {
        return "jugador_no_encontrado";
    }
    else
    {
        return avl->obtenerNombre(id) + " " + to_string(avl->obtenerPuntaje(id));
    }
}

int main()
{
    AVL *JugadoresAVL = new AVL();
    AVL2 *rankAVL = new AVL2(); // Para rank
    int N;
    cin >> N;
    string *resultado = new string[N];
    for (int i = 0; i < N; i++)
    {
        string op;
        cin >> op;
        if (op == "ADD")
        {
            int id;
            cin >> id;
            string nombre;
            cin >> nombre;
            int puntaje;
            cin >> puntaje;
            resultado[i] = "";
            int cantAntes = JugadoresAVL->size();
            add(JugadoresAVL, id, nombre, puntaje);
            if (JugadoresAVL->size() > cantAntes)
            {
                rankAVL->insertar(puntaje); // Para rank
            }
        }
        if (op == "FIND")
        {
            int id;
            cin >> id;
            resultado[i] = find(JugadoresAVL, id);
        }
        if (op == "RANK")
        {
            int puntaje;
            cin >> puntaje;
            resultado[i]= to_string(rankAVL->rank(puntaje));
            
        }
        if (op == "TOP1")
        {
            resultado[i] = JugadoresAVL->obtenerTop1();
        }
        if (op == "COUNT")
        {
            resultado[i] = to_string(JugadoresAVL->size());
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (resultado[i] != "")
        {
            cout << resultado[i] << endl;
        }
    }
    delete[] resultado;
    delete JugadoresAVL;
    delete rankAVL;

    return 0;
}