#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <random>
#include <algorithm>

using namespace std;

bool comp(const tuple<string, int, float>& a, const tuple<string, int, float>& b) {
    return get<2>(a) > get<2>(b);
}

class GA1{
public:
    vector<tuple<string,int,float>> poblacion;
    int nPobla;
    int tBin;
    bool minimizar;
    pair<int,int> limites;

    GA1(int, bool, pair<int,int>);
    // Métodos
    void genPoblacion(); // Genera una poblacion de forma aleatoria
    int tamBinario(); // Halla la cantidad de bits maximos.
    string getBinarios(int); // Obtener los binarios
    float f(int); // Funcion
    void seleccion(); // Seleccion a partir de torneo
    int getNumber(string ); // Obtener un numero de un binario
    void cruceMuta(); // Cruzamiento y mutación
    float getPromedio();
    float getMaximo();
};

float GA1::getPromedio(){
    float res = 0.0f;
    for(int i = 0; i < nPobla; i++){
        res += (float)get<2>(poblacion[i]);
    }
    res = res/(float)nPobla;
    return res;
}

float GA1::getMaximo(){
    float maximum = -1e9f;
    for(int i = 0; i < nPobla; i++){
        maximum = max((float)get<2>(poblacion[i]),maximum);
    }
    return maximum;
}

GA1::GA1(int nP, bool m, pair<int,int> l){
    nPobla = nP;
    tBin = 0;
    minimizar = m;
    poblacion.resize(nP);
    limites = l;
}

int GA1::tamBinario(){
    int cant = 0;
    int number = limites.second;
    while(number){
        number/=2;
        cant++;
    }
    cant++;
    return cant;
}

string GA1::getBinarios(int number){
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
    tBin = tamBinario();
    for(int i = tBinario; i < tBin-1; i++){
        binario = '0' + binario;
    }
    return binario;
}

void GA1::genPoblacion(){
    for(int i = 0; i < nPobla; i++){
        int num = limites.first + rand() % (limites.second -limites.first + 1);
        string binario = getBinarios(num);
        float fit = f(num);
        if(minimizar) fit = 1.0f/(1.0f+f(num));
        poblacion[i] = make_tuple(binario,num,fit);
    }
    return;
}

float GA1::f(int x){
    return pow(x,2);
    // return sin(x);
}

void GA1::seleccion(){
    vector<tuple<string,int,float>> new_poblacion;
    sort(poblacion.begin(), poblacion.end(), comp);
    new_poblacion.push_back(poblacion[0]);
    vector<tuple<string,int,float>> prueba(poblacion.begin()+1, poblacion.end());
    int tam = prueba.size();
    for(int i = 0; i < tam; i++){
        vector<int> indices;
        if(tam >= 4){
            while(indices.size()<tam){
                int valor = rand()%tam;
                if(find(indices.begin(), indices.end(), valor) == indices.end()){
                    indices.push_back(valor);
                }
            }
        }
        else for(int i = 0; i < tam; i++) indices.push_back(rand()%tam);

        // indices.push_back(rand()%nPobla);

        // for(int i = 0; i < nPobla; i++) cout << indices[i] << " ";
        // cout << endl;
        tuple<string,int,float> semi1 = (get<2>(prueba[indices[0]]) < get<2>(prueba[indices[1]])) ? prueba[indices[1]] : prueba[indices[0]];
        tuple<string,int,float> semi2 = (get<2>(prueba[indices[2]]) < get<2>(prueba[indices[3]])) ? prueba[indices[3]] : prueba[indices[2]];
        tuple<string,int,float> fin = (get<2>(semi1) < get<2>(semi2)) ? semi2 : semi1;

        new_poblacion.push_back(fin);
    }
    poblacion = new_poblacion;
    return;
}

int GA1::getNumber(string b){
    int result = 0;
    for(int i = b.size()-1, j = 0; i > -1; i--, j++){
        if(b[i] == '1') result += pow(2,j);
    }
    return result;
}

void GA1::cruceMuta(){
    vector<tuple<string, int,float>> nuevoPobla;
    int indMuta = rand() % nPobla;
    for(int i = 0; i < nPobla; i++){
        tuple<string,int,float> padre1, padre2;
        string hijo;
        int indice1 = rand()%nPobla;
        int indice2 = rand()%nPobla;
        int corte = 2 + rand()%(tBin-2);
        // int corte = 2;
        while(indice1 == indice2) indice2 = rand()%nPobla;
        padre1 = poblacion[indice1];
        padre2 = poblacion[indice2];

        hijo = get<0>(padre1).substr(0,corte);
        hijo += get<0>(padre2).substr(corte);
        if(i == indMuta){
            int bit = rand()%tBin;
            if(hijo[bit] == '1') hijo[bit] = '0';
            else hijo[bit] = '1';
        }
        int valorHijo = getNumber(hijo);
        while(valorHijo < limites.first || valorHijo > limites.second){
            hijo = "";
            indice1 = rand()%nPobla;
            indice2 = rand()%nPobla;
            while(indice1 == indice2) indice2 = rand()%nPobla;
            padre1 = poblacion[indice1];
            padre2 = poblacion[indice2];

            hijo = get<0>(padre1).substr(0,corte);
            hijo += get<0>(padre2).substr(corte);
            if(i == indMuta){
                int bit = rand()%tBin;
                if(hijo[bit] == '1') hijo[bit] = '0';
                else hijo[bit] = '1';
            }
            valorHijo = getNumber(hijo);
        }
        if(minimizar) nuevoPobla.push_back(make_tuple(hijo, valorHijo, 1.0f/(1.0f+this->f(valorHijo))));
        else nuevoPobla.push_back(make_tuple(hijo, valorHijo, this->f(valorHijo)));
    }
    poblacion = nuevoPobla;
}
