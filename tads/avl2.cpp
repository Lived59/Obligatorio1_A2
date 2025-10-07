#include <cassert>
#include <string>
#include <iostream>
#include <limits>
using namespace std;

class AVL2
{
private:
    struct NodoAVL
    {
        int dato;
        NodoAVL *izq;
        NodoAVL *der;
        int altura;
        int size;
    };
    NodoAVL *raiz;

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
        B->size = 1 + getSize(B->izq) + getSize(B->der);
        A->altura = 1 + max(getAltura(A->izq), getAltura(A->der));
        A->size = 1 + getSize(A->izq) + getSize(A->der);
        B = A;
    }

    void rotacionAntiHoraria(NodoAVL *&A)
    {
        NodoAVL *B = A->der;
        NodoAVL *T2 = B->izq;
        A->der = T2;
        B->izq = A;
        A->altura = 1 + max(getAltura(A->izq), getAltura(A->der));
        A->size = 1 + getSize(A->izq) + getSize(A->der);
        B->altura = 1 + max(getAltura(B->izq), getAltura(B->der));
        B->size = 1 + getSize(B->izq) + getSize(B->der);
        A = B;
    }

    void InsertarAux(NodoAVL *&nodo, int dato)
    {
        if (!nodo)
        {
            nodo = new NodoAVL();
            nodo->dato = dato;
            nodo->izq = nodo->der = NULL;
            nodo->altura = 1;
            nodo->size = 1;
            return;
        }
        if (nodo->dato < dato)
        {
            InsertarAux(nodo->der, dato);
        }
        else
        {
            InsertarAux(nodo->izq, dato);
        }

        // Calcular altura
        nodo->altura = 1 + max(getAltura(nodo->izq), getAltura(nodo->der));
        nodo->size = 1 + getSize(nodo->izq) + getSize(nodo->der);
        // Verificar balance
        int balance = calcularBalance(nodo);
        
        if (balance > 1)
        {
            if (calcularBalance(nodo->izq) < 0)
            {
                rotacionAntiHoraria(nodo->izq);
            }
            rotacionHoraria(nodo);
        }
        else if (balance < -1)
        {
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

    void inOrderAux(NodoAVL *nodo)
    {
        if (nodo)
        {
            inOrderAux(nodo->izq);
            cout << nodo->dato << endl;
            inOrderAux(nodo->der);
        }
    }

    int rankAux(NodoAVL *nodo, int puntaje)
    {
        if (!nodo)
            return 0;
        if (nodo->dato >= puntaje)
        {
            return 1 + getSize(nodo->der) + rankAux(nodo->izq, puntaje);
        }
        else
        {
            return rankAux(nodo->der, puntaje);
        }
    }

public:
    AVL2()
    {
        raiz = NULL;
    };
    ~AVL2()
    {
        destruir(raiz);
    };
    void insertar(int dato)
    {
        InsertarAux(raiz, dato);
    }

    int rank(int puntaje)
    {
        return rankAux(raiz, puntaje);
    }
    int getSize(NodoAVL *nodo)
    {
        if (!nodo)
            return 0;
        return nodo->size;
    }
};
