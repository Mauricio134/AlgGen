#include <iostream>
#include <cmath>
#include <bitset>
#include<cstdlib>

using namespace std;

class AlgGen{
public:
    int generaciones;
    int nVariables;
    int nPopulation;
    vector<int> limites;
    string MaxMin;
    vector<vector<int>> aleatorios;

    AlgGen(int , int ,int ,vector<int> , string );
    int fitness();
    void creadorAl();
};

AlgGen::AlgGen(int g, int nV, int nP, vector<int> l, string Mm){
    generaciones = g;
    nVariables = nV;
    limites = l;
    MaxMin = Mm;
    nPopulation = nP;
}

void AlgGen::creadorAl(){
    aleatorios.resize(nPopulation);
    for(int i = 0; i < nPopulation; i++){
        aleatorios[i].resize(nVariables);
    }
    for(int i = 0; i < nPopulation; i++){
        for(int j = 0; j < nVariables; j++){
            aleatorios[i][j] = limites[j*2] + rand()%limites[j*2+1];
        }
    }
    return;
}

int main(){
    vector<int> l = {3,4,2,3,6,10};
    AlgGen f1(4,3,4,l,"Maximizar");
    f1.creadorAl();
    for(int i = 0; i < f1.aleatorios.size(); i++){
        for(int j = 0; j < f1.aleatorios[i].size();j++ ){
            cout << f1.aleatorios[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}