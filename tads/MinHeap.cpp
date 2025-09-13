#include <cassert>
#include <string>
#include <iostream>
#include <limits>

class MinHeap
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
        return a - b;
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
        // obtener pos de los 2 hijos
        int posHijoIzq = izq(pos);
        int posHijoDer = der(pos);

        // verificar que hijo vive en el heap
        if (posHijoIzq < this->primeroLibre && posHijoDer < this->primeroLibre)
        {
            // 2 hijos
            int posHijoMenor = funcionComparadora(vec[posHijoIzq], vec[posHijoDer]) > 0 ? posHijoDer : posHijoIzq;
            if (funcionComparadora(vec[pos], vec[posHijoMenor]) > 0)
            {
                intercambiar(pos, posHijoMenor);
                hundir(posHijoMenor);
            }
        }
        else
        {
            if (posHijoIzq < this->primeroLibre)
            {
                // Solo hijo izq
                if (funcionComparadora(vec[pos], vec[posHijoIzq]) > 0)
                {
                    intercambiar(pos, posHijoIzq);
                    hundir(posHijoIzq);
                }
            }
        }

        // CB No tengo ningun hijo, no hago nada, es hoja
    }

public:
    MinHeap(int capacidad)
    {
        this->vec = new int[capacidad + 1];
        this->capacidad = capacidad;
        this->primeroLibre = 1;
    }

    ~MinHeap()
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