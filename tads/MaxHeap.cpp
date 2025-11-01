#include <cassert>
#include <string>
#include <iostream>
#include <limits>

class MaxHeap
{
private:
    int *vec;
    int capacidad;
    int primeroLibre;

    bool estaLleno()
    {
        return this->primeroLibre > this->capacidad;
    }
    

    int padre(int pos)
    {
        return pos / 2;
    }

    int funcionComparadora(int a, int b)
    {
        return b - a;
    }

    void intercambiar(int posPadre, int pos)
    {
        int aux = vec[posPadre];
        vec[posPadre] = vec[pos];
        vec[pos] = aux;
    }

    void flotar(int pos)
    {
        if (pos > 1)
        {
            int posPadre = padre(pos);
            if (funcionComparadora(vec[posPadre], vec[pos]) > 0)
            {
                intercambiar(posPadre, pos);
                flotar(posPadre);
            }
        }
    }

    int izq(int pos)
    {
        return pos * 2;
    }

    int der(int pos)
    {
        return pos * 2 + 1;
    }

    void hundir(int pos)
    {
        int posHijoIzq = izq(pos);
        int posHijoDer = der(pos);

        if (posHijoIzq < this->primeroLibre && posHijoDer < this->primeroLibre)
        {
            int posHijoMayor = funcionComparadora(vec[posHijoIzq], vec[posHijoDer]) < 0 ? posHijoIzq : posHijoDer;
            if (funcionComparadora(vec[pos], vec[posHijoMayor]) > 0)
            {
                intercambiar(pos, posHijoMayor);
                hundir(posHijoMayor);
            }
        }
        else if (posHijoIzq < this->primeroLibre)
        {
            if (funcionComparadora(vec[pos], vec[posHijoIzq]) > 0)
            {
                intercambiar(pos, posHijoIzq);
                hundir(posHijoIzq);
            }
        }
    }

public:
    MaxHeap(int capacidad)
    {
        this->vec = new int[capacidad + 1];
        this->capacidad = capacidad;
        this->primeroLibre = 1;
    }

    ~MaxHeap()
    {
        delete[] this->vec;
    }

    void insertar(int dato)
    {
        assert(!this->estaLleno());
        if (!this->estaLleno())
        {
            this->vec[this->primeroLibre] = dato;
            flotar(this->primeroLibre);
            this->primeroLibre++;
        }
    }

    void eliminar()
    {
        assert(!estaVacio());
        this->vec[1] = this->vec[primeroLibre - 1];
        this->primeroLibre--;
        hundir(1);
    }

    int tope()
    {
        assert(this->primeroLibre > 1);
        return this->vec[1];
    }

    bool estaVacio()
    {
        return this->primeroLibre == 1;
    }
};