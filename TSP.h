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
    void PMXmuta();
    vector<string> PMX(TUPLE, TUPLE, int, int);    
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
        double randomX = scaledX * 2.0f - 1.0f;
        double scaledY = static_cast<double>(randomInt[1]) / RAND_MAX;
        double randomY = scaledY * 2.0f - 1.0f;
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

vector<string> TSP::PMX(TUPLE p1, TUPLE p2, int c1, int c2){
    vector<string> resultado(nPoints+1,"");
    vector<string> i, j;
    for(int k = c1; k <= c2; k++){
        resultado[k] = get<0>(p1)[k];
        bool presence = false;
        for(int t = c1; t <= c2; t++){
            if(get<0>(p2)[k] == get<0>(p1)[t]){
                presence = true;
                break;
            }
        }
        if(!presence){
            i.push_back(get<0>(p2)[k]);
            j.push_back(get<0>(p1)[k]);
        }
    }
    vector<string> padre1 = get<0>(p1);
    vector<string> padre2 = get<0>(p2);
    for(int k = 0; k < j.size(); k++){
        for(int r = 0; r < padre2.size(); r++){
            if(padre2[r] == j[k]){
                if(r < c1 || r > c2){
                    resultado[r] = i[k];
                }
                else{
                    j[k] = padre1[r];
                    k--;
                }
            }
        }
    }
    for(int k = 0; k < resultado.size(); k++){
        if(resultado[k] == ""){
            resultado[k] = get<0>(p2)[k];
        }
    }
    return resultado;
}

void TSP::PMXmuta(){
    vector<TUPLE> nPoblacion;
    for(int i = 0; i < nPobla; i++){
        TUPLE padre1, padre2;
        vector<string> hijo;
        int indice1 = rand()%nPobla;
        int indice2 = rand()%nPobla;
        while(indice2 == indice1) indice2 = rand()%nPobla;
        int corte1 = 1 + rand()%(nPoints-1);
        int corte2 = 1 + rand()%(nPoints-1);
        while(corte2 <= corte1){
            corte1 = 1 + rand()%(nPoints-1);
            corte2 = 1 + rand()%(nPoints-1);
        }

        padre1 = poblacion[indice1];
        padre2 = poblacion[indice2];
        hijo = PMX(padre1, padre2, corte1, corte2);
        // cout << "Rango: " <<  corte1 << " " << corte2 << endl;
        // for(int h = 0; h < hijo.size(); h++){
        //     cout << hijo[h];
        // }
        // cout << endl;
        vector<string> hijoMutado(corte2-corte1+1);
        set<int> iteradores;
        for(int j = 0; j < hijoMutado.size(); j++){
            int r = corte1 + rand()%(hijoMutado.size());
            while(iteradores.count(r)) r = corte1 + rand()%(hijoMutado.size());
            // cout << "R: " << r << endl;
            hijoMutado[j] = hijo[r];
            // for(int h = 0; h < hijoMutado.size(); h++){
            //     cout << hijoMutado[h];
            // }
            // cout << endl;
            iteradores.insert(r);
        }
        for(int j = 0, k = corte1; j < hijoMutado.size(); j++, k++){
            hijo[k] = hijoMutado[j];
        }
        // for(int h = 0; h < hijo.size(); h++){
        //     cout << hijo[h];
        // }
        // cout << endl;
        nPoblacion.push_back(make_tuple(hijo, 1.0/(1.0+fitness(hijo))));
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