#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "TSP.h"

using namespace std;

vector<pair<string,pair<double,double>>> nodePositions;

vector<vector<pair<string,pair<double,double>>>> aristas;

void drawCircle(double cx, double cy, double r, int segments) {
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < segments; i++) {
        double theta1 = 2.0 * M_PI * (double)i / (double)segments;
        double x1 = r * cos(theta1);
        double y1 = r * sin(theta1);

        double theta2 = 2.0 * M_PI * (double)(i + 1) / (double)segments;
        double x2 = r * cos(theta2);
        double y2 = r * sin(theta2);

        // Agregar un vértice central en (cx, cy) para formar triángulos
        glVertex2d(cx, cy);
        glVertex2d(cx + x1, cy + y1);
        glVertex2d(cx + x2, cy + y2);
    }
    glEnd();
}

// Función para dibujar un texto en OpenGL
void drawText(double x, double y, string text) {
    glRasterPos2d(x, y);
    for(int i = 0; i < text.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[i]);
    }
}

void drawEdge(double x1, double y1, double x2, double y2) {
    glBegin(GL_LINES);
    glVertex2d(x1, y1);
    glVertex2d(x2, y2);
    glEnd();
}

void display() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 1.0); // Color verde para las aristas
    for(int i = 0; i < aristas.size()-1; i++){
        for(int j = 0; j < nodePositions.size()-1; j++) drawEdge(aristas[i][j].second.first, aristas[i][j].second.second, aristas[i][j+1].second.first, aristas[i][j+1].second.second);
    }
    glColor3f(1.0, 0.0, 0.0);
    for(int j = 0; j < nodePositions.size()-1; j++) drawEdge(aristas[aristas.size()-1][j].second.first, aristas[aristas.size()-1][j].second.second, aristas[aristas.size()-1][j+1].second.first, aristas[aristas.size()-1][j+1].second.second);

    // Dibujar nodos y sus etiquetas
    for(int i = 0; i < nodePositions.size(); i++) {
        glColor3f(0.0, 0.0, 0.0); // Color blanco
        drawCircle(nodePositions[i].second.first, nodePositions[i].second.second, 0.05, 20); // Dibujar el círculo del nodo
        glColor3f(1.0, 1.0, 1.0); // Color negro
        drawText(nodePositions[i].second.first, nodePositions[i].second.second, nodePositions[i].first); // Dibujar la etiqueta en el centro del nodo
    }
    glFlush();
}

int main(int argc, char** argv){
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Graph");
    TSP viajero(12, 15, 0);
    int generacion = 200;
    aristas.resize(generacion);
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
    
    for(int i = 0; i < menor.size(); i++){
        nodePositions.push_back(make_pair(menor[i],viajero.grafo[menor[i]]));
        aristas[0].push_back(make_pair(menor[i],viajero.grafo[menor[i]]));
    }
    for(int i = 0; i < menor.size(); i++){
        cout << nodePositions[i].second.first << " " << nodePositions[i].second.second << endl;
        
    }
    for(int i = 2; i <= generacion; i++){
        nodePositions.clear();
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
        viajero.OXmuta();

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
        for(int j = 0; j < menor.size(); j++){
            nodePositions.push_back(make_pair(menor[j],viajero.grafo[menor[j]]));
            aristas[i-1].push_back(make_pair(menor[j],viajero.grafo[menor[j]]));
        }
        for(int j = 0; j < menor.size(); j++){
            cout << nodePositions[j].second.first << " " << nodePositions[j].second.second << endl;
        }
    }
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}