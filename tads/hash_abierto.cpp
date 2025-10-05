#include <iostream>
#include <string>

using namespace std;

struct NodoRecurso {
    string dominio;
    string path;
    string titulo;
    int tiempo;
    NodoRecurso* sig;

    NodoRecurso(string d, string p, string t, int ti) 
        : dominio(d), path(p), titulo(t), tiempo(ti), sig(NULL) {}
};

class HashAbierto{
    private:
        NodoRecurso** tabla; 
        int tamano;
        int cantidadElementos; 

        float factorDeCarga(){
            return (float)cantidadElementos / tamano;
        }

        void rehash(){
            int tamanoNuevo = siguientePrimo(tamano * 2);
            NodoRecurso** nuevaTabla = new NodoRecurso*[tamanoNuevo];
            for (int i = 0; i < tamanoNuevo; i++) {
                nuevaTabla[i] = NULL;
            }

            // Reinsertar todos los elementos en la nueva tabla
            for (int i = 0; i < tamano; i++){
                NodoRecurso* celda = tabla[i];
                while (celda != NULL){
                    NodoRecurso* proximo = celda->sig;

                    int pos = normalizar(fnHash(celda->dominio, celda->path), tamanoNuevo);
                    celda->sig = nuevaTabla[pos];   
                    nuevaTabla[pos] = celda;

                    celda = proximo;
                }
                tabla[i] = NULL;
            }

            delete[] tabla;
            tabla = nuevaTabla;
            tamano = tamanoNuevo;
        }

        int fnHash(const string& dominio, const string& path) {
            unsigned int sum = 0;
            for (int k = 0; k < dominio.size(); k++){
                sum = sum * 31 + (unsigned char)dominio[k];
            } 
            sum = sum * 31 + (unsigned char)'/';
            for (int k = 0; k < path.size(); k++){
                sum = sum * 31 + (unsigned char)path[k];
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
            
            int pos = normalizar(fnHash(dominio, path), tamano);
            NodoRecurso* celda = tabla[pos];

            // Buscar si ya existe
            while (celda != NULL){
                if (celda->dominio == dominio && celda->path == path){
                    celda->titulo = titulo;
                    celda->tiempo = tiempo;
                    return;
                }
                celda = celda->sig;
            }

            // No existe
            NodoRecurso* nuevo = new NodoRecurso(dominio, path, titulo, tiempo);
            nuevo->sig = tabla[pos];
            tabla[pos] = nuevo;
            cantidadElementos++;
        }

        void borrar(string dominio, string path){
            int pos = normalizar(fnHash(dominio, path), tamano);
            NodoRecurso* celda = tabla[pos];
            NodoRecurso* prev = NULL;

            while (celda != NULL){
                if (celda->dominio == dominio && celda->path == path){
                    if (prev == NULL) {
                        tabla[pos] = celda->sig;
                    } else {
                        prev->sig = celda->sig;
                    }
                    delete celda;
                    cantidadElementos--;          
                    return;                      
                }
                prev = celda;
                celda = prev->sig;
            }
        }

        string buscar(string dominio, string path){
            int pos = normalizar(fnHash(dominio, path), tamano);
            NodoRecurso* celda = tabla[pos];
            while (celda != NULL){
                if (celda->dominio == dominio && celda->path == path){
                    return celda->titulo + " " + to_string(celda->tiempo);
                }
                celda = celda->sig;
            }
            return "recurso_no_encontrado";
        }

    public:
        //Crea una tabla de hash vacia
        HashAbierto(unsigned int tamanoInicial){
            tamano = siguientePrimo(tamanoInicial);
            cantidadElementos = 0;

            tabla = new NodoRecurso*[tamano];
            for (int i = 0; i < tamano; i++){
                tabla[i] = NULL;
            }
        }
        
        ~HashAbierto(){
            clear();         
            delete[] tabla;   
            tabla = NULL;
            tamano = 0;
        }

        // Funciones principales
        void put(string dominio, string path, string titulo, int tiempo){
            insertar(dominio, path, titulo, tiempo);
        }

        void remove(string dominio, string path){
            borrar(dominio, path);
        }

        bool contains(string dominio, string path){
            return this->get(dominio, path) != "recurso_no_encontrado";
        }

        string get(string dominio, string path){
            return buscar(dominio, path);
        }

        int size(){
            return cantidadElementos;
        }

        void clear_domain(string dominio){
            // Orden K
            // Ir a Domain y agarrar lista de recursos
            // Con este domain y esa lista de recursos ir uno a uno y hashear en Asociacion y borrar
            // Luego voy a Domain y borro toda la lista de recursos
        }

        void clear(){
            for (int i = 0; i < tamano; i++){
                NodoRecurso* cur = tabla[i];
                while (cur != NULL){
                    NodoRecurso* nxt = cur->sig;
                    delete cur;
                    cur = nxt;
                }
                tabla[i] = NULL;
            }
            cantidadElementos = 0;
        }
};





