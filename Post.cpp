#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>

using namespace std;

//Classe para instanciar os multiplos objetos utilizados nos vetores
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

        void MoverFaixaChegada(){
            this->PosY -= this->velocidade;
            if (PosY <= -40.0){
                PosY += 640.0;
            }
        }

        float getPosX(){
            return this->PosX;
        }

        void setPosX(float X){
            if (X <= -100.0){
                X += 250.0;
            }
            this->PosX = X;
        }

        float getPosY(){
            return PosY;
        }

        float getPosZ(){
            return PosZ;
        }

        //Base do poste
        void drawPoste(GLfloat x, GLfloat y, GLfloat z, GLfloat lado, GLfloat altura,GLfloat R, GLfloat G, GLfloat B,GLuint text_ID_Poste){
            glColor3f(R,G,B);
            glBegin(GL_QUADS);
                glVertex3f(0.5, 0.5, z);
                glVertex3f(-0.5, 0.5, z);
                glVertex3f(-0.5, -0.5, z);
                glVertex3f(0.5, -0.5, z);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, text_ID_Poste);

            glBegin(GL_QUAD_STRIP);
                glTexCoord2f(0.0,0.0); glVertex3f(0.5, 0.5, z);
                glTexCoord2f(0.0,3.0); glVertex3f(0.5, 0.5, altura);
                glVertex3f(-0.5, 0.5, z);
                glVertex3f(-0.5, 0.5, altura);
                glVertex3f(-0.5, -0.5, z);
                glVertex3f(-0.5, -0.5, altura);
                glTexCoord2f(3.0,0.0); glVertex3f(0.5, -0.5, z);
                glTexCoord2f(3.0,3.0); glVertex3f(0.5, -0.5, altura);
                glVertex3f(0.5, 0.5, z);
                glVertex3f(0.5, 0.5, altura);
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

        //Chamada da função para desenhar o poste
        void Poste(GLfloat X, GLfloat Y, GLfloat Z, GLfloat lado, GLuint text_ID_Poste){
            drawPoste(X,Y,Z,lado,5,1,1,1,text_ID_Poste);
            drawLampada(X,Y,Z,5,1,1,0);
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            drawPoste(X,Y,Z,lado,5,0,0,0,text_ID_Poste);
            drawLampada(X,Y,Z,5,0,0,0);
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

        //Desenha o poste no 0,0,0 e translada para a posição desejada
        void DesenharPoste(GLuint text_ID_Poste){
            glPushMatrix();
            glTranslatef(this->PosX,this->PosY,this->PosZ-2.5);
            glScalef(0.2,0.05,1);
            Poste(0,0,0,0.1,text_ID_Poste);
            glPopMatrix();
        }
};