#include <iostream>
#include "definiciones.cpp"
using namespace std;

class GrafoLista {
	private:
		Arista ** vertices;
		int cantidadV;
		bool dirigido;
		bool ponderado;
		int cantidadA;

	public:
		GrafoLista(int cantV, bool esDirigido, bool esPonderado) {
			vertices = new Arista*[cantV + 1]();
			cantidadV = cantV;
			dirigido = esDirigido;
			ponderado = esPonderado;
			cantidadA = 0;
		}

		~GrafoLista() {
			delete[] vertices;
		}

		void agregarArista(int origen, int destino, int peso = 1) {
			Arista * nuevaArista = new Arista();
			nuevaArista->destino = destino;
			nuevaArista->peso = peso;
			nuevaArista->origen = origen;
			nuevaArista->sig = vertices[origen];
			vertices[origen] = nuevaArista;
			cantidadA++;
			if (!dirigido) {
				Arista * inversa = new Arista();
				inversa->destino = origen;
				inversa->peso = peso;
				inversa->origen = destino;
				inversa->sig = vertices[destino];
				vertices[destino] = inversa;
				cantidadA++;
			}
		}

		void imprimir() {
			cout << endl << "Lista de adyacencia:" << endl;
			for (int i = 1; i <= cantidadV; i++) {
				cout << i << ": ";
				Arista * ady = adyacentes(i);
				while (ady) {
					cout << ady->destino << " ";
					if (ponderado) {
						cout << "(" << ady->peso << ") ";
					}
					if(ady->sig) {
						cout << "-> ";
					}
					ady = ady->sig;
				}
				cout << endl;
			}
		}

		Arista* adyacentes(int vertice) {
			return vertices[vertice];
		}

		int cantidadVertices() {
			return cantidadV;
		}

		int cantidadAristas() {
			return cantidadA;
		}
};