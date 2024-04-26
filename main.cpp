#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <random>
#include "ga1.h"
#include "ga2.h"
#include "ga3.h"

using namespace std;

vector<pair<float,float>> promedios;
vector<pair<float,float>> maximos;

pair<float,float> indiProm;
pair<float,float> indiMax;

float scaleToRange(float x, float min1, float max1) {
    return (x - min1) / (max1 - min1);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

    glColor3f(1.0, 1.0, 1.0); // Set color to white for the axes

    // Draw X-axis
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f); // Start point of X-axis
    glVertex2f(1.0f, 0.0f);  // End point of X-axis
    glEnd();

    // Draw Y-axis
    glBegin(GL_LINES);
    glVertex2f(0.0f, 1.0f); // Start point of Y-axis
    glVertex2f(0.0f, -1.0f);  // End point of Y-axis
    glEnd();
    
    // for(int i = 0; i < promedios.size(); i++){
    //     glBegin(GL_POINTS); // Begin drawing lines
    //     // Specify the vertices of the line
    //     glVertex2f(promedios[i].first, promedios[i].second); // First endpoint of the line
    // }


    glColor3f(1.0, 0.0, 0.0); // Set color to red
    glBegin(GL_LINES); // Begin drawing lines
    for(int i = 0; i < promedios.size()-1; i++){
        float x1 = scaleToRange(promedios[i].first, 0.0f, indiMax.first); // Escala entre -1 y 1
        float y1 = scaleToRange(promedios[i].second, 0.0f, (float)indiMax.second); // Escala entre -1 y 1
        float x2 = scaleToRange(promedios[i+1].first, 0.0f, indiMax.first); // Escala entre -1 y 1
        float y2 = scaleToRange(promedios[i+1].second, 0.0f, (float)indiMax.second);// Escala entre -1 y 1
        x1 = scaleToRange(x1, 0.0f, 1.0f);
        y1 = scaleToRange(y1, 0.0f, 1.0f);
        x2 = scaleToRange(x2, 0.0f, 1.0f);
        y2 = scaleToRange(y2, 0.0f, 1.0f);
        glVertex2f(x1, y1); // Primer punto de la línea
        glVertex2f(x2, y2); // Segundo punto de la línea
    }
    glEnd(); // End drawing lines

    glColor3f(0.0, 1.0, 0.0); // Set color to red
    glBegin(GL_LINES); // Begin drawing lines
    for(int i = 0; i < maximos.size()-1; i++){
        float x1 = scaleToRange(maximos[i].first, 0.0f, indiMax.first); // Escala entre -1 y 1
        float y1 = scaleToRange(maximos[i].second, 0.0f, (float)indiMax.second); // Escala entre -1 y 1
        float x2 = scaleToRange(maximos[i+1].first, 0.0f, indiMax.first); // Escala entre -1 y 1
        float y2 = scaleToRange(maximos[i+1].second, 0.0f, (float)indiMax.second);// Escala entre -1 y 1
        x1 = scaleToRange(x1, 0.0f, 1.0f);
        y1 = scaleToRange(y1, 0.0f, 1.0f);
        x2 = scaleToRange(x2, 0.0f, 1.0f);
        y2 = scaleToRange(y2, 0.0f, 1.0f);
        // cout << x1 << " " << y1 << endl;
        glVertex2f(x1, y1); // Primer punto de la línea
        glVertex2f(x2, y2); // Segundo punto de la línea
    }
    glEnd(); // End drawing lines

    glFlush(); // Flush the OpenGL pipeline
}

void init() {
    glClearColor(0.1, 0.1, 0.1, 0.0); // Set the clear color to black
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.1, 1.1, -1.1, 1.1, -1.1, 1.1); // Set up the coordinate system
}


int main(int argc, char** argv){
    srand(time(NULL));
    float gen = 100.0f;
    float maxP = -1e9f;
    float maxM = -1e9f;
    int nPoblacion = 15;
    bool mini = false;
    int opt = 3;
    if(opt == 1){
        GA1 algo1(nPoblacion,mini, make_pair(0,15));
        algo1.genPoblacion();
        for(int i = 0; i < algo1.nPobla; i++){
            cout << get<0>(algo1.poblacion[i]) << ": ";
            cout << get<1>(algo1.poblacion[i]) << " -> ";
            cout << get<2>(algo1.poblacion[i]) << endl;
        }
        float prom = algo1.getPromedio();
        float maxi = algo1.getMaximo();
        cout << "Promedio: " << prom << endl;
        cout << "Maximo: " << maxi << endl;
        if(algo1.minimizar){
            maximos.push_back(make_pair(1.0f,maxi));
            promedios.push_back(make_pair(1.0f,prom));
        }
        else{
            maximos.push_back(make_pair(1.0f,maxi));
            promedios.push_back(make_pair(1.0f,prom));
        }
        maxP = max(prom, maxP);
        maxM = max(maxi, maxM);
        for(int i = 2;i <= gen; i++){
            cout << endl;
            algo1.seleccion();
            // for(int i = 0; i < algo.nPobla; i++){
            //     cout << get<0>(algo.poblacion[i]) << ": ";
            //     cout << get<1>(algo.poblacion[i]) << " -> ";
            //     cout << get<2>(algo.poblacion[i]) << endl;
            // }
            cout << endl;
            algo1.cruceMuta();
            for(int i = 0; i < algo1.nPobla; i++){
                cout << get<0>(algo1.poblacion[i]) << ": ";
                cout << get<1>(algo1.poblacion[i]) << " -> ";
                cout << get<2>(algo1.poblacion[i]) << endl;
            }
            prom = algo1.getPromedio();
            maxi = algo1.getMaximo();
            cout << "Promedio: " << prom << endl;
            cout << "Maximo: " << maxi << endl;
            if(algo1.minimizar){
                maximos.push_back(make_pair((float)i,maxi));
                promedios.push_back(make_pair((float)i,prom));
            }
            else{
                maximos.push_back(make_pair((float)i,maxi));
                promedios.push_back(make_pair((float)i,prom));
            }
            maxP = max(prom, maxP);
            maxM = max(maxi, maxM);
        }
    }
    else if(opt == 2){
        vector<pair<int,int>> limits;
        limits.push_back(make_pair(0,15));
        limits.push_back(make_pair(0,15));
        GA2 algo2(nPoblacion,mini, limits);
        algo2.genPoblacion();
        for(int i = 0; i < algo2.nPobla; i++){
            cout << get<0>(algo2.poblacion[i]) << " " << get<1>(algo2.poblacion[i]) << ": ";
            cout << get<2>(algo2.poblacion[i]) << " " << get<3>(algo2.poblacion[i]) << " -> ";
            cout << get<4>(algo2.poblacion[i]) << endl;
        }
        float prom = algo2.getPromedio();
        float maxi = algo2.getMaximo();
        cout << "Promedio: " << prom << endl;
        cout << "Maximo: " << maxi << endl;
        if(algo2.minimizar){
            maximos.push_back(make_pair(1.0f,maxi));
            promedios.push_back(make_pair(1.0f,prom));
        }
        else{
            maximos.push_back(make_pair(1.0f,maxi));
            promedios.push_back(make_pair(1.0f,prom));
        }
        maxP = max(prom, maxP);
        maxM = max(maxi, maxM);
        for(int i = 2;i <= gen; i++){
            cout << endl;
            algo2.seleccion();
            // for(int i = 0; i < algo2.nPobla; i++){
            //     cout << get<0>(algo2.poblacion[i]) << " " << get<1>(algo2.poblacion[i]) << ": ";
            //     cout << get<2>(algo2.poblacion[i]) << " " << get<3>(algo2.poblacion[i]) << " -> ";
            //     cout << get<4>(algo2.poblacion[i]) << endl;
            // }
            cout << endl;
            algo2.cruceMuta();
            for(int i = 0; i < algo2.nPobla; i++){
                cout << get<0>(algo2.poblacion[i]) << " " << get<1>(algo2.poblacion[i]) << ": ";
                cout << get<2>(algo2.poblacion[i]) << " " << get<3>(algo2.poblacion[i]) << " -> ";
                cout << get<4>(algo2.poblacion[i]) << endl;
            }
            prom = algo2.getPromedio();
            maxi = algo2.getMaximo();
            cout << "Promedio: " << prom << endl;
            cout << "Maximo: " << maxi << endl;
            if(algo2.minimizar){
                maximos.push_back(make_pair((float)i,maxi));
                promedios.push_back(make_pair((float)i,prom));
            }
            else{
                maximos.push_back(make_pair((float)i,maxi));
                promedios.push_back(make_pair((float)i,prom));
            }
            maxP = max(prom, maxP);
            maxM = max(maxi, maxM);
        }
    }
    else if(opt == 3){
        vector<pair<int,int>> limits;
        limits.push_back(make_pair(0,15));
        limits.push_back(make_pair(0,15));
        limits.push_back(make_pair(0,15));
        GA3 algo3(nPoblacion,mini, limits);
        algo3.genPoblacion();
        for(int i = 0; i < algo3.nPobla; i++){
            cout << get<0>(algo3.poblacion[i]) << " " << get<1>(algo3.poblacion[i]) << " " << get<2>(algo3.poblacion[i]) << ": ";
            cout << get<3>(algo3.poblacion[i]) << " " << get<4>(algo3.poblacion[i]) << " " << get<5>(algo3.poblacion[i]) << " -> ";
            cout << get<6>(algo3.poblacion[i]) << endl;
        }
        float prom = algo3.getPromedio();
        float maxi = algo3.getMaximo();
        cout << "Promedio: " << prom << endl;
        cout << "Maximo: " << maxi << endl;
        if(algo3.minimizar){
            maximos.push_back(make_pair(1.0f,maxi));
            promedios.push_back(make_pair(1.0f,prom));
        }
        else{
            maximos.push_back(make_pair(1.0f,maxi));
            promedios.push_back(make_pair(1.0f,prom));
        }
        maxP = max(prom, maxP);
        maxM = max(maxi, maxM);
        for(int i = 2;i <= gen; i++){
            cout << endl;
            algo3.seleccion();
            // for(int i = 0; i < algo3.nPobla; i++){
            //     cout << get<0>(algo3.poblacion[i]) << " " << get<1>(algo3.poblacion[i]) << " " << get<2>(algo3.poblacion[i]) << ": ";
            //     cout << get<3>(algo3.poblacion[i]) << " " << get<4>(algo3.poblacion[i]) << " " << get<5>(algo3.poblacion[i]) << " -> ";
            //     cout << get<6>(algo3.poblacion[i]) << endl;
            // }
            cout << endl;
            algo3.cruceMuta();
            for(int i = 0; i < algo3.nPobla; i++){
                cout << get<0>(algo3.poblacion[i]) << " " << get<1>(algo3.poblacion[i]) << " " << get<2>(algo3.poblacion[i]) << ": ";
                cout << get<3>(algo3.poblacion[i]) << " " << get<4>(algo3.poblacion[i]) << " " << get<5>(algo3.poblacion[i]) << " -> ";
                cout << get<6>(algo3.poblacion[i]) << endl;
            }
            prom = algo3.getPromedio();
            maxi = algo3.getMaximo();
            cout << "Promedio: " << prom << endl;
            cout << "Maximo: " << maxi << endl;
            if(algo3.minimizar){
                maximos.push_back(make_pair((float)i,maxi));
                promedios.push_back(make_pair((float)i,prom));
            }
            else{
                maximos.push_back(make_pair((float)i,maxi));
                promedios.push_back(make_pair((float)i,prom));
            }
            maxP = max(prom, maxP);
            maxM = max(maxi, maxM);
        }
    }
    indiProm.first = gen;
    indiProm.second = maxP;
    indiMax.first = gen;
    indiMax.second = maxM;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500); 
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Line Example");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}
//g++ -std=c++14 main.cpp -o main -framework OpenGL -framework GLUT