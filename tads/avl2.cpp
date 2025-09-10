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

    void InsertarAux(NodoAVL *&nodo, int dato)
    {
        if (!nodo)
        {
            nodo = new NodoAVL();
            nodo->dato = dato;
            nodo->izq = nodo->der = NULL;
            nodo->altura = 1;
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

        // Verificar balance
        int balance = calcularBalance(nodo);
        bool desbalanceDer = balance < -1;
        bool desbalanceIzq = balance > 1;

        // Desbalance izquierda-izquierda
        if (desbalanceIzq && nodo->izq->dato > dato)
        {
            // rotacion derecha
            rotacionHoraria(nodo);
        }

        // Desbalance izquierda-derecha
        if (desbalanceIzq && nodo->izq->dato < dato)
        {
            // rotacion izquierda en Y
            // rotacion derecha en Z
            rotacionAntiHoraria(nodo->izq);
            rotacionHoraria(nodo);
        }

        // Desbalance derecha-derecha
        if (desbalanceDer && nodo->der->dato < dato)
        {
            // rotacion izquierda
            rotacionAntiHoraria(nodo);
        }

        // Desbalance derecha-izquierda
        if (desbalanceDer && nodo->der->dato > dato)
        {
            // rotacion derecha en Y
            // rotacion izquierda en Z
            rotacionHoraria(nodo->der);
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
            return 1 + rankAux(nodo->izq, puntaje) + rankAux(nodo->der, puntaje);
        }
        else
            return rankAux(nodo->izq, puntaje) + rankAux(nodo->der, puntaje);
   
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
};
