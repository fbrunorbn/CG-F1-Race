#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Luz.cpp"
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
        void drawCar(int R, int G, int B, Luz &luz, float PosX, float PosY, float PosZ){
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
            glm::vec3 P,C;
            glm::vec3 cor = glm::vec3(R,G,B);
            glColor3f(cor.r, cor.g, cor.b);
            luz.setEspecularMaterial(glm::vec3(0.5));

            glBegin(GL_QUADS);
                //Cima
                P = glm::vec3((0.0+PosX),(0.0+PosY),(0.4,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(0.0,0.0,0.4);

                P = glm::vec3((1+PosX),(0+PosY),(0.4,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(1,0,0.4);

                P = glm::vec3((1+PosX),(0.5+PosY),(0.4,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(1,0.5,0.4);

                P = glm::vec3((0.0+PosX),(0.5+PosY),(0.4,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(0,0.5,0.4);
            glEnd();
            glBegin(GL_QUADS);
                //Atras
                P = glm::vec3((0.0+PosX),(0.0+PosY),(0.0,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(0.0,0.0,0.0);

                P = glm::vec3((1.0+PosX),(0.0+PosY),(0.0,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(1.0,0.0,0.0);
                
                P = glm::vec3((1+PosX),(0.0+PosY),(0.4,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(1.0,0.0,0.4);

                P = glm::vec3((0.0+PosX),(0.0+PosY),(0.4,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(0.0,0.0,0.4);

            glEnd();

            glBegin(GL_QUADS);
                //Baixo
                P = glm::vec3((0.0+PosX),(0.0+PosY),(0.0,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,0.0,-1.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(0,0,0);

                P = glm::vec3((1+PosX),(0+PosY),(0,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,0.0,-1.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(1,0,0);

                P = glm::vec3((1+PosX),(0.5+PosY),(0,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,0.0,-1.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(1,0.5,0);

                P = glm::vec3((0.0+PosX),(0.5+PosY),(0.0,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,0.0,-1.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(0,0.5,0);
            glEnd();
            glBegin(GL_QUADS);
                //Frente
                P = glm::vec3((0.0+PosX),(0.5+PosY),(0.0,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(0,0.5,0);

                P = glm::vec3((1.0+PosX),(0.5+PosY),(0.0,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(1,0.5,0);

                P = glm::vec3((1.0+PosX),(0.5+PosY),(0.4,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(1,0.5,0.4);

                P = glm::vec3((0.0+PosX),(0.5+PosY),(0.4,PosZ));
                C = luz.ilumina(P,glm::vec3(0.0,1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(0,0.5,0.4);
            glEnd();
            glBegin(GL_QUADS);
                //Esquerda
                P = glm::vec3((0.0+PosX),(0.0+PosY),(0.0,PosZ));
                C = luz.ilumina(P,glm::vec3(-1.0,0.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(0,0,0);

                P = glm::vec3((0.0+PosX),(0.0+PosY),(0.4,PosZ));
                C = luz.ilumina(P,glm::vec3(-1.0,0.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(0,0,0.4);

                P = glm::vec3((0.0+PosX),(0.5+PosY),(0.4,PosZ));
                C = luz.ilumina(P,glm::vec3(-1.0,0.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(0,0.5,0.4);
                
                P = glm::vec3((0.0+PosX),(0.5+PosY),(0.0,PosZ));
                C = luz.ilumina(P,glm::vec3(-1.0,0.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(0,0.5,0);
            glEnd();
            glBegin(GL_QUADS);
                //Direita
                P = glm::vec3((1.0+PosX),(0.0+PosY),(0.0,PosZ));
                C = luz.ilumina(P,glm::vec3(1.0,0.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(1,0,0);

                P = glm::vec3((1.0+PosX),(0.0+PosY),(0.4,PosZ));
                C = luz.ilumina(P,glm::vec3(1.0,0.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(1,0,0.4);

                P = glm::vec3((1.0+PosX),(0.5+PosY),(0.4,PosZ));
                C = luz.ilumina(P,glm::vec3(1.0,0.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(1,0.5,0.4);

                P = glm::vec3((1.0+PosX),(0.5+PosY),(0.0,PosZ));
                C = luz.ilumina(P,glm::vec3(1.0,0.0,0.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(1,0.5,0);
            glEnd();
        }
};