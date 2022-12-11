#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Luz.cpp"

using namespace std;

//Classe para instanciar os postes
class Post{
    private:
        float PosX;
        float PosY;
        float PosZ;
        float velocidade;
        GLuint textID;

    public:
        Post(float PosX, float PosY, float PosZ){
            this->PosX = PosX;
            this->PosY = PosY;
            this->PosZ = PosZ;
            this->velocidade = 0.0; //Tem velocidade 0, seu movimento se dá somente em relação a velocidade relativa ao meu carro
        }

        //Definindo a velocidade do objeto
        void DefineVelo(float velocidade, float MaxVelo){
            this->velocidade = velocidade/(MaxVelo*2);
        }

        //Fazendo a translação dele para sua posição correta
        void MoverObjetoOffPista(){
            this->PosY -= this->velocidade;
            if (PosY <= -40.0){
                PosY += 80.0;
            }
        }

        float getPosX(){
            return this->PosX;
        }

        float getPosY(){
            return PosY;
        }

        float getPosZ(){
            return PosZ;
        }

        //Base do poste
        void drawPoste(GLfloat x, GLfloat y, GLfloat z, GLfloat lado, GLfloat altura,GLfloat R, GLfloat G, GLfloat B,GLuint text_ID_Poste,Luz &luz){
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
            glm::vec3 P,C;
            glm::vec3 cor = glm::vec3(R,G,B);
            glColor3f(cor.r, cor.g, cor.b);
            glBindTexture(GL_TEXTURE_2D, text_ID_Poste);
            glBegin(GL_QUAD_STRIP);
                P = glm::vec3(-0.5 + PosX, -0.5 +PosY, z+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(0.0,0.0); glVertex3f(-0.5, -0.5, z);

                P = glm::vec3(0.5 + PosX, -0.5 +PosY, z+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(1.0,0.0); glVertex3f(0.5, -0.5, z);

                P = glm::vec3(0.5 + PosX, -0.5 +PosY, altura+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(1.0,4.0); glVertex3f(0.5, -0.5, altura);

                P = glm::vec3(-0.5 + PosX, -0.5 +PosY, altura+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(0.0,4.0); glVertex3f(-0.5, -0.5, altura);

                P = glm::vec3(-0.5 + PosX, -0.5 +PosY, z+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(0.0,0.0); glVertex3f(-0.5, -0.5, z);

                P = glm::vec3(-0.5 + PosX, 0.5 +PosY, z+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(1.0,0.0); glVertex3f(-0.5, 0.5, z);

                P = glm::vec3(-0.5 + PosX, 0.5 +PosY, altura+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(1.0,4.0); glVertex3f(-0.5, 0.5, altura);

                P = glm::vec3(-0.5 + PosX, -0.5 +PosY, altura+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(0.0,4.0); glVertex3f(-0.5, -0.5, altura);

                P = glm::vec3(0.5 + PosX, -0.5 +PosY, z+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(0.0,0.0); glVertex3f(0.5, -0.5, z);

                P = glm::vec3(0.5 + PosX, 0.5 +PosY, z+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(1.0,0.0); glVertex3f(0.5, 0.5, z);

                P = glm::vec3(0.5 + PosX, 0.5 +PosY, altura+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(1.0,4.0); glVertex3f(0.5, 0.5, altura);

                P = glm::vec3(0.5 + PosX, -0.5 +PosY, altura+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(0.0,4.0); glVertex3f(0.5, -0.5, altura);

                P = glm::vec3(-0.5 + PosX, 0.5 +PosY, z+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(0.0,0.0); glVertex3f(-0.5, 0.5, z);

                P = glm::vec3(0.5 + PosX, 0.5 +PosY, z+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(1.0,0.0); glVertex3f(0.5, 0.5, z);

                P = glm::vec3(0.5 + PosX, 0.5 +PosY, altura+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(1.0,4.0); glVertex3f(0.5, 0.5, altura);

                P = glm::vec3(-0.5 + PosX, 0.5 +PosY, altura+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,-1.0,0.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glTexCoord2f(0.0,4.0); glVertex3f(-0.5, 0.5, altura);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        //Desenha a parte da lâmpada do poste
        void drawLampada(GLfloat x, GLfloat y, GLfloat z, GLfloat altura, GLfloat R, GLfloat G, GLfloat B){
            glColor3f(0.33,0.33,0.33);
            glBegin(GL_QUADS);
                glVertex3f(4.0,0.5,5);
                glVertex3f(-0.5, 0.5, 5);
                glVertex3f(-0.5, -0.5, 5);
                glVertex3f(4.0,-0.5,5);
            glEnd();
            glColor3f(R,G,B);
            glBegin(GL_QUADS);
                glVertex3f(3.0,0.5,4.97);
                glVertex3f(1, 0.5, 4.97);
                glVertex3f(1, -0.5, 4.95);
                glVertex3f(3.0,-0.5,4.95);
            glEnd();
            glBegin(GL_QUADS);
                glVertex3f(1, -0.5, 4.95);
                glVertex3f(3.0,-0.5,4.95);
                glVertex3f(3.0,-0.5,5);
                glVertex3f(1, -0.5, 5);
            glEnd();
            glBegin(GL_QUADS);
                glVertex3f(3.0,-0.5,4.95);
                glVertex3f(3.0, 0.5,4.97);
                glVertex3f(3.0,0.5,5);
                glVertex3f(3.0,-0.5,5);
            glEnd();
        }

        //Chamada da função para desenhar o poste completo
        void Poste(GLfloat X, GLfloat Y, GLfloat Z, GLfloat lado, GLuint text_ID_Poste ,Luz &luz){
            drawPoste(X,Y,Z,lado,5,1,1,1,text_ID_Poste,luz);
            drawLampada(X,Y,Z,5,1,1,0);
        }

        //Desenha o poste no 0,0,0 e translada para a posição desejada
        void DesenharPoste(GLuint text_ID_Poste,Luz &luz){
            glPushMatrix();
            glTranslatef(this->PosX,this->PosY,this->PosZ-2.5);
            glScalef(0.2,0.05,1);
            Poste(0,0,0,0.1,text_ID_Poste,luz);
            glPopMatrix();
        }
};