#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <random>

using namespace std;

float mul2[] = {1.0f,-1.0f};

string get0(tuple<string,string,int,int,float> padre, int corte){
    return get<0>(padre).substr(0,corte);
}

string get1(tuple<string,string,int,int,float> padre, int corte){
    return get<0>(padre).substr(corte);
}

string get2(tuple<string,string,int,int,float> padre, int corte){
    return get<1>(padre).substr(0,corte);
}

string get3(tuple<string,string,int,int,float> padre, int corte){
    return get<1>(padre).substr(corte);
}

string (*getting[2][2])(tuple<string,string,int,int,float>, int ) = {{get0,get1},{get2,get3}};

class GA2{
public:
    vector<tuple<string,string,int,int,float>> poblacion;
    int nPobla;
    int tBin[2];
    bool minimizar;
    vector<pair<int,int>> limites;

    GA2(int, bool, vector<pair<int,int>>);
    // Métodos
    void genPoblacion(); // Genera una poblacion de forma aleatoria
    int tamBinario(bool ); // Halla la cantidad de bits maximos.
    string getBinarios(int, bool); // Obtener los binarios
    float f(int, int); // Funcion
    void seleccion(); // Seleccion a partir de torneo
    int getNumber(string ); // Obtener un numero de un binario
    void cruceMuta(); // Cruzamiento y mutación
    float getPromedio();
    float getMaximo();
};

float GA2::getPromedio(){
    float res = 0.0f;
    for(int i = 0; i < nPobla; i++){
        res += (float)get<4>(poblacion[i]);
    }
    res = res/(float)nPobla;
    return res;
}

float GA2::getMaximo(){
    float maximum = -1e9f;
    for(int i = 0; i < nPobla; i++){
        maximum = max((float)get<4>(poblacion[i]),maximum);
    }
    return maximum;
}

GA2::GA2(int nP, bool m, vector<pair<int,int>> l){
    nPobla = nP;
    tBin[0] = 0;
    tBin[1] = 0;
    minimizar = m;
    poblacion.resize(nP);
    limites = l;
}

int GA2::tamBinario(bool choose){
    int cant = 0;
    int number = limites[0].second;
    if(choose) number = limites[1].second;
    while(number){
        number/=2;
        cant++;
    }
    cant++;
    return cant;
}

string GA2::getBinarios(int number, bool c){
    string binario = "";
    while(number){
        if(number%2 == 0){
            binario = '0' + binario;
        }
        else if(number%2 == 1){
            binario = '1' + binario;
        }
        number /= 2;
    }
    int tBinario = binario.size();
    tBin[c] = tamBinario(c);
    for(int i = tBinario; i < tBin[c]-1; i++){
        binario = '0' + binario;
    }
    return binario;
}

void GA2::genPoblacion(){
    srand(time(NULL));
    int num[2];
    string binario[2];
    for(int i = 0; i < nPobla; i++){
        for(int j = 0; j < 2; j++){
            num[j] = limites[j].first + rand() % (limites[j].second -limites[j].first + 1);
            binario[j] = getBinarios(num[j], j);
        }

        float fit = f(num[0], num[1]);
        if(minimizar) fit = 1/(1+f(num[0], num[1]));
        poblacion[i] = make_tuple(binario[0], binario[1],num[0], num[1],fit);
    }
    return;
}

float GA2::f(int x, int y){
    return sin(x)*cos(y);
}

void GA2::seleccion(){
    srand(time(NULL));
    vector<tuple<string,string,int,int,float>> new_poblacion;
    for(int i = 0; i < nPobla; i++){
        vector<int> indices;
        if(nPobla >= 4){
            while(indices.size()<nPobla){
                int valor = rand()%nPobla;
                if(find(indices.begin(), indices.end(), valor) == indices.end()){
                    indices.push_back(valor);
                }
            }
        }
        else for(int i = 0; i < nPobla; i++) indices.push_back(rand()%nPobla);

        // indices.push_back(rand()%nPobla);

        // for(int i = 0; i < nPobla; i++) cout << indices[i] << " ";
        // cout << endl;
        // cout << "first match: " << indices[0] << " vs " << indices[1] << endl;
        // cout << "second match: " << indices[2] << " vs " << indices[3] << endl;
        tuple<string,string,int,int,float> semi1 = (get<4>(poblacion[indices[0]]) < get<4>(poblacion[indices[1]])) ? poblacion[indices[1]] : poblacion[indices[0]];
        tuple<string,string,int,int,float> semi2 = (get<4>(poblacion[indices[2]]) < get<4>(poblacion[indices[3]])) ? poblacion[indices[3]] : poblacion[indices[2]];
        tuple<string,string,int,int,float> fin = (get<4>(semi1) < get<4>(semi2)) ? semi2 : semi1;

        new_poblacion.push_back(fin);
    }
    poblacion = new_poblacion;
    return;
}

int GA2::getNumber(string b){
    int result = 0;
    for(int i = b.size()-1, j = 0; i > -1; i--, j++){
        if(b[i] == '1') result += pow(2,j);
    }
    return result;
}

void GA2::cruceMuta(){
    srand(time(NULL));
    vector<tuple<string,string,int,int,float>> nuevoPobla;
    int indMuta = rand() % nPobla;
    for(int i = 0; i < nPobla; i++){
        tuple<string,string,int,int,float> padre1[2], padre2[2];
        string hijo[2];
        int valorHijo[2];
        int corte[2];
        for(int j = 0; j < 2; j++){
            int indice1 = rand()%nPobla;
            int indice2 = rand()%nPobla;
            corte[j] = 2 + rand()%(tBin[j]-2);
            // cout << j << ": " << tBin[j] << " " << corte[j] << endl;
            // int corte = 2;
            while(indice1 == indice2) indice2 = rand()%nPobla;
            // cout << j << " " << indice1 << " " << indice2 << endl;
            padre1[j] = poblacion[indice1];
            padre2[j] = poblacion[indice2];
            hijo[j] = getting[j][0](padre1[j],corte[j]);
            hijo[j] += getting[j][1](padre2[j],corte[j]);
            if(i == indMuta){
                int bit = rand()%tBin[j];
                if(hijo[j][bit] == '1') hijo[j][bit] = '0';
                else hijo[j][bit] = '1';
            }
            valorHijo[j] = getNumber(hijo[j]);
            while(valorHijo[j] < limites[j].first || valorHijo[j] > limites[j].second){
                hijo[j] = "";
                indice1 = rand()%nPobla;
                indice2 = rand()%nPobla;
                while(indice1 == indice2) indice2 = rand()%nPobla;
                padre1[j] = poblacion[indice1];
                padre2[j] = poblacion[indice2];

                hijo[j] = getting[j][0](padre1[j],corte[j]);
                hijo[j] += getting[j][1](padre2[j],corte[j]);
                if(i == indMuta){
                    int bit = rand()%tBin[j];
                    if(hijo[j][bit] == '1') hijo[j][bit] = '0';
                    else hijo[j][bit] = '1';
                }
                valorHijo[j] = getNumber(hijo[j]);
                // cout << j << " " << hijo[j] << " " <<  valorHijo[j] << endl;
                // cout << j << " " << valorHijo[j] << endl;
            }   
        }
        if(minimizar) nuevoPobla.push_back(make_tuple(hijo[0], hijo[1], valorHijo[0], valorHijo[1], 1.0f/(1+this->f(valorHijo[0], valorHijo[1]))));
        else nuevoPobla.push_back(make_tuple(hijo[0], hijo[1], valorHijo[0], valorHijo[1], this->f(valorHijo[0], valorHijo[1])));
    }
    poblacion = nuevoPobla;
}
