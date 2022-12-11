#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Luz.cpp"

using namespace std;

//Classe para instanciar cada uma das faixas centrais
class FaixaCentral{
    private:
        float PosX;
        float PosY;
        float PosZ;
        float velocidade;

    public:
        FaixaCentral(float PosX, float PosY, float PosZ){
            this->PosX = PosX;
            this->PosY = PosY;
            this->PosZ = PosZ;
            this->velocidade = 0.0;//Tem velocidade 0, seu movimento se dá somente em relação a velocidade relativa ao meu carro
        }

        //Funções para definir a velocidade relativa das faixas centrais
        void DefineVelo(float velocidade, float MaxVelo){
            this->velocidade = velocidade/MaxVelo;
        }

        //Função para mover as faixas para suas posições corretas
        void MoverFaixaCentral(){
            this->PosY -= this->velocidade;
            if (PosY <= -40.0){
                PosY += 80.0;
            }
        }

        float getPosX(){
            return PosX;
        }

        float getPosY(){
            return PosY;
        }

        float getPosZ(){
            return PosZ;
        }

        float drawFaixaCentral(Luz &luz){
            
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
            glm::vec3 P,C;
            glm::vec3 cor = glm::vec3(1,1,1);
            glColor3f(cor.r, cor.g, cor.b);
            luz.setEspecularMaterial(glm::vec3(0.2));
            glPushMatrix();
            glTranslatef(this->PosX,this->PosY,this->PosZ);
            glColor3f(1,1,1);
            glBegin(GL_QUADS);
                P = glm::vec3(-0.25+PosX,-1+PosY,0+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                glColor3f(C.r, C.g, C.b); 
                glVertex3f(-0.25,-1,0);

                P = glm::vec3(0.25+PosX,-1+PosY,0+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(0.25,-1,0);

                P = glm::vec3(0.25+PosX,1+PosY,0+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(0.25,1,0);

                P = glm::vec3(-0.25+PosX,1+PosY,0+PosZ);
                C = luz.ilumina(P,glm::vec3(0.0,0.0,1.0),cor);
                glColor3f(C.r, C.g, C.b);
                glVertex3f(-0.25,1,0);
            glEnd();
            glPopMatrix();
        }
};