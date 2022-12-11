#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Luz.cpp"

using namespace std;

//Classe para instanciar os multiplos objetos utilizados nos vetores
class Tree{
    private:
        float PosX;
        float PosY;
        float PosZ;
        float velocidade;
        GLuint textID;

    public:
        Tree(float PosX, float PosY, float PosZ){
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

        //Desenhando a copa das arvores
        void drawFilledCopa(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat faces, GLfloat altura,GLfloat R, GLfloat G, GLfloat B,GLuint text_ID_copa, Luz &luz){
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
            glm::vec3 P,C;
            glm::vec3 cor = glm::vec3(R,G,B);
            glColor3f(cor.r, cor.g, cor.b);
            luz.setEspecularMaterial(glm::vec3(0.2));
            
            int i;
            int triangleAmount = faces;
            
            GLfloat twicePi = 2.0f * M_PI;
            glBindTexture(GL_TEXTURE_2D, text_ID_copa);
            glBegin(GL_POLYGON);
                glColor3f(R,G,B);
                glTexCoord2f(0.5,2); glVertex3f(x, y, z);
                P = glm::vec3(x,y,z);
                C = luz.ilumina(P,glm::vec3(0.0,0.0,-1.0),cor);
                glColor3f(C.r, C.g, C.b);
                for(i = 0; i <= triangleAmount;i++) { 
                    P = glm::vec3(
                        (x + (radius * cos(i *  twicePi / triangleAmount))), 
                        (y + (radius * sin(i * twicePi / triangleAmount))),
                        z);
                    C = luz.ilumina(P,glm::vec3(0.0,0.0,-1.0),cor);
                    glColor3f(C.r, C.g, C.b);
                    glTexCoord2f(0.0,0.0); 
                    glVertex3f(
                        x + (radius * cos(i *  twicePi / triangleAmount)), 
                        y + (radius * sin(i * twicePi / triangleAmount)),
                        z
                    );

                    P = glm::vec3(
                        (x + (radius * cos((i+1) *  twicePi / triangleAmount))), 
                        (y + (radius * sin((i+1) * twicePi / triangleAmount))),
                        z);
                    C = luz.ilumina(P,glm::vec3(0.0,0.0,-1.0),cor);
                    glColor3f(C.r, C.g, C.b);
                    glTexCoord2f(1.0,0.0); 
                    glVertex3f(
                        x + (radius * cos((i+1) *  twicePi / triangleAmount)), 
                        y + (radius * sin((i+1) * twicePi / triangleAmount)),
                        z
                    );
                }
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
            
            //Lateral da copa
            glBindTexture(GL_TEXTURE_2D, text_ID_copa);
            glBegin(GL_TRIANGLE_FAN);
                glColor3f(R,G,B);
                glTexCoord2f(0.5,1); glVertex3f(x, y, altura);
                glm::vec3 point1 = glm::vec3(x,y,altura);
                for(i = 0; i <= triangleAmount;i++) {
                    glm::vec3 point2 = glm::vec3(
                        (x + (radius * cos(i *  twicePi / triangleAmount))), 
                        (y + (radius * sin(i * twicePi / triangleAmount))),
                        z
                    );
                    glm::vec3 point3 = glm::vec3(
                        (x + (radius * cos((i+1) *  twicePi / triangleAmount))), 
                        (y + (radius * sin((i+1) * twicePi / triangleAmount))),
                        z
                    );
                    glm::vec3 Normal = luz.calculoNormal(point1,point2,point3);

                    P = glm::vec3(
                        x + (radius * cos(i *  twicePi / triangleAmount)), 
                        y + (radius * sin(i * twicePi / triangleAmount)),
                        z
                    );
                    C = luz.ilumina(P,Normal,cor);
                    glColor3f(C.r, C.g, C.b);
                    glTexCoord2f(0.0,0.0);
                    glVertex3f(
                        x + (radius * cos(i *  twicePi / triangleAmount)), 
                        y + (radius * sin(i * twicePi / triangleAmount)),
                        z
                    );
                    P = glm::vec3(
                        x + (radius * cos((i+1) *  twicePi / triangleAmount)), 
                        y + (radius * sin((i+1) * twicePi / triangleAmount)),
                        z
                    );
                    C = luz.ilumina(P,Normal,cor);
                    glColor3f(C.r, C.g, C.b);
                    glTexCoord2f(1.0,1.0);
                    glVertex3f(
                        x + (radius * cos((i+1) *  twicePi / triangleAmount)), 
                        y + (radius * sin((i+1) * twicePi / triangleAmount)),
                        z
                    );
                }
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        //Desenhando o tronco das árvores
        void drawFilledCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat faces, GLfloat altura, GLint R, GLint G, GLint B, GLuint text_ID_caule, Luz &luz){
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
            glm::vec3 P,C;
            glm::vec3 cor = glm::vec3(R,G,B);
            glColor3f(cor.r, cor.g, cor.b);
            luz.setEspecularMaterial(glm::vec3(0.2));
            int QtdFaces = faces;
            GLfloat twicePi = 2.0f * M_PI;

            glBindTexture(GL_TEXTURE_2D, text_ID_caule);
            glBegin(GL_QUAD_STRIP);
                for(int i = 0; i <= QtdFaces;i++) {
                    //Pegando a normal
                    glm::vec3 point1 = glm::vec3(
                    (x + (radius * cos(i *  twicePi / QtdFaces))), 
                    (y + (radius * sin(i * twicePi / QtdFaces))),
                    z);
                    glm::vec3 point2 = glm::vec3(
                    (x + (radius * cos((i+1) *  twicePi / QtdFaces))), 
                    (y + (radius * sin((i+1) * twicePi / QtdFaces))),
                    z);
                    glm::vec3 point3 = glm::vec3(
                    (x + (radius * cos(i *  twicePi / QtdFaces))), 
                    (y + (radius * sin(i * twicePi / QtdFaces))),
                    z + altura);
                    glm::vec3 Normal = luz.calculoNormal(point1,point2,point3);

                    //Aplicando cor e textura
                    P = glm::vec3(
                    (x + (radius * cos(i *  twicePi / QtdFaces)) + PosX), 
                    (y + (radius * sin(i * twicePi / QtdFaces)) + PosY),
                    z + PosZ);
                    C = luz.ilumina(P,Normal,cor);
                    glColor3f(C.r, C.g, C.b);
                    glTexCoord2f(0.0,0.0);
                    glVertex3f(
                    x + (radius * cos(i *  twicePi / QtdFaces)), 
                    y + (radius * sin(i * twicePi / QtdFaces)),
                    z);

                    P = glm::vec3(
                    (x + (radius * cos(i *  twicePi / QtdFaces)) + PosX), 
                    (y + (radius * sin(i * twicePi / QtdFaces)) + PosY),
                    z + altura + PosZ);
                    C = luz.ilumina(P,Normal,cor);
                    glColor3f(C.r, C.g, C.b);
                    glTexCoord2f(0.0,1.0); glVertex3f(
                    x + (radius * cos(i *  twicePi / QtdFaces)), 
                    y + (radius * sin(i * twicePi / QtdFaces)),
                    z + altura);

                    P = glm::vec3(
                    (x + (radius * cos((i+1) * twicePi / QtdFaces)) + PosX), 
                    (y + (radius * sin((i+1) * twicePi / QtdFaces)) + PosY),
                    z + PosZ);
                    C = luz.ilumina(P,Normal,cor);
                    glColor3f(C.r, C.g, C.b);
                    glTexCoord2f(1.0,0.0); glVertex3f(
                    x + (radius * cos((i+1) *  twicePi / QtdFaces)), 
                    y + (radius * sin((i+1) * twicePi / QtdFaces)),
                    z);

                    P = glm::vec3(
                    (x + (radius * cos((i+1) * twicePi / QtdFaces)) + PosX), 
                    (y + (radius * sin((i+1) * twicePi / QtdFaces)) + PosY),
                    z + altura + PosZ);
                    C = luz.ilumina(P,Normal,cor);
                    glColor3f(C.r, C.g, C.b);
                    glTexCoord2f(1.0,1.0); glVertex3f(
                    x + (radius * cos((i+1) *  twicePi / QtdFaces)), 
                    y + (radius * sin((i+1) * twicePi / QtdFaces)),
                    z + altura
                    );
                }
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        //Chamanda da função para a copa
        void copa(GLfloat X, GLfloat Y, GLfloat Z, GLfloat radius,GLuint text_ID_copa, Luz &luz){
            drawFilledCopa(X,Y,Z,radius,8,10,1,1,1,text_ID_copa, luz);
        }

        //Chamada da função para o tronco
        void caule(GLfloat X, GLfloat Y, GLfloat Z, GLfloat radius, GLuint text_ID_caule,Luz &luz ){
            drawFilledCircle(0,0,0,0.5,4,-3,1,1,1, text_ID_caule, luz);
        }

        //Desenha as arvores no 0,0,0 e as translada para suas posições corretas
        void DesenharArvore(GLuint text_ID_copa, GLuint text_ID_caule, Luz &luz){
            glPushMatrix();
            glTranslatef(this->PosX,this->PosY,this->PosZ);
            glScalef(0.8,0.2,0.25);
            copa(0,0,0,3,text_ID_copa,luz);
            copa(0,0,4,3,text_ID_copa,luz);
            caule(0,0,0,1, text_ID_caule, luz);
            glPopMatrix();
        }
};