#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>

using namespace std;

//Classe para instanciar os multiplos objetos utilizados nos vetores
class Clouds{
    private:
        float PosX;
        float PosY;
        float PosZ;
        float velocidade;
        GLuint textID;

    public:
        Clouds(float PosX, float PosY, float PosZ, GLuint text_ID){
            this->PosX = PosX;
            this->PosY = PosY;
            this->PosZ = PosZ;
            this->textID = text_ID;
        }

        float getPosX(){
            return this->PosX;
        }

        //Movimenta a nuvem no eixo X, do lado direito para o esquerdo, até sair da extrema esquerda para a extrema direita, e fica no ciclo
        void setPosX(float X){
            if (X <= -150.0){
                X += 350.0;
            }
            this->PosX = X;
        }

        void BoxForTexure(GLfloat R, GLfloat G, GLfloat B){
            glColor3f(R,G,B);
            glBindTexture(GL_TEXTURE_2D, this->textID);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0,0.0); glVertex3f(-1.5,0,-0.5);
                glTexCoord2f(1.0,0.0); glVertex3f(1.5,0,-0.5);
                glTexCoord2f(1.0,1.0); glVertex3f(1.5,0,0.5);
                glTexCoord2f(0.0,1.0); glVertex3f(-1.5,0,0.5);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        //Desenha as nuvens no 0,0,0 e translada
        void DesenharNuvem(){
            glPushMatrix();
            glTranslatef(this->PosX,this->PosY,this->PosZ);
            glScalef(10,10,6);
            BoxForTexure(1,1,1);
            glPopMatrix();
        }
};