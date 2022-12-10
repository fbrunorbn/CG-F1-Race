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
        void EstaticObjects(float X, float Y, GLuint text_ID_Chao, Luz &luz){
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
            glm::vec3 P,C;
            //Chao        
            glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, text_ID_Chao);
            glm::vec3 cor = glm::vec3(1,1,1);
            glColor3f(cor.r, cor.g, cor.b);
            luz.setEspecularMaterial(glm::vec3(0.2));
            for(float i = -60; i<120; i+=20){
                for (float j = -40; j < 60; j+=20){
                    glBegin(GL_QUADS);
                        P = glm::vec3((i),(j),(10));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b); 
                        glTexCoord2f(0.0,0.0);  
                        glVertex3f(i,j,10);

                        P = glm::vec3((i+20),(j),(10));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b);
                        glTexCoord2f(int(X),0.0);
                        glVertex3f(i+20,j,10);

                        P = glm::vec3((i+20),(j+20),(10));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b);
                        glTexCoord2f(int(X),int(Y));
                        glVertex3f(i+20,j+20,10);

                        P = glm::vec3((i),(j+20),(10));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b);
                        glTexCoord2f(0.0,int(Y));
                        glVertex3f(i,j+20,10);
                    glEnd();                    
                }
            }
            glBindTexture(GL_TEXTURE_2D, 0);
            glPopMatrix();

            //Pista
            glPushMatrix();
            cor = glm::vec3(0,0,0);
            glColor3f(cor.r, cor.g, cor.b);
            luz.setEspecularMaterial(glm::vec3(0.05));
            for(float i = 25; i<35; i+=1){
                for (float j = -40; j < 60; j+=1){
                    glBegin(GL_QUADS);
                        P = glm::vec3((i),(j),(10.03));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b); 
                        glVertex3f(i,j,10.03);

                        P = glm::vec3((i+1),(j),(10.03));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b);
                        glVertex3f(i+1,j,10.03);

                        P = glm::vec3((i+1),(j+1),(10.03));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b);
                        glVertex3f(i+1,j+1,10.03);

                        P = glm::vec3((i),(j+1),(10.03));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b);
                        glVertex3f(i,j+1,10.03);
                    glEnd();                    
                }
            }
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