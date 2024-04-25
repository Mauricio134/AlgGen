#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <random>

using namespace std;

float mul3[] = {1.0f,-1.0f};

string get0(tuple<string,string, string,int,int, int,float> padre, int corte){
    return get<0>(padre).substr(0,corte);
}

string get1(tuple<string,string, string,int,int, int,float> padre, int corte){
    return get<0>(padre).substr(corte);
}

string get2(tuple<string,string, string,int,int, int,float> padre, int corte){
    return get<1>(padre).substr(0,corte);
}

string get3(tuple<string,string, string,int,int, int,float> padre, int corte){
    return get<1>(padre).substr(corte);
}

string get4(tuple<string,string, string,int,int, int,float> padre, int corte){
    return get<2>(padre).substr(0,corte);
}

string get5(tuple<string,string, string,int,int, int,float> padre, int corte){
    return get<2>(padre).substr(corte);
}

string (*getting3[3][3])(tuple<string,string, string,int,int, int,float>, int ) = {{get0,get1},{get2,get3},{get4,get5}};

class GA3{
public:
    vector<tuple<string,string, string,int,int, int,float>> poblacion;
    int nPobla;
    int tBin[3];
    bool minimizar;
    vector<pair<int,int>> limites;

    GA3(int, bool, vector<pair<int,int>>);
    // Métodos
    void genPoblacion(); // Genera una poblacion de forma aleatoria
    int tamBinario(int ); // Halla la cantidad de bits maximos.
    string getBinarios(int, int); // Obtener los binarios
    float f(int, int, int); // Funcion
    void seleccion(); // Seleccion a partir de torneo
    int getNumber(string ); // Obtener un numero de un binario
    void cruceMuta(); // Cruzamiento y mutación
    float getPromedio();
    float getMaximo();
};

float GA3::getPromedio(){
    float res = 0.0f;
    for(int i = 0; i < nPobla; i++){
        res += (float)get<6>(poblacion[i]);
    }
    res = res/(float)nPobla;
    return res;
}

float GA3::getMaximo(){
    float maximum = -1e9f;
    for(int i = 0; i < nPobla; i++){
        maximum = max((float)get<6>(poblacion[i]),maximum);
    }
    return maximum;
}

GA3::GA3(int nP, bool m, vector<pair<int,int>> l){
    nPobla = nP;
    tBin[0] = 0;
    tBin[1] = 0;
    minimizar = m;
    poblacion.resize(nP);
    limites = l;
}

int GA3::tamBinario(int choose){
    int cant = 0;
    int number = limites[0].second;
    if(choose == 1) number = limites[1].second;
    else if(choose == 2) number = limites[2].second;
    while(number){
        number/=2;
        cant++;
    }
    cant++;
    return cant;
}

string GA3::getBinarios(int number, int c){
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

void GA3::genPoblacion(){
    srand(time(NULL));
    int num[3];
    string binario[3];
    for(int i = 0; i < nPobla; i++){
        for(int j = 0; j < 3; j++){
            num[j] = limites[j].first + rand() % (limites[j].second -limites[j].first + 1);
            binario[j] = getBinarios(num[j], j);
        }

        float fit = f(num[0], num[1], num[2]);
        if(minimizar) fit = 1/(1+f(num[0], num[1], num[2]));
        poblacion[i] = make_tuple(binario[0], binario[1], binario[2],num[0], num[1], num[2],fit);
    }
    return;
}

float GA3::f(int x, int y, int z){
    // cout << x << " " << y << " " << z << endl;
    return x*y*z;
}

void GA3::seleccion(){
    srand(time(NULL));
    vector<tuple<string,string, string,int,int, int,float>> new_poblacion;
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
        tuple<string,string, string,int,int, int,float> semi1 = (get<6>(poblacion[indices[0]]) < get<4>(poblacion[indices[1]])) ? poblacion[indices[1]] : poblacion[indices[0]];
        tuple<string,string, string,int,int, int,float> semi2 = (get<6>(poblacion[indices[2]]) < get<4>(poblacion[indices[3]])) ? poblacion[indices[3]] : poblacion[indices[2]];
        tuple<string,string, string,int,int, int,float> fin = (get<6>(semi1) < get<4>(semi2)) ? semi2 : semi1;

        new_poblacion.push_back(fin);
    }
    poblacion = new_poblacion;
    return;
}

int GA3::getNumber(string b){
    int result = 0;
    for(int i = b.size()-1, j = 0; i > -1; i--, j++){
        if(b[i] == '1') result += pow(2,j);
    }
    return result;
}

void GA3::cruceMuta(){
    srand(time(NULL));
    vector<tuple<string,string, string,int,int, int,float>> nuevoPobla;
    int indMuta = rand() % nPobla;
    for(int i = 0; i < nPobla; i++){
        tuple<string,string, string,int,int, int,float> padre1[3], padre2[3];
        string hijo[3];
        int valorHijo[3];
        int corte[3];
        for(int j = 0; j < 3; j++){
            int indice1 = rand()%nPobla;
            int indice2 = rand()%nPobla;
            corte[j] = 2 + rand()%(tBin[j]-2);
            // cout << j << ": " << tBin[j] << " " << corte[j] << endl;
            // int corte = 2;
            while(indice1 == indice2) indice2 = rand()%nPobla;
            // cout << j << " " << indice1 << " " << indice2 << endl;
            padre1[j] = poblacion[indice1];
            padre2[j] = poblacion[indice2];
            hijo[j] = getting3[j][0](padre1[j],corte[j]);
            hijo[j] += getting3[j][1](padre2[j],corte[j]);
            // cout << "Antes: " << hijo[j] << endl;
            if(i == indMuta){
                // cout << "tamano: " << tBin[j] << endl;
                int bit = rand()%tBin[j];
                if(hijo[j][bit] == '1') hijo[j][bit] = '0';
                else hijo[j][bit] = '1';
            }
            // cout << "Despues: " << hijo[j] << endl;
            valorHijo[j] = getNumber(hijo[j]);
            while(valorHijo[j] < limites[j].first || valorHijo[j] > limites[j].second){
                hijo[j] = "";
                indice1 = rand()%nPobla;
                indice2 = rand()%nPobla;
                while(indice1 == indice2) indice2 = rand()%nPobla;
                padre1[j] = poblacion[indice1];
                padre2[j] = poblacion[indice2];

                hijo[j] = getting3[j][0](padre1[j],corte[j]);
                hijo[j] += getting3[j][1](padre2[j],corte[j]);
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
        // cout << valorHijo[0] << ", " <<  valorHijo[1] << ", " << valorHijo[2] << endl;
        if(minimizar) nuevoPobla.push_back(make_tuple(hijo[0], hijo[1], hijo[2], valorHijo[0], valorHijo[1], valorHijo[2], 1.0f/(1.0f+this->f(valorHijo[0], valorHijo[1], valorHijo[2]))));
        else nuevoPobla.push_back(make_tuple(hijo[0], hijo[1], hijo[2], valorHijo[0], valorHijo[1], valorHijo[2], this->f(valorHijo[0], valorHijo[1], valorHijo[2])));
    }
    poblacion = nuevoPobla;
}
