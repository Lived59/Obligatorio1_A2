#ifndef DEFINICIONES
#define DEFINICIONES
struct Arista
{
	int origen;
	int destino;
	int peso;
	Arista *sig;
};
#endif
