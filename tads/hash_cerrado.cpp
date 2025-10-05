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
        int tamano;
        int cantidadElementos;
        
        float factorDeCarga(){
            return (float)cantidadElementos / tamano;
        }

        void rehash(){
            int tamanoNuevo = siguientePrimo(tamano * 2);

            // Creo una nueva tabla
            Asociacion** nuevaTabla = new Asociacion*[tamanoNuevo];
            for (int i = 0; i < tamanoNuevo; i++) {
                nuevaTabla[i] = NULL;
            }

            // Copio y pego los elementos (Lo borrado no se cuenta)
            for (int i = 0; i < this->tamano; i++) {
                if(tabla[i] != NULL){
                    if (tabla[i]->estado == OCUPADO) {
                        //h1
                        int hash = fnHash(tabla[i]->dominio, tabla[i]->path);
                        int base = normalizar(hash, tamanoNuevo);

                        //h2 depende del tamano viejo, uso el nuevo
                        unsigned int sum2 = 0;
                        for (size_t k = 0; k < tabla[i]->dominio.size(); k++) {
                            sum2 = sum2 * 131u + (unsigned char)tabla[i]->dominio[k];
                        }
                        sum2 = sum2 * 131u + (unsigned char)'/';
                        for (size_t k = 0; k < tabla[i]->path.size(); k++) {
                            sum2 = sum2 * 131u + (unsigned char)tabla[i]->path[k];
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

        int fnHash2(const string& dominio, const string& path) {
            unsigned int sum = 0;
            for (int k = 0; k < dominio.size(); k++){
                sum = sum * 131 + (unsigned char)dominio[k];
            } 
            sum = sum * 131 + (unsigned char)'/';
            for (int k = 0; k < path.size(); k++){
                sum = sum * 131 + (unsigned char)path[k];
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

        int posicionTentativa(int i, string dominio, string path)
        {
            // Doble Hash
            return (int)(this->fnHash(dominio, path) + i*this->fnHash2(dominio, path));
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

            int posBucketBorrado = -1;
            bool inserte = false;
            int intento = 0;

            while (!inserte && intento < this->tamano){
                int hashValue = this->posicionTentativa(intento, dominio, path);
                unsigned int pos = this->normalizar(hashValue, this->tamano);
                Asociacion* celda = tabla[pos];

                // A) Bucket nunca usado
                if (celda == NULL) {
                    int target = (posBucketBorrado != -1) ? posBucketBorrado : (int)pos;

                    if (tabla[target] == NULL) {
                        tabla[target] = new Asociacion(dominio, path, titulo, tiempo);
                    } else {
                        tabla[target]->dominio = dominio;
                        tabla[target]->path    = path;
                        tabla[target]->titulo  = titulo;
                        tabla[target]->tiempo  = tiempo;
                        tabla[target]->estado  = OCUPADO;
                    }
                    cantidadElementos++;
                    inserte = true;
                }
                // B) Bucket borrado
                else if (celda->estado == BORRADO) {
                    if (posBucketBorrado == -1){
                        posBucketBorrado = (int)pos;
                    } 
                    intento++;
                }
                // C) Bucket ocupado
                else {
                    if (celda->dominio == dominio && celda->path == path) {
                        // Actualizo
                        celda->titulo = titulo;
                        celda->tiempo = tiempo;
                        inserte = true;
                    } else {
                        intento++;
                    }
                }
            }
        }

        void borrar(string dominio, string path){
            bool borre = false;
            int intento = 0;

            while (!borre && intento < this->tamano)
            {
                int hashValue = this->posicionTentativa(intento, dominio, path); // h1 + i*h2
                unsigned int pos = this->normalizar(hashValue, this->tamano);
                Asociacion* celda = tabla[pos];

                // No existe
                if (celda == NULL) {
                    break;
                }
                // Bucket borrado
                if (celda->estado == BORRADO) {
                    intento++;
                    continue;
                }
                if (celda->dominio == dominio && celda->path == path) {
                    celda->estado = BORRADO;
                    this->cantidadElementos--;
                    borre = true;
                } else {
                    intento++;
                }
            }
        }

        string buscar(string dominio, string path){
            int intento = 0;
            string resultado = "recurso_no_encontrado";

            while (intento < this->tamano){
                int hashValue = this->posicionTentativa(intento, dominio, path);
                unsigned int pos = this->normalizar(hashValue, this->tamano);
                Asociacion* celda = tabla[pos];

                // Bucket nunca usado
                if (celda == NULL) {
                    break;
                }
                // Bucket borrado
                if (celda->estado == BORRADO) {
                    intento++;
                    continue;
                }
                // Bucket ocupado
                if (celda->dominio == dominio && celda->path == path) {
                    resultado = celda->titulo + " " + to_string(celda->tiempo);
                    break;
                } else {
                    intento++;
                }
            }
            return resultado;
        }

    public:
        //Crea una tabla de hash vacia
        HashCerrado(unsigned int tamanoInicial){
            //unsigned int es para que no acepte negativos
            this->tamano = siguientePrimo(tamanoInicial);
            this->cantidadElementos = 0;
            this->tabla = new Asociacion*[this->tamano];
            for(int i = 0; i < this->tamano; i++){
                this->tabla[i] = NULL;
            }
        }
        
        ~HashCerrado(){
            for(int i = 0; i < this->tamano; i++){
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

        void clear_domain(string dominio){
            // Orden K
            // Ir a Domain y agarrar lista de recursos
            // Con este domain y esa lista de recursos ir uno a uno y hashear en Asociacion y borrar
            // Luego voy a Domain y borro toda la lista de recursos
        }

        void clear(){
            for (int i = 0; i < this->tamano; i++){
            if (this->tabla[i] != NULL){
                delete this->tabla[i];
                this->tabla[i] = NULL;
            }
        }
        this->cantidadElementos = 0;
        }
};
    