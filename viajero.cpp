#include <iostream>
#include "TSP.h"

using namespace std;

int main(){
    srand(time(NULL));
    TSP viajero(9, 4, 6);
    int generacion = 5;

    viajero.generarPoblacion();

    for(int i = 0; i < viajero.poblacion.size(); i++){
        vector<string> camino = get<0>(viajero.poblacion[i]);
        for(int j = 0; j < camino.size(); j++){
            cout << camino[j];
        }
        cout << ": ";
        cout << get<1>(viajero.poblacion[i]) << endl;
    }
    vector<string> menor = viajero.minimum();
    cout << "minimo camino: " ;
    for(int j = 0; j < menor.size(); j++){
        cout << menor[j];
    }
    cout << endl;
    for(int i = 2; i <= generacion; i++){
        cout << endl;
        viajero.seleccion();
        // for(int i = 0; i < viajero.poblacion.size(); i++){
        //     vector<string> camino = get<0>(viajero.poblacion[i]);
        //     for(int j = 0; j < camino.size(); j++){
        //         cout << camino[j];
        //     }
        //     cout << ": ";
        //     cout << get<1>(viajero.poblacion[i]) << endl;
        // }

        // cout << endl;
        viajero.PMXmuta();

        for(int i = 0; i < viajero.poblacion.size(); i++){
            vector<string> camino = get<0>(viajero.poblacion[i]);
            for(int j = 0; j < camino.size(); j++){
                cout << camino[j];
            }
            cout << ": ";
            cout << get<1>(viajero.poblacion[i]) << endl;
        }
        vector<string> menor = viajero.minimum();
        cout << "minimo camino: " ;
        for(int j = 0; j < menor.size(); j++){
            cout << menor[j];
        }
        cout << endl;
    }
    return 0;
}