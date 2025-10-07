#include <cassert>
#include <string>
#include <iostream>
#include <limits>
using namespace std;

class AVL
{
private:
    struct NodoAVL
    {
        int id;        // Esto es dato
        string nombre;
        int puntaje;
        NodoAVL *izq;
        NodoAVL *der;
        int altura;
    };
    NodoAVL *raiz;
    int cant = 0;
    int maxPuntaje = -1;
    string top1 = "sin_jugadores";
    int idTop1 = -1;

    int max(int a, int b)
    {
        return (a > b) ? a : b;
    }

    int getAltura(NodoAVL *nodo)
    {
        if (!nodo)
            return 0;
        return nodo->altura;
    }

    int calcularBalance(NodoAVL *nodo)
    {
        if (!nodo)
            return 0;
        return getAltura(nodo->izq) - getAltura(nodo->der);
    }

    void rotacionHoraria(NodoAVL *&B)
    {
        NodoAVL *A = B->izq;
        NodoAVL *T2 = A->der;
        B->izq = T2;
        A->der = B;
        B->altura = 1 + max(getAltura(B->izq), getAltura(B->der));
        A->altura = 1 + max(getAltura(A->izq), getAltura(A->der));
        B = A;
    }

    void rotacionAntiHoraria(NodoAVL *&A)
    {
        NodoAVL *B = A->der;
        NodoAVL *T2 = B->izq;
        A->der = T2;
        B->izq = A;
        A->altura = 1 + max(getAltura(A->izq), getAltura(A->der));
        B->altura = 1 + max(getAltura(B->izq), getAltura(B->der));
        A = B;
    }

    void InsertarAux(NodoAVL *&nodo, int id, string nombre, int puntaje)
    {
        if (!nodo)
        {
            nodo = new NodoAVL();
            nodo->id = id;
            nodo->nombre = nombre;
            nodo->puntaje = puntaje;
            nodo->izq = nodo->der = NULL;
            nodo->altura = 1;
            cant++; 
            if (puntaje > maxPuntaje)
            {
                maxPuntaje = puntaje;
                top1 = nombre + " " + to_string(puntaje);
                idTop1 = id;
            }
            else if (puntaje == maxPuntaje && id < idTop1)
            {
                top1 = nombre + " " + to_string(puntaje);
                idTop1 = id;
            }
            return;
        }
        if (nodo->id < id)
        {
            InsertarAux(nodo->der, id, nombre, puntaje);
        }
        if (nodo->id > id)
        {
            InsertarAux(nodo->izq, id, nombre, puntaje);
        }
        if (nodo->id == id)
            return;

        // Calcular altura
        nodo->altura = 1 + max(getAltura(nodo->izq), getAltura(nodo->der));

        // Verificar balance
        int balance = calcularBalance(nodo);
        
        // Desbalance izquierda
        if (balance > 1)
        {
            // Caso LR: Si el hijo izquierdo está inclinado a la derecha, requiere rotación doble.
            if (calcularBalance(nodo->izq) < 0)
            {
                rotacionAntiHoraria(nodo->izq);
            }
            rotacionHoraria(nodo);
        }
        // Desbalance derecha
        else if (balance < -1)
        {
            // Caso RL: Si el hijo derecho está inclinado a la izquierda (balance > 0), requiere rotación doble.
            if (calcularBalance(nodo->der) > 0)
            {
                rotacionHoraria(nodo->der);
            }
            rotacionAntiHoraria(nodo);
        }
    }

    void destruir(NodoAVL *&nodo)
    {
        if (nodo)
        {
            destruir(nodo->izq);
            destruir(nodo->der);
            delete nodo;
            nodo = NULL;
        }
    }

    bool buscarAux(NodoAVL *nodo, int id)
    {
        if (!nodo)
            return false;
        if (nodo->id == id)
            return true;
        if (nodo->id < id)
            return buscarAux(nodo->der, id);
        if (nodo->id > id)
            return buscarAux(nodo->izq, id);
    }

    string nombreAux(NodoAVL *nodo, int id)
    {
        if (nodo->id == id)
            return nodo->nombre;
        if (nodo->id < id)
            return nombreAux(nodo->der, id);
        if (nodo->id > id)
            return nombreAux(nodo->izq, id);
    }

    int idAux(NodoAVL *nodo, int id)
    {
        if (nodo->id == id)
            return nodo->puntaje;
        if (nodo->id < id)
            return idAux(nodo->der, id);
        if (nodo->id > id)
            return idAux(nodo->izq, id);
    }

public:
    AVL()
    {
        raiz = NULL;
    }
    ~AVL()
    {
        destruir(raiz);
    }

    void insertar(int id, string nombre, int puntaje)
    {
        InsertarAux(raiz, id, nombre, puntaje);
    }

    int size()
    {
        return cant;
    }

    bool buscar(int id)
    {
        return buscarAux(raiz, id);
    }

    string obtenerNombre(int id)
    {
        return nombreAux(raiz, id);
    }

    int obtenerPuntaje(int id)
    {
        return idAux(raiz, id);
    }

    string obtenerTop1()
    {
        return top1;
    }
};
