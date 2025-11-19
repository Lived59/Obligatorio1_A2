#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;


void backTracking(){

    if(esSolucion()){

    }
    else{
        for(int i...){
            if(PuedoRealizarMovimiento()){
                RealizarMovimiento();
                backTracking();
                DeshacerMovimiento();
            }
        }
    }
}
int main()
{
    string in;
    cin >> in;
    int P;
    cin >> P;

    return 0;
}