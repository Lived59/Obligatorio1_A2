#include <iostream>
#include <string>
#include <cassert>
using namespace std;

enum Estado {VACIO, OCUPADO, BORRADO};

struct Asociacion {
    string dominio;
    string path;
    string titulo;
    int tiempo;
    Estado estado;

    Asociacion(string unDominio, string unPath, string unTitulo, int unTiempo)
        : dominio(unDominio), path(unPath), titulo(unTitulo), tiempo(unTiempo), estado(OCUPADO) {}
};

class HashCerrado {
    private:
        Asociacion** tabla;
        int tamaño;
        int cantidadElementos;
        
        float factorDeCarga(){
            return (float)cantidadElementos / tamaño;
        }

        void rehash(){
            int tamañoNuevo = siguientePrimo(tamaño * 2);

            Asociacion** nuevaTabla = new Asociacion*[tamañoNuevo];
            for (int i = 0; i < tamañoNuevo; i++) {
                nuevaTabla[i] = nullptr;
            }

            // mover solo OCUPADO; limpiar BORRADO
            for (int i = 0; i < tamaño; i++) {
                if(tabla[i] != nullptr){
                    if (tabla[i]->estado == OCUPADO) {
                        int hash = fnHash(tabla[i]->dominio, tabla[i]->path);
                        int base = normalizar(hash, tamañoNuevo);

                        // sondeo lineal en la tabla nueva
                        for (int intento = 0; intento < tamañoNuevo; ++intento) {
                            int pos = (base + intento) % tamañoNuevo;
                            if (nuevaTabla[pos] == nullptr) {
                                nuevaTabla[pos] = tabla[i];
                                break;
                            }
                        }
                    } else {
                        delete tabla[i]; // liberar
                    }
                }
            }
            delete[] tabla;
            tabla = nuevaTabla;
            tamaño = tamañoNuevo;
        }

        int fnHash(const string& dominio, const string& path) {
            unsigned int sum = 0;

            // Dominio
            for (size_t k = 0; k < dominio.size(); ++k) {
                sum = sum * 31 + dominio[k];
            }

            // separador para evitar colisiones entre (a,bc) y (ab,c)
            sum = sum * 31 + '/';

            // Path
            for (size_t k = 0; k < path.size(); ++k) {
                sum = sum * 31 + path[k];
            }

            return (int)sum;
        }

        int normalizar(int hash, int tamaño){
            //verificar valores posibles
            int n = hash % tamaño;
            if(n < 0){
                n += tamaño;
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
            
            int hash = this->fnHash(dominio, path);
            int normalizar = this->normalizar(hash, this->tamaño);

            int posBucketBorrado = -1;
            bool inserte = false;
            int intento = 0;

            while (!inserte && intento < this->tamaño){
                int hashValue = (normalizar + intento) % tamaño;
                Asociacion* cell = tabla[hashValue];

                // A) bucket nunca usado (nullptr): decidimos dónde insertar
                if (cell == nullptr) {
                    int target = (posBucketBorrado != -1) ? posBucketBorrado : hashValue;

                    if (tabla[target] == nullptr) {
                        // libre de verdad
                        tabla[target] = new Asociacion(dominio, path, titulo, tiempo); // ctor deja OCUPADO
                    } else {
                        // era BORRADO → reusar objeto (evita leak)
                        tabla[target]->dominio = dominio;
                        tabla[target]->path    = path;
                        tabla[target]->titulo  = titulo;
                        tabla[target]->tiempo  = tiempo;
                        tabla[target]->estado  = OCUPADO;
                    }
                    cantidadElementos++;
                    inserte = true;
                }
                // B) bucket BORRADO → recordar el primero para posible reutilización
                else if (cell->estado == BORRADO) {
                    if (posBucketBorrado == -1){
                        posBucketBorrado = hashValue;
                    } 
                    intento++;
                }
                // C) bucket OCUPADO
                else {
                    // ¿misma clave compuesta?
                    if (cell->dominio == dominio && cell->path == path) {
                        // actualizar (no cambia cantidadElementos)
                        cell->titulo = titulo;
                        cell->tiempo = tiempo;
                        inserte = true;
                    } else {
                        // otra clave → seguir sondeando
                        intento++;
                    }
                }
            }
        }

        void borrar(string dominio, string path){
            int hash = this->fnHash(dominio, path);
            int normalizar = this->normalizar(hash, this->tamaño);

            bool borre = false;
            int intento = 0;

            while (!borre && intento < this->tamaño)
            {
                int hashValue = (normalizar + intento) % tamaño;
                Asociacion* cell = tabla[hashValue];

                // no existe
                if (cell == nullptr) {
                    break;
                }

                // bucket borrado: seguir buscando
                if (cell->estado == BORRADO) {
                    intento++;
                    continue;
                }

                // comparar claves
                if (cell->dominio == dominio && cell->path == path) {
                    cell->estado = BORRADO;
                    cantidadElementos--;
                    borre = true;
                } else {
                    intento++;
                }
            }
        }

        string buscar(string dominio, string path){
            int hash = this->fnHash(dominio, path);
            int normalizar = this->normalizar(hash, this->tamaño);

            int intento = 0;
            string resultado = "recurso_no_encontrado";

            while (intento < this->tamaño){
                int hashValue = (normalizar + intento) % tamaño;
                Asociacion* cell = tabla[hashValue];

                // bucket nunca usado: no puede estar más adelante
                if (cell == nullptr) {
                    break;
                }

                // bucket borrado: seguir buscando
                if (cell->estado == BORRADO) {
                    intento++;
                    continue;
                }

                // bucket ocupado: comparar clave compuesta
                if (cell->dominio == dominio && cell->path == path) {
                    resultado = cell->titulo + " " + to_string(cell->tiempo);
                    break;
                } else {
                    intento++;
                }
            }
            return resultado;
        }

    public:
        //Crea una tabla de hash vacia
        HashCerrado(unsigned int tamañoInicial){
            //unsigned int es para que no acepte negativos
            this->tamaño = siguientePrimo(tamañoInicial);
            this->cantidadElementos = 0;
            this->tabla = new Asociacion*[this->tamaño];
            for(int i = 0; i < this->tamaño; i++){
                this->tabla[i] = nullptr;
            }
        }
        
        //Funcion clear() que borra todos los dominios
        ~HashCerrado(){
            for(int i = 0; i < this->tamaño; i++){
                delete this->tabla[i];
            }
            delete[] this->tabla;
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
            return this->cantidadElementos;
        }

        int count_domain(string dominio){
            //Orden 1
        }

        string list_domain(string dominio){
            //Orden K
        }

        void clear_domain(string dominio){
            //Orden K
        }

        void clear(){
            this->~HashCerrado();
        }
};
    