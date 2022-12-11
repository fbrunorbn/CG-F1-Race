#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#pragma once

using namespace std;

//Classe para instanciar o semafóro que conterá as texturas "3"-"2"-"1"-"GO"
class SemaforoLargada{
    private:
        float PosX;
        float PosY;
        float PosZ;

    public:

        SemaforoLargada(float PosX, float PosY, float PosZ){
            this->PosX = PosX;
            this->PosY = PosY;
            this->PosZ = PosZ;
        }

        //Desenha o cubo com sua textura para o semaforo
        void Cube(GLfloat X, GLfloat Y, GLfloat Z,GLuint text_ID){
            glColor3f(X,Y,Z);
            glBindTexture(GL_TEXTURE_2D, text_ID);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
                glTexCoord2f(1.0,0.0); glVertex3f(1,-1,-1);
                glTexCoord2f(1.0,1.0); glVertex3f(1,-1,1);
                glTexCoord2f(0.0,1.0); glVertex3f(-1,-1,1);
            glEnd();
            glBegin(GL_QUADS);
                glTexCoord2f(0.0,0.0); glVertex3f(-1,1,-1);
                glTexCoord2f(1.0,0.0); glVertex3f(1,1,-1);
                glTexCoord2f(1.0,1.0); glVertex3f(1,1,1);
                glTexCoord2f(0.0,1.0); glVertex3f(-1,1,1);
            glEnd();
            glBegin(GL_QUADS);
                glTexCoord2f(0.0,0.0); glVertex3f(1,-1,-1);
                glTexCoord2f(1.0,0.0); glVertex3f(1,1,-1);
                glTexCoord2f(1.0,1.0); glVertex3f(1,1,1);
                glTexCoord2f(0.0,1.0); glVertex3f(1,-1,1);
            glEnd();
            glBegin(GL_QUADS);
                glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
                glTexCoord2f(1.0,0.0); glVertex3f(-1,1,-1);
                glTexCoord2f(1.0,1.0); glVertex3f(-1,1,1);
                glTexCoord2f(0.0,1.0); glVertex3f(-1,-1,1);
            glEnd();
            glBegin(GL_QUADS);
                glVertex3f(-1,-1,-1);
                glVertex3f(1,-1,-1);
                glTexCoord2f(1.0,1.0); glVertex3f(1,1,-1);
                glTexCoord2f(0.0,1.0); glVertex3f(-1,1,-1);
            glEnd();
            glBegin(GL_QUADS);
                glVertex3f(-1,-1,1);
                glVertex3f(1,-1,1);
                glVertex3f(1,1,1);
                glVertex3f(-1,1,1);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        //É desenhando o semaforo no 0,0,0, rotacionado para mostrar a textura de frente para a camera, e transladado para sua posição acima dos carros
        void drawSemaphore(float RotacaoSemaforo, GLuint text_ID){
            glPushMatrix();
            glTranslatef(this->PosX, this->PosY, this->PosZ);
            glRotatef(RotacaoSemaforo,0,0,1);
            glScalef(2,2,1.5);
            Cube(1,1,1,text_ID);
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            Cube(0,0,0,text_ID);
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glPopMatrix();
        }
};