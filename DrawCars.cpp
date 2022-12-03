#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#pragma once

using namespace std;

//Classe para instanciar cada um dos carros inimigos
class DrawCars{
    private:
        float RotacaoPneu;

    public:
        DrawCars(float RotacaoPneu){
            this->RotacaoPneu = RotacaoPneu;
        }
        //Essa função desenhan um cilindro de x lados, com suas "tampas", para assim representar os pneus ao sair do corpo do carro
        void drawFilledCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat faces, GLfloat altura, GLint R, GLint G, GLint B){
            int i;
            int QtdTriangulos = faces;
            
            GLfloat twicePi = 2.0f * M_PI;
            
            glBegin(GL_POLYGON);
                glColor3f(R,G,B);
                glVertex3f(x, y, z);
                for(i = 0; i <= QtdTriangulos;i++) { 
                    glVertex3f(
                        x + (radius * cos(i *  twicePi / QtdTriangulos)), 
                        y + (radius * sin(i * twicePi / QtdTriangulos)),
                        z
                    );
                }
            glEnd();

            glBegin(GL_POLYGON);
                glColor3f(R,G,B);
                glVertex3f(x, y, z+altura);
                for(i = 0; i <= QtdTriangulos;i++) { 
                    glVertex3f(
                        x + (radius * cos(i *  twicePi / QtdTriangulos)), 
                        y + (radius * sin(i * twicePi / QtdTriangulos)),
                        z+altura
                    );
                }
            glEnd();

            glBegin(GL_QUAD_STRIP);
                glColor3f(R,G,B);
                for(i = 0; i <= QtdTriangulos;i++) { 
                    glVertex3f(
                        x + (radius * cos(i *  twicePi / QtdTriangulos)), 
                        y + (radius * sin(i * twicePi / QtdTriangulos)),
                        z
                    );
                    glVertex3f(
                        x + (radius * cos(i *  twicePi / QtdTriangulos)), 
                        y + (radius * sin(i * twicePi / QtdTriangulos)),
                        z + altura
                    );
                }
            glEnd();
        }

        //Funcao para desenhar os pneus, levar eles para dentro dos carros, e rotacionar eles, caso o carro estaja em movimento
        void drawPneus(GLfloat X, GLfloat Y, GLfloat Z){
            glPushMatrix();
            glTranslatef(X,Y,Z);
            glRotatef(this->RotacaoPneu,1,0,0);
            glRotatef(90,0,1,0);
            drawFilledCircle(0,0,0,0.12,8,1.3,0,0,1);
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            drawFilledCircle(0,0,0,0.12,8,1.3,0,0,0);
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glPopMatrix();
        }

        //Função para desenhar o carro na posicao 0,0,0
        void drawCar(int R, int G, int B){
            glColor3f(R,G,B);
            glBegin(GL_QUADS);
                glVertex3f(0,0,0);
                glVertex3f(1,0,0);
                glVertex3f(1,0.5,0);
                glVertex3f(0,0.5,0);
            glEnd();
            glBegin(GL_QUADS);
                glVertex3f(0,0,0);
                glVertex3f(1,0,0);
                glVertex3f(1,0,0.4);
                glVertex3f(0,0,0.4);
            glEnd();
            glBegin(GL_QUADS);
                glVertex3f(0,0,0);
                glVertex3f(0,0,0.4);
                glVertex3f(0,0.5,0.4);
                glVertex3f(0,0.5,0);
            glEnd();
            glBegin(GL_QUADS);
                glVertex3f(1,0,0);
                glVertex3f(1,0,0.4);
                glVertex3f(1,0.5,0.4);
                glVertex3f(1,0.5,0);
            glEnd();
            glBegin(GL_QUADS);
                glVertex3f(0,0,0.4);
                glVertex3f(1,0,0.4);
                glVertex3f(1,0.5,0.4);
                glVertex3f(0,0.5,0.4);
            glEnd();
            glBegin(GL_QUADS);
                glVertex3f(0,0.5,0.4);
                glVertex3f(0,0.5,0);
                glVertex3f(1,0.5,0);
                glVertex3f(1,0.5,0.4);
            glEnd();

            glColor3f(0,0,0);
            glBegin(GL_LINES);
                glVertex3f(0,0,0);
                glVertex3f(1,0,0);
                glVertex3f(1,0.5,0);
                glVertex3f(0,0.5,0);
            glEnd();
            glBegin(GL_LINES);
                glVertex3f(0,0,0);
                glVertex3f(1,0,0);
                glVertex3f(1,0,0.4);
                glVertex3f(0,0,0.4);
            glEnd();
            glBegin(GL_LINES);
                glVertex3f(0,0,0);
                glVertex3f(0,0,0.4);
                glVertex3f(0,0.5,0.4);
                glVertex3f(0,0.5,0);
            glEnd();
            glBegin(GL_LINES);
                glVertex3f(1,0,0);
                glVertex3f(1,0,0.4);
                glVertex3f(1,0.5,0.4);
                glVertex3f(1,0.5,0);
            glEnd();
            glBegin(GL_LINES);
                glVertex3f(0,0,0.4);
                glVertex3f(1,0,0.4);
                glVertex3f(1,0.5,0.4);
                glVertex3f(0,0.5,0.4);
            glEnd();
            glBegin(GL_LINES);
                glVertex3f(0,0.5,0.4);
                glVertex3f(0,0.5,0);
                glVertex3f(1,0.5,0);
                glVertex3f(1,0.5,0.4);
            glEnd();
        }
};