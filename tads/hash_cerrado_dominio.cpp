#include <iostream>
#include <string>
#include <cassert>
using namespace std;

enum EstadoDom { DVACIO, DOCUPADO, DBORRADO };

struct NodoRecurso {
    string path;
    string titulo;
    int tiempo;
    NodoRecurso* sig;

    NodoRecurso(string p, string t, int ti) : path(p), titulo(t), tiempo(ti), sig(nullptr) {}
};

struct Dominio {
    string dominio;
    NodoRecurso* recursos; // lista: más reciente primero
    int cant;
    EstadoDom estado;

    Dominio(string d) : dominio(d), recursos(nullptr), cant(0), estado(DOCUPADO) {}
};

class HashDominio {
    private:
        Dominio** tabla;
        int tamano;
        int cantidadElementos;

        float factorDeCarga(){
            return (float)cantidadElementos / tamano;
        }

        void rehash(){
            int tamanoNuevo = siguientePrimo(tamano * 2);

            Dominio** nuevaTabla = new Dominio*[tamanoNuevo];
            for (int i = 0; i < tamanoNuevo; i++) {
                nuevaTabla[i] = NULL;
            }
            // Copio y pego los elementos (Lo borrado no se cuenta)
            for (int i = 0; i < this->tamano; i++) {
                if(tabla[i] != NULL){
                    if (tabla[i]->estado == DOCUPADO) {
                        int hash = fnHash(tabla[i]->dominio);
                        int base = normalizar(hash, tamanoNuevo);

                        unsigned int sum2 = 0;
                        for (size_t k = 0; k < tabla[i]->dominio.size(); k++) {
                            sum2 = sum2 * 131u + (unsigned char)tabla[i]->dominio[k];
                        }
                        int step = 1 + (int)(sum2 % (unsigned)(tamanoNuevo - 1));
                        if (step == 0) {
                            step = 1;
                        }
                
                        // Copio el elemento
                        for (int intento = 0; intento < tamanoNuevo; ++intento) {
                            int pos = (base + intento * step) % tamanoNuevo;
                            if (nuevaTabla[pos] == NULL) {
                                nuevaTabla[pos] = tabla[i];
                                break;
                            }
                        }
                    } else {
                        delete tabla[i];
                    }
                }
            }
            delete[] tabla;
            tabla = nuevaTabla;
            tamano = tamanoNuevo;
        }

        int fnHash(const string& dominio) {
            unsigned int sum = 0;
            for (int k = 0; k < dominio.size(); k++){
                sum = sum * 31 + (unsigned char)dominio[k];
            }

            return (int)sum;
        }

        int fnHash2(const string& dominio) {
            unsigned int sum = 0;
            for (int k = 0; k < dominio.size(); k++){
                sum = sum * 131 + (unsigned char)dominio[k];
            } 
            
            if (this->tamano <= 1){
                return 1;
            } 
            int step = 1 + (int)(sum % (unsigned)(this->tamano - 1));
            if (step == 0){
                step = 1; 
            } 
            return step;
        }

        int posicionTentativa(int i, string dominio)
        {
            //Doble Hash
            return (int)(this->fnHash(dominio) + i*this->fnHash2(dominio));
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
            if (factorDeCarga() > 0.7){
                this->rehash();
            }

            int posBucketDBORRADO = -1;
            bool inserte = false;
            int intento = 0;

            while (!inserte && intento < this->tamano){
                int hashValue = this->posicionTentativa(intento, dominio);
                unsigned int pos = this->normalizar(hashValue, this->tamano);
                Dominio* celda = tabla[pos];

                if (celda == NULL) {
                    int target = (posBucketDBORRADO != -1) ? posBucketDBORRADO : (int)pos;

                    if (tabla[target] == NULL) {
                        tabla[target] = new Dominio(dominio);
                        
                    } else {
                        tabla[target]->dominio  = dominio;
                        tabla[target]->estado   = DOCUPADO;
                        tabla[target]->recursos = NULL;
                        tabla[target]->cant     = 0;
                    }
                    // insertar recurso
                    insertarNodo(tabla[target], path, titulo, tiempo);
                    this->cantidadElementos++;
                    inserte = true;
                }
                else if (celda->estado == DBORRADO) {
                    if (posBucketDBORRADO == -1){
                        posBucketDBORRADO = (int)pos;
                    }
                    intento++;
                }
                else {
                    if (celda->dominio == dominio) {
                        borrarNodo(celda, path);
                        insertarNodo(celda, path, titulo, tiempo);
                        inserte = true;
                    } else {
                        intento++;
                    }
                }
            }
        }

        void borrar(string dominio, const string& path){
            int intento = 0;

            while (intento < this->tamano){
                int hashValue = this->posicionTentativa(intento, dominio);
                unsigned int pos = this->normalizar(hashValue, this->tamano);
                Dominio* celda = tabla[pos];

                if (celda == NULL) {
                    break;
                }
                if (celda->estado == DBORRADO) {
                    intento++; 
                    continue; 
                }
                if (celda->dominio == dominio) {
                    borrarNodo(celda, path);
                    return;
                } else {
                    intento++;
                }
            }
        }

        //Funciones de lista
        void insertarNodo(Dominio* celda, const string& path, const string& titulo, int tiempo) {
            NodoRecurso* nuevo = new NodoRecurso(path, titulo, tiempo);
            nuevo->sig = celda->recursos; 
            celda->recursos = nuevo;
            celda->cant++;
        }

        void borrarNodo(Dominio* celda, const string& path) {
            NodoRecurso* prev = NULL;
            NodoRecurso* recursos  = celda->recursos;

            while (recursos != NULL && recursos->path != path) {
                prev = recursos;
                recursos  = recursos->sig;
            }

            if (recursos == NULL) {
                return;
            }

            if (prev == NULL) {
                celda->recursos = recursos->sig;
            } else {
                prev->sig = recursos->sig;
            }

            delete recursos;
            
            if (celda->cant > 0){
                celda->cant--;
            } 
        }

    public:
        //Crea una tabla de hash vacia
        HashDominio(unsigned int nInicial) {
            tamano = siguientePrimo((int) (nInicial > 2 ? nInicial : 2));
            cantidadElementos = 0;
            tabla = new Dominio*[tamano];
            for (int i = 0; i < tamano; ++i) tabla[i] = NULL;
        }

        ~HashDominio() {
            if (!tabla) return;
            for (int i = 0; i < tamano; ++i) {
                Dominio* celda = tabla[i];
                if (celda != NULL) {
                    // liberar lista enlazada
                    NodoRecurso* cur = celda->recursos;
                    while (cur) {
                        NodoRecurso* nxt = cur->sig;
                        delete cur;
                        cur = nxt;
                    }
                    delete celda;
                }
            }
            delete[] tabla;
            tabla = NULL;
            tamano = 0;
            cantidadElementos = 0;
        }

        //Funciones exclusivas de la clase
        int count_domain(string dominio){
            int intento = 0;
            while (intento < this->tamano){
                int hashValue = this->posicionTentativa(intento, dominio);
                unsigned int pos = this->normalizar(hashValue, this->tamano);
                Dominio* celda = tabla[pos];

                if (celda == NULL) {
                    break;
                }
                if (celda->estado == DBORRADO) {
                    intento++; 
                    continue; 
                }

                if (celda->dominio == dominio) {
                    return celda->cant;
                } else {
                    intento++;
                }
            }
            return 0;
        }

        string list_domain(string dominio){
            // Puedo devolver la lista o solo imprimirla. Elijo imprimirla.
            string resultado = "";

            //Busco la lista del dominio (Hash)
            int intento = 0;
            while (intento < this->tamano){
                int hashValue = this->posicionTentativa(intento, dominio);
                unsigned int pos = this->normalizar(hashValue, this->tamano);
                Dominio* celda = tabla[pos];

                if (celda == NULL){
                    break;
                }
                if (celda->estado == DBORRADO) { 
                    intento++; 
                    continue; 
                }
                if (celda->dominio == dominio) {
                    NodoRecurso* actual = celda->recursos;
                    while (actual != NULL) {
                        resultado += actual->path + " ";
                        actual = actual->sig;
                    }
                    return resultado;
                } else {
                    intento++;
                }
            }
            return resultado;
        }
        
        //Funciones basicas
        void put(string dominio, string path, string titulo, int tiempo){
            insertar(dominio, path, titulo, tiempo);
        }

        void remove(string dominio, string path){
            borrar(dominio, path);
        }

        void clear(){
            for (int i = 0; i < tamano; ++i) {
                Dominio* celda = tabla[i];
                if (celda != NULL) {
                    NodoRecurso* cur = celda->recursos;
                    while (cur) {
                        NodoRecurso* nxt = cur->sig;
                        delete cur;
                        cur = nxt;
                    }
                    delete celda;
                    tabla[i] = NULL;
                }
            }
            cantidadElementos = 0; 
        }
    };