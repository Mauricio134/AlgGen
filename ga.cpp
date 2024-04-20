#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <random>

using namespace std;

class GA1{
public:
    vector<pair<string,int>>  valores;
    vector<int> fit;
    pair<int, int> limites;
    int poblacion;
    int generaciones;
    float promedio;
    float maximo;
    bool minimizar;
    int tamBinario;
    int nSeleccion;
    int corte;
    
    GA1(pair<int,int>, int, int, int, int, bool);
    void generarPoblacion();
    int tamBinarios();
    string getBinario(int , int);
    int fitness(int);
    void getFitness();
    void getAverMaxFitness();
    vector<pair<string, int>> seleccion();
    void corteMuta(vector<pair<string,int>> );
    int reacalNumber(string );
};

GA1::GA1(pair<int,int> l, int p, int g, int c, int ns, bool m){
    limites = l;
    minimizar = m;
    poblacion = p;
    generaciones = g;
    maximo = 0.0f;
    promedio = 0.0f;
    tamBinario = 0;
    corte = c;
    nSeleccion = ns;
    valores.resize(p);
    fit.resize(p);
}

void GA1::generarPoblacion(){
    srand((unsigned) time(NULL));
    tamBinario = tamBinarios();
    for(int i = 0; i < poblacion; i++){
        valores[i].second = limites.first + (rand() % (limites.second - limites.first+1));
        valores[i].first = getBinario(valores[i].second, tamBinario);
    }
}

int GA1::tamBinarios(){
    int counter = 0;
    int number = limites.second;
    while(number){
        counter++;
        number/=2;
    }
    counter++;
    return counter;
}

string GA1::getBinario(int number, int tam){
    string bin;
    while(number){
        bin = to_string(number%2) + bin;
        number/=2;
    }
    int t = bin.size();
    if(t < tam){
        for(int i = t; i < tam; i++){
            bin = "0"+bin;
        }
    }
    return bin;
}

int GA1::fitness(int x){
    return pow(x,2);
    // return x*sin(x) + pow(x,2)*cos(x);
}

void GA1::getFitness(){
    for(int i = 0; i < poblacion; i++){
        fit[i] = fitness(valores[i].second);
    }
    return;
}

void GA1::getAverMaxFitness(){
    if(minimizar) maximo = numeric_limits<float>::max();
    else maximo = 0.0f;
    for(int i = 0; i < poblacion; i++){
        promedio += (float)fit[i];
        if(minimizar){
            maximo = min((float)fit[i], maximo);
        }else{
            maximo = max((float)fit[i], maximo);
        }
    }
    promedio = (float)promedio/(float)poblacion;
    return;
}

vector<pair<string, int>> GA1::seleccion() {
    if(!minimizar) sort(valores.begin(), valores.end(), [this](const pair<string,int> & p1, const pair<string, int> & p2){
        float case1 = (float) this->fitness(p1.second)/(float)promedio;
        float case2 = (float) this->fitness(p2.second)/(float)promedio;
        return (int)case1 > (int)case2;
    });
    else sort(valores.begin(), valores.end(), [this](const pair<string,int> & p1, const pair<string, int> & p2){
        float case1 = (float) this->fitness(p1.second)/(float)promedio;
        float case2 = (float) this->fitness(p2.second)/(float)promedio;
        return (int)case1 < (int)case2;
    });
    vector<pair<string,int>> result(valores.begin(), valores.begin()+ nSeleccion);
    return result;
}

int GA1::reacalNumber(string binario){
    int suma = 0;
    for(int i = tamBinario-1, j = 0; i > -1; i--, j++) if(binario[i] != '0') suma += pow(2,j);
    return suma;
}

void GA1::corteMuta(vector<pair<string,int>> sel){
    vector<pair<string, int>> nValores;
    int cantSel = sel.size();
    for(int i = 0; i< cantSel; i++){
        nValores.push_back(sel[i]);
    }
    int indMuta = rand() % poblacion;
    for(int i = 0; i < poblacion-nSeleccion; i++){
        pair<string,string> padre;
        string hijo;
        int i1 = rand() % nSeleccion;
        int i2 = rand() % nSeleccion;
        while(i2 == i1){
            i2 = rand() % nSeleccion;
        }
        padre = make_pair(sel[i1].first, sel[i2].first);

        hijo += padre.first.substr(0, corte);
        hijo += padre.second.substr(corte);

        if(i == indMuta){
            // cout << "Yo mute: " << endl;
            int bit = rand()%tamBinario;
            if(hijo[bit] == '0') hijo[bit] = '1';
            else hijo[bit] = '0';
        }
        int number = reacalNumber(hijo);
        while(number < limites.first || limites.second < number){
            hijo = "";
            i1 = rand() % nSeleccion;
            i2 = rand() % nSeleccion;
            while(i2 == i1){
                i2 = rand() % nSeleccion;
            }
            padre = make_pair(sel[i1].first, sel[i2].first);

            hijo += padre.first.substr(0, corte);
            hijo += padre.second.substr(corte);

            if(i == indMuta){
                // cout << "Yo mute: " << endl;
                int bit = rand()%tamBinario;
                if(hijo[bit] == '0') hijo[bit] = '1';
                else hijo[bit] = '0';
            }
            number = reacalNumber(hijo);
        }
        nValores.push_back(make_pair(hijo,number));
    }
    valores = nValores;
}
int main(){
    GA1 algoritmo(
        make_pair(0,17),
        4,
        10,
        2,
        3,
        true
    );
    algoritmo.generarPoblacion();
    algoritmo.getFitness();
    algoritmo.getAverMaxFitness();
    for(int i = 0; i < algoritmo.poblacion; i++){
        cout << algoritmo.valores[i].first << " :";
        cout << algoritmo.valores[i].second << endl;
    }

    cout << "Promedio: " << algoritmo.promedio << endl;
    cout << "Maximo: " << algoritmo.maximo << endl;
    for(int k = 1; k < algoritmo.generaciones; k++){
        // for(int i = 0; i < algoritmo.poblacion; i++){
        //     cout << algoritmo.valores[i].first << " :";
        //     cout << algoritmo.valores[i].second << endl;
        // }

        vector<pair<string, int>> sel = algoritmo.seleccion();
        // for(int i = 0; i < sel.size(); i++){
        //     cout << sel[i].first << " :";
        //     cout << sel[i].second << endl;
        // }

        algoritmo.corteMuta(sel);
        algoritmo.getFitness();
        algoritmo.getAverMaxFitness();
        // for(int i = 0; i < algoritmo.poblacion; i++){
        //     cout << algoritmo.valores[i].first << " :";
        //     cout << algoritmo.valores[i].second << endl;
        // }
        cout << "Promedio: " << algoritmo.promedio << endl;
        cout << "Maximo: " << algoritmo.maximo << endl;
    }
    return 0;
}