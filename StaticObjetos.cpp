#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Luz.cpp"
#include "LuzSpot.cpp"

using namespace std;

//Classe para instanciar os objetos que serão estáticos na cena
class StaticObjetos{
    public:
        //Desenhando os elementos estáticos, que no caso é o piso, chão, linhas laterais
        void EstaticObjects(float X, float Y, GLuint text_ID_Chao, Luz &luz, LuzSpot &luzSpot){
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
            glm::vec3 P,C;
            //Chao        
            glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, text_ID_Chao);
            glm::vec3 cor = glm::vec3(1,1,1);
            glColor3f(cor.r, cor.g, cor.b);
            luz.setEspecularMaterial(glm::vec3(0.2));//Tem novamente uma especular baixa para ter pouca reflexão
            for(float x = -60; x<120; x+=20){//Quadrado grande divido em muitos para ter uma maior qtd de vertices
                for (float y = -40; y < 60; y+=20){
                    glBegin(GL_QUADS);
                        P = glm::vec3((x),(y),(10));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b); 
                        glTexCoord2f(0.0,0.0);  
                        glVertex3f(x,y,10);

                        P = glm::vec3((x+20),(y),(10));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b);
                        glTexCoord2f(int(X),0.0);
                        glVertex3f(x+20,y,10);

                        P = glm::vec3((x+20),(y+20),(10));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b);
                        glTexCoord2f(int(X),int(Y));
                        glVertex3f(x+20,y+20,10);

                        P = glm::vec3((x),(y+20),(10));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b);
                        glTexCoord2f(0.0,int(Y));
                        glVertex3f(x,y+20,10);
                    glEnd();                    
                }
            }
            glBindTexture(GL_TEXTURE_2D, 0);
            glPopMatrix();

            //Pista
            glPushMatrix();
            cor = glm::vec3(0,0,0);
            glColor3f(cor.r, cor.g, cor.b);
            luz.setEspecularMaterial(glm::vec3(0.05));//Queremos aqui uma reflexão quase nula, pois é a pista
            luzSpot.setDifusaMaterial(glm::vec3(0.8,0.8,0.0));
            for(float x = 25; x<35; x+=1){//Quadrado grande divido em muitos para ter uma maior qtd de vertices
                for (float y = -40; y < 60; y+=0.5){
                    glBegin(GL_QUADS);
                        P = glm::vec3((x),(y),(10.03));
                        int resultado = luzSpot.calculoDefPointSpot(P);
                        if(resultado == 0){
                            C = luzSpot.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }else{
                            C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }
                        glColor3f(C.r, C.g, C.b); 
                        glVertex3f(x,y,10.03);

                        P = glm::vec3((x+1),(y),(10.03));
                        resultado = luzSpot.calculoDefPointSpot(P);
                        if(resultado == 0){
                            C = luzSpot.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }else{
                            C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }
                        glColor3f(C.r, C.g, C.b);
                        glVertex3f(x+1,y,10.03);

                        P = glm::vec3((x+1),(y+0.5),(10.03));
                        resultado = luzSpot.calculoDefPointSpot(P);
                        if(resultado == 0){
                            C = luzSpot.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }else{
                            C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }
                        glColor3f(C.r, C.g, C.b);
                        glVertex3f(x+1,y+0.5,10.03);

                        P = glm::vec3((x),(y+0.5),(10.03));
                        resultado = luzSpot.calculoDefPointSpot(P);
                        if(resultado == 0){
                            C = luzSpot.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }else{
                            C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }
                        glColor3f(C.r, C.g, C.b);
                        glVertex3f(x,y+0.5,10.03);
                    glEnd();                    
                }
            }
            glPopMatrix();

            //Faixa Esq 
            glPushMatrix();
            cor = glm::vec3(1,1,1);
            glColor3f(cor.r, cor.g, cor.b);
            luz.setEspecularMaterial(glm::vec3(0.5));
            luzSpot.setDifusaMaterial(glm::vec3(1.0));
            for(float x = 25; x<25.5; x+=0.25){//Retangulo grande divido em muitos para ter uma maior qtd de vertices
                for (float y = -40; y < 60; y+=1){
                    glBegin(GL_QUADS);
                        P = glm::vec3((x),(y),(10.05));
                        int resultado = luzSpot.calculoDefPointSpot(P);
                        if(resultado == 0){
                            C = luzSpot.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }else{
                            C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }
                        glColor3f(C.r, C.g, C.b); 
                        glVertex3f(x,y,10.05);

                        P = glm::vec3((x+0.25),(y),(10.05));
                        resultado = luzSpot.calculoDefPointSpot(P);
                        if(resultado == 0){
                            C = luzSpot.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }else{
                            C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }
                        glColor3f(C.r, C.g, C.b);
                        glVertex3f(x+0.25,y,10.05);

                        P = glm::vec3((x+0.25),(y+1),(10.05));
                        resultado = luzSpot.calculoDefPointSpot(P);
                        if(resultado == 0){
                            C = luzSpot.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }else{
                            C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }
                        glColor3f(C.r, C.g, C.b);
                        glVertex3f(x+0.25,y+1,10.05);

                        P = glm::vec3((x),(y+1),(10.05));
                        resultado = luzSpot.calculoDefPointSpot(P);
                        if(resultado == 0){
                            C = luzSpot.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }else{
                            C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        }
                        glColor3f(C.r, C.g, C.b);
                        glVertex3f(x,y+1,10.05);
                    glEnd();                    
                }
            }
            glPopMatrix();
            //Faixa Dir
            glPushMatrix();
            cor = glm::vec3(1,1,1);
            glColor3f(cor.r, cor.g, cor.b);
            luz.setEspecularMaterial(glm::vec3(0.05));
            for(float x = 34.5; x<35; x+=0.25){//Retangulo grande divido em muitos para ter uma maior qtd de vertices
                for (float y = -40; y < 60; y+=1){
                    glBegin(GL_QUADS);
                        P = glm::vec3((x),(y),(10.05));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b); 
                        glVertex3f(x,y,10.05);

                        P = glm::vec3((x+0.25),(y),(10.05));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b);
                        glVertex3f(x+0.25,y,10.05);

                        P = glm::vec3((x+0.25),(y+1),(10.05));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b);
                        glVertex3f(x+0.25,y+1,10.05);

                        P = glm::vec3((x),(y+1),(10.05));
                        C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                        glColor3f(C.r, C.g, C.b);
                        glVertex3f(x,y+1,10.05);
                    glEnd();                    
                }
            }
            glPopMatrix();
        }
};