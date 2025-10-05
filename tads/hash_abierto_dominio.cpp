#include <iostream>
#include <string>

using namespace std;

struct NodoRecursoDom {
    string path;
    string titulo;
    int tiempo;
    NodoRecursoDom* sig; 

    NodoRecursoDom(const string& p, const string& t, int ti)
        : path(p), titulo(t), tiempo(ti), sig(NULL) {}
};

struct NodoDominio {
    string dominio;
    NodoRecursoDom* recursos; 
    int cant; // Cantidad de recursos del dominio
    NodoDominio* sig;         

    NodoDominio(const string& d)
        : dominio(d), recursos(NULL), cant(0), sig(NULL) {}
};

class HashAbiertoDominio {
    private:
        NodoDominio** tabla; // Array de buckets de dominios
        int tamano; // Buckets
        int cantidadDominios; 

        float factorDeCarga(){
            return (float)cantidadDominios / tamano;
        }

        void rehash(){
            int tamanoNuevo = siguientePrimo(tamano * 2);
            NodoDominio** nueva = new NodoDominio*[tamanoNuevo];
            for (int i = 0; i < tamanoNuevo; i++){
                nueva[i] = NULL;
            }

            for (int i = 0; i < tamano; i++){
                NodoDominio* celda = tabla[i];
                while (celda != NULL){
                    NodoDominio* nxt = celda->sig;

                    int pos = normalizar(fnHash(celda->dominio), tamanoNuevo);
                    celda->sig = nueva[pos];
                    nueva[pos] = celda;

                    celda = nxt;
                }
                tabla[i] = NULL;
            }
            delete[] tabla;
            tabla = nueva;
            tamano = tamanoNuevo;
        }

        int fnHash(const string& dominio) {
            unsigned int sum = 0;
            for (int k = 0; k < (int)dominio.size(); k++){
                sum = sum * 31 + (unsigned char)dominio[k];
            }

            return (int)sum;
        }

        int normalizar(int hash, int tamano){
            int n = hash % tamano;
            if(n < 0){
                n += tamano;
            }
            return n;
        }

        int siguientePrimo(int num) {
            while (true) {
                bool esPrimo = true;
                for (int i = 2; i * i <= num; i++) {
                    if (num % i == 0) {
                        esPrimo = false;
                        break;
                    }
                }
                if (esPrimo) {
                    return num;
                }
                num++;
            }
        }

        void insertar(string dominio, string path, string titulo, int tiempo){
            if(factorDeCarga() > 0.7){
                this->rehash();
            }

            int pos = normalizar(fnHash(dominio), tamano);

            // Buscar el dominio
            NodoDominio* celda = tabla[pos];
            NodoDominio* dom = NULL;
            while (celda != NULL){
                if (celda->dominio == dominio){
                    dom = celda;
                    break;
                }
                celda = celda->sig;
            }

            if (dom == NULL){
                // Crear dominio y encadenar en cabeza del bucket
                dom = new NodoDominio(dominio);
                dom->sig = tabla[pos];
                tabla[pos] = dom;
                cantidadDominios++;
            }

            
            NodoRecursoDom* recursos = dom->recursos;
            NodoRecursoDom* recursosPrevios = NULL;
            while (recursos != NULL){
                if (recursos->path == path){
                    // Actualizar datos
                    recursos->titulo = titulo;
                    recursos->tiempo = tiempo;

                    // Mover a cabeza
                    if (recursosPrevios != NULL){
                        recursosPrevios->sig = recursos->sig;     
                        recursos->sig = dom->recursos;  
                        dom->recursos = recursos;
                    }
                    return;
                }
                recursosPrevios = recursos;
                recursos = recursos->sig;
            }

            // No existía el path
            NodoRecursoDom* nuevo = new NodoRecursoDom(path, titulo, tiempo);
            nuevo->sig = dom->recursos;
            dom->recursos = nuevo;
            dom->cant++;
        }

        void borrar(string dominio, const string& path){
            int pos = normalizar(fnHash(dominio), tamano);

            // Buscar el dominio con puntero al anterior
            NodoDominio* celda = tabla[pos];
            NodoDominio* prev = NULL;
            while (celda != NULL && celda->dominio != dominio){
                prev = celda;
                celda = celda->sig;
            }
            if (celda == NULL) return; // no existe el dominio

            // Buscar el recurso en la lista del dominio
            NodoRecursoDom* recursosCelda = celda->recursos;
            NodoRecursoDom* rprev = NULL;
            while (recursosCelda != NULL){
                if (recursosCelda->path == path){
                    // Desenganchar recurso
                    if (rprev == NULL) celda->recursos = recursosCelda->sig;
                    else rprev->sig = recursosCelda->sig;
                    delete recursosCelda;
                    celda->cant--;
                    break;
                }
                rprev = recursosCelda;
                recursosCelda = rprev->sig;
            }

            // Si el dominio quedó vacío, elimino el nodo dominio
            if (celda->cant == 0){
                if (prev == NULL){
                    tabla[pos] = celda->sig;
                } 
                else {
                    prev->sig = celda->sig;
                }
                liberarListaRecursos(celda->recursos);
                delete celda;
                cantidadDominios--;
            }
        }

        void liberarListaRecursos(NodoRecursoDom*& head) {
            while (head != NULL){
                NodoRecursoDom* nxt = head->sig;
                delete head;
                head = nxt;
            }
        }

    public:
        //Crea una tabla de hash vacia
        HashAbiertoDominio(unsigned int nInicial) {
            if (nInicial < 2){
                nInicial = 2;
            }
            tamano = siguientePrimo((int)nInicial);
            cantidadDominios = 0;
            tabla = new NodoDominio*[tamano];
            for (int i = 0; i < tamano; i++) {
                tabla[i] = NULL;
            }
        }

        ~HashAbiertoDominio() {
            clear();
            delete[] tabla;
            tabla = NULL;
            tamano = 0;
            cantidadDominios = 0;
        }

        //Funciones exclusivas de la clase
        int count_domain(string dominio){
            int pos = normalizar(fnHash(dominio), tamano);
            NodoDominio* celda = tabla[pos];
            while (celda != NULL){
                if (celda->dominio == dominio){
                    return celda->cant;
                } 
                celda = celda->sig;
            }
            return 0;
        }

        string list_domain(string dominio){
            int pos = normalizar(fnHash(dominio), tamano);
            NodoDominio* celda = tabla[pos];
            while (celda != NULL && celda->dominio != dominio){
                celda = celda->sig;
            } 

            if (celda == NULL || celda->cant == 0){
                return "";
            } 

            string res = "";
            NodoRecursoDom* r = celda->recursos; // más reciente primero
            bool first = true;
            while (r != NULL){
                if (!first){
                    res += " ";
                } 
                res += r->path;
                first = false;
                r = r->sig;
            }
            return res;
        }
        
        //Funciones basicas
        void put(string dominio, string path, string titulo, int tiempo){
            insertar(dominio, path, titulo, tiempo);
        }

        void remove(string dominio, string path){
            borrar(dominio, path);
        }

        void clear(){
            for (int i = 0; i < tamano; i++){
                NodoDominio* celda = tabla[i];
                while (celda != NULL){
                    NodoDominio* nxt = celda->sig;
                    liberarListaRecursos(celda->recursos);
                    delete celda;
                    celda = nxt;
                }
                tabla[i] = NULL;
            }
            cantidadDominios = 0;
        }
};