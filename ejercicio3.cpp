#include <iostream>
#include <string>
#include "./tads/hash_abierto.cpp"
#include "./tads/hash_abierto_dominio.cpp"

using namespace std;

static void borrarPaths(const string& recursos, const string& dominio, HashAbierto& asociacion, HashAbiertoDominio& dominios) {
    string path;
    for (int i = 0; i < recursos.size(); i++) {
        char caracter = recursos[i];
        if (caracter == ' ') {
            if (!path.empty()) {
                asociacion.remove(dominio, path);
                dominios.remove(dominio, path);
                path.clear();
            }
        } else {
            //Voy armando el path caracter por caracter
            path.push_back(caracter);
        }
    }
    if (!path.empty()) { // último token si no terminó con espacio
        asociacion.remove(dominio, path);
        dominios.remove(dominio, path);
    }
}

int main() {
    int N;
    cin >> N;

    HashAbierto asociacion((unsigned int)N);
    HashAbiertoDominio dominios((unsigned int)N);

    for (int i = 0; i < N; ++i) {
        string comando;
        cin >> comando;

        if (comando == "PUT") {
            string dom, path, titulo;
            int tiempo;
            cin >> dom >> path >> titulo >> tiempo;
            asociacion.put(dom, path, titulo, tiempo);
            dominios.put(dom, path, titulo, tiempo);
        }
        else if (comando == "GET") {
            string dom, path;
            cin >> dom >> path;
            cout << asociacion.get(dom, path) << '\n';
        }
        else if (comando == "REMOVE") {
            string dom, path;
            cin >> dom >> path;
            asociacion.remove(dom, path);
            dominios.remove(dom, path);
        }
        else if (comando == "CONTAINS") {
            string dom, path;
            cin >> dom >> path;
            cout << (asociacion.contains(dom, path) ? "true" : "false") << '\n';
        }
        else if (comando == "COUNT_DOMAIN") {
            string dom;
            cin >> dom;
            cout << dominios.count_domain(dom) << '\n';
        }
        else if (comando == "LIST_DOMAIN") {
            string dom;
            cin >> dom;
            string s = dominios.list_domain(dom); 
            cout << s << '\n';                    
        }
        else if (comando == "CLEAR_DOMAIN") {
            string dom;
            cin >> dom;
            string lista = dominios.list_domain(dom);
            //Separo path por path de la lista y borro uno por uno
            borrarPaths(lista, dom, asociacion, dominios);
        }
        else if (comando == "SIZE") {
            cout << asociacion.size() << '\n';
        }
        else if (comando == "CLEAR") {
            asociacion.clear();
            dominios.clear();
        }
    }
    return 0;
}
