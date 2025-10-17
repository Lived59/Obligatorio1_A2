#include <iostream>
using namespace std;
class MFSet
{
	int *altura, *representante, n;

public:
	MFSet(int n)
	{
		representante = new int[n];
		altura = new int[n];
		this->n = n;
		armarConjuntos();
	}

	void armarConjuntos()
	{
		for (int i = 0; i < n; i++)
		{
			representante[i] = i;
		}
	}

	int find(int x)
	{
		if (representante[x] != x)
		{
			// aplanamos el arbol
			representante[x] = find(representante[x]);
		}
		return representante[x];
	}

	void merge(int x, int y)
	{
		int xset = find(x);
		int yset = find(y);

		// si estan dentro del mismo set terminamos
		if (xset == yset)
			return;

		// evitamos aumentar la altura del arbol
		if (altura[xset] < altura[yset])
		{
			representante[xset] = yset;
		}
		else if (altura[xset] > altura[yset])
		{
			representante[yset] = xset;
		}

		// si ambas alturas con la misma usamos cualquiera y actualizamos la altura
		else
		{
			representante[yset] = xset;
			altura[xset] = altura[xset] + 1;
		}
	}
};

// int main()
// {
// 	MFSet* mfset = new MFSet(5);
// 	mfset->merge(0, 2);
// 	mfset->merge(4, 2);
// 	mfset->merge(3, 1);
// 	if (mfset->find(4) == mfset->find(0))
// 		cout << "Si\n";
// 	else
// 		cout << "No\n";

// 	if (mfset->find(1) == mfset->find(0))
// 		cout << "Si\n";
// 	else
// 		cout << "No\n";

// 	return 0;
// }
// fuente: https://www.geeksforgeeks.org/disjoint-set-data-structures/