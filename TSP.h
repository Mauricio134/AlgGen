#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <tuple>
#include <random>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <string>
#include <stack>

using namespace std;

typedef tuple<vector<string>, double> TUPLE;

bool compareByDouble(const TUPLE& a, const TUPLE& b) {
    return get<1>(a) < get<1>(b);
}

class TSP{
public:
    int nPoints;
    int nPobla;
    int inicio;
    vector<tuple<vector<string>,double>> poblacion;
    map<string, pair<double,double>> grafo;
    TSP(int , int, int);
    void generarPoblacion();
    void OXmuta();   
    double distanciaEuclidiana(int, int);
    double fitness(vector<string>);
    vector<string> getCamino();
    void seleccion();
    vector<string> minimum();
};

TSP::TSP(int nP, int nPo, int i){
    nPoints = nP;
    nPobla = nPo;
    inicio = i;
    poblacion.resize(nPo);
    for(int i = 0; i < nP; i++){
        int randomInt[2];
        randomInt[0] = rand();
        randomInt[1] = rand();
        double scaledX = static_cast<double>(randomInt[0]) / RAND_MAX;
        double randomX = scaledX * 1.6f - 0.8f;
        double scaledY = static_cast<double>(randomInt[1]) / RAND_MAX;
        double randomY = scaledY * 1.6f - 0.8f;
        grafo[to_string(i)] = make_pair(randomX, randomY);
    }
}

double TSP::distanciaEuclidiana(int p1, int p2){
    string pStart = to_string(p1);
    string pEnd = to_string(p2);
    return sqrt(pow((grafo[pEnd].first-grafo[pStart].first),2) + pow((grafo[pEnd].second-grafo[pStart].second),2));
}

double TSP::fitness(vector<string> camino){
    double result = 0.0;
    for(int i = 0; i < camino.size()-1; i++){
        result += distanciaEuclidiana(stoi(camino[i]), stoi(camino[i+1]));
    }
    return result;
}

vector<string> TSP::getCamino(){
    set<int> recorridos;
    vector<string> camino;
    camino.push_back(to_string(inicio));
    recorridos.insert(inicio);
    for(int i = 0; i < nPoints-1; i++){
        int newPoint = rand()%nPoints;
        while(recorridos.count(newPoint)) newPoint = rand()%nPoints;
        camino.push_back(to_string(newPoint));
        recorridos.insert(newPoint);
    }
    camino.push_back(to_string(inicio));
    return camino;
}

void TSP::generarPoblacion(){
    for(int i = 0; i < nPobla; i++){
        vector<string> camino = getCamino();
        poblacion[i] = make_tuple(camino, 1.0/(1.0+fitness(camino)));
    }
}

void TSP::seleccion(){
    sort(poblacion.begin(), poblacion.end(), compareByDouble);
    vector<TUPLE> nPoblacion(poblacion.begin()+1, poblacion.end());
    nPoblacion.push_back(poblacion.back());
    poblacion = nPoblacion;
}

void TSP::OXmuta(){
    vector<TUPLE> nPoblacion;
    for(int i = 0; i < nPobla; i++){
        vector<string> padre1, padre2;
        int indice1 = rand()%nPobla;
        int indice2 = rand()%nPobla;
        while(indice2 == indice1) indice2 = rand()%nPobla;
        padre1 = get<0>(poblacion[indice1]);
        padre2 = get<0>(poblacion[indice2]);
        int tam = padre1.size();
        vector<string> hijo(tam, " ");
        //OX
        int c1 = 1 + rand()%(tam-2);
        int c2 = 1 + rand()%(tam-2);
        while(c1 == c2) c2 = 1 + rand()%(tam-2);
        if(c1 > c2) swap(c1,c2);
        set<string> inter;
        for(int j = c1; j <= c2; j++){
            hijo[j] = padre1[j];
            inter.insert(padre1[j]);
        }
        stack<string> conjunto;
        for(int j = c1; j <= c2; j++) if(!inter.count(padre2[j])) conjunto.push(padre2[j]);
        for(int j = 1; j < c1; j++) if(!inter.count(padre2[j])) conjunto.push(padre2[j]);
        for(int j = c2+1; j < tam-1; j++)if(!inter.count(padre2[j])) conjunto.push(padre2[j]);
        for(int j = c2+1; j < tam-1; j++){
            hijo[j] = conjunto.top();
            conjunto.pop();
        }
        for(int j = 1; j < c1; j++){
            hijo[j] = conjunto.top();
            conjunto.pop();
        }
        
        for(int j = 0; j < hijo.size(); j++) if(hijo[j] == " ") hijo[j] = padre2[j];
        nPoblacion.push_back(make_tuple(hijo, 1.0/(1.0+fitness(hijo))));
        int ale = rand()%nPoblacion.size();
        vector<string> elegido = get<0>(nPoblacion[ale]);
        swap(elegido[c1], elegido[c2]);
        get<0>(nPoblacion[ale]) = elegido;
    }
    poblacion = nPoblacion;
}

vector<string> TSP::minimum(){
    vector<string> response;
    double mayor = -1e9;

    for(int i = 0; i < poblacion.size(); i++){
        if(get<1>(poblacion[i]) > mayor){
            mayor = get<1>(poblacion[i]);
            response = get<0>(poblacion[i]);
        }
    }
    return response;
}