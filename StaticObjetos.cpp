#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>

using namespace std;

//Classe para instanciar os multiplos objetos utilizados nos vetores
class StaticObjetos{
    public:
        //Desenhando os elementos estáticos, que no caso é o piso, chão, linhas laterais
        void EstaticObjects(float X, float Y, GLuint text_ID_Chao){
            //Chao        
            glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, text_ID_Chao);
            glColor3f(0,0.29,0.20);
            glBegin(GL_QUADS);
                glNormal3f(0.f, 0.f, 1.f);
                glTexCoord2f(0.0,0.0);       glVertex3f(-60,-40,10);
                glTexCoord2f(int(X),0.0);    glVertex3f(120,-40,10);
                glTexCoord2f(int(X),int(Y)); glVertex3f(120,60,10);
                glTexCoord2f(0.0,int(Y));    glVertex3f(-60,60,10);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
            glPopMatrix();
            //Pista
            glPushMatrix();
            glColor3f(0,0,0);
            glBegin(GL_QUADS);
                glNormal3f(0.f, 0.f, 1.f);
                glVertex3f(25,-40,10.03);
                glVertex3f(35,-40,10.03);
                glVertex3f(35,60,10.03);
                glVertex3f(25,60,10.03);
            glEnd(); 
            glPopMatrix();
            //Faixa Esq 
            glPushMatrix();
            glColor3f(1,1,1);
            glBegin(GL_QUADS);
                glNormal3f(0.f, 0.f, 1.f);
                glVertex3f(25,-40,10.05);
                glVertex3f(25.5,-40,10.05);
                glVertex3f(25.5,60,10.05);
                glVertex3f(25,60,10.05);
            glEnd(); 
            glPopMatrix();
            //Faixa Dir 
            glPushMatrix();
            glColor3f(1,1,1);
            glBegin(GL_QUADS);
                glNormal3f(0.f, 0.f, 1.f);
                glVertex3f(34.5,-40,10.05);
                glVertex3f(35,-40,10.05);
                glVertex3f(35,60,10.05);
                glVertex3f(34.5,60,10.05);
            glEnd(); 
            glPopMatrix();
        }
};