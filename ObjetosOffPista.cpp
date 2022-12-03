#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>

using namespace std;

//Classe para instanciar os multiplos objetos utilizados nos vetores
class ObjetosOffPista{
    private:
        float PosX;
        float PosY;
        float PosZ;
        float velocidade;
        GLuint textID;

    public:
        ObjetosOffPista(float PosX, float PosY, float PosZ){
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

        //Desenhando a copa das arvores
        void drawFilledCopa(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat faces, GLfloat altura,GLfloat R, GLfloat G, GLfloat B,GLuint text_ID_copa){
            int i;
            int triangleAmount = faces;
            
            GLfloat twicePi = 2.0f * M_PI;
            glBindTexture(GL_TEXTURE_2D, text_ID_copa);
            glBegin(GL_POLYGON);
                glColor3f(R,G,B);
                glTexCoord2f(0.5,2); glVertex3f(x, y, z);
                for(i = 0; i <= triangleAmount;i++) { 
                    glTexCoord2f(0.0,0.0); 
                    glVertex3f(
                        x + (radius * cos(i *  twicePi / triangleAmount)), 
                        y + (radius * sin(i * twicePi / triangleAmount)),
                        z
                    );

                    glTexCoord2f(1.0,1.0); 
                    glVertex3f(
                        x + (radius * cos((i+1) *  twicePi / triangleAmount)), 
                        y + (radius * sin((i+1) * twicePi / triangleAmount)),
                        z
                    );
                }
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
            
            glBindTexture(GL_TEXTURE_2D, text_ID_copa);
            glBegin(GL_TRIANGLE_FAN);
                glColor3f(R,G,B);
                glTexCoord2f(0.5,1); glVertex3f(x, y, altura);
                for(i = 0; i <= triangleAmount;i++) {
                    glTexCoord2f(0.0,0.0);
                    glVertex3f(
                        x + (radius * cos(i *  twicePi / triangleAmount)), 
                        y + (radius * sin(i * twicePi / triangleAmount)),
                        z
                    );
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
        
        //Base do poste
        void drawFilledTrunk(GLfloat x, GLfloat y, GLfloat z, GLfloat lado, GLfloat altura,GLfloat R, GLfloat G, GLfloat B){
            glColor3f(R,G,B);
            glBegin(GL_QUADS);
                glVertex3f(0.5, 0.5, z);
                glVertex3f(-0.5, 0.5, z);
                glVertex3f(-0.5, -0.5, z);
                glVertex3f(0.5, -0.5, z);
            glEnd();
            glBegin(GL_QUAD_STRIP);
                glVertex3f(0.5, 0.5, z);
                glVertex3f(0.5, 0.5, altura);
                glVertex3f(-0.5, 0.5, z);
                glVertex3f(-0.5, 0.5, altura);
                glVertex3f(-0.5, -0.5, z);
                glVertex3f(-0.5, -0.5, altura);
                glVertex3f(0.5, -0.5, z);
                glVertex3f(0.5, -0.5, altura);
                glVertex3f(0.5, 0.5, z);
                glVertex3f(0.5, 0.5, altura);
            glEnd();
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
                glTexCoord2f(0.0,30.0); glVertex3f(0.5, 0.5, altura);
                glVertex3f(-0.5, 0.5, z);
                glVertex3f(-0.5, 0.5, altura);
                glVertex3f(-0.5, -0.5, z);
                glVertex3f(-0.5, -0.5, altura);
                glTexCoord2f(30.0,0.0); glVertex3f(0.5, -0.5, z);
                glTexCoord2f(30.0,30.0); glVertex3f(0.5, -0.5, altura);
                glVertex3f(0.5, 0.5, z);
                glVertex3f(0.5, 0.5, altura);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        //Desenhando o tronco das árvores
        void drawFilledCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat faces, GLfloat altura, GLint R, GLint G, GLint B, GLuint text_ID_caule){
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

            glBindTexture(GL_TEXTURE_2D, text_ID_caule);
            glBegin(GL_QUAD_STRIP);
                for(i = 0; i <= QtdTriangulos;i++) {
                    glTexCoord2f(0.0,0.0);
                    glVertex3f(
                    x + (radius * cos(i *  twicePi / QtdTriangulos)), 
                    y + (radius * sin(i * twicePi / QtdTriangulos)),
                    z);

                    glTexCoord2f(0.0,1.0); glVertex3f(
                    x + (radius * cos(i *  twicePi / QtdTriangulos)), 
                    y + (radius * sin(i * twicePi / QtdTriangulos)),
                    z + altura
                    );

                    glTexCoord2f(1.0,1.0); glVertex3f(
                    x + (radius * cos((i+1) *  twicePi / QtdTriangulos)), 
                    y + (radius * sin((i+1) * twicePi / QtdTriangulos)),
                    z
                    );

                    glTexCoord2f(1.0,0.0); glVertex3f(
                    x + (radius * cos((i+1) *  twicePi / QtdTriangulos)), 
                    y + (radius * sin((i+1) * twicePi / QtdTriangulos)),
                    z + altura
                    );
                }
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        //Chamanda da função para a copa
        void copa(GLfloat X, GLfloat Y, GLfloat Z, GLfloat radius,GLuint text_ID_copa){
            drawFilledCopa(X,Y,Z,radius,8,10,0,1,0,text_ID_copa);
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            drawFilledCopa(X,Y,Z,radius,8,10,0,0,0,text_ID_copa);
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

        //Chamada da função para o tronco
        void caule(GLfloat X, GLfloat Y, GLfloat Z, GLfloat radius, GLuint text_ID_caule){
            drawFilledCircle(0,0,0,0.5,4,-3,1,1,0, text_ID_caule);
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            drawFilledCircle(0,0,0,0.5,4,-3,0,0,0, text_ID_caule);
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

        //Desenha as arvores no 0,0,0 e as translada para suas posições corretas
        void DesenharArvore(GLuint text_ID_copa, GLuint text_ID_caule){
            glPushMatrix();
            glTranslatef(this->PosX,this->PosY,this->PosZ);
            glScalef(0.8,0.2,0.25);
            copa(0,0,0,3,text_ID_copa);
            copa(0,0,4,3,text_ID_copa);
            caule(0,0,0,1, text_ID_caule);
            glPopMatrix();
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
            drawPoste(X,Y,Z,lado,5,0.33,0.33,0.33,text_ID_Poste);
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

        //Desenha as nuvens
        void Circulo(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat R, GLfloat G, GLfloat B){
            int i;
            int triangleAmount = 360;
            GLfloat twicePi = 2.0f * M_PI;
            glColor3f(R,G,B);
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(x, y ,z);
                for(i = 0; i <= triangleAmount;i++) { 
                    glVertex3f(
                        x + (radius * sin(i *  twicePi / triangleAmount)),
                        y ,
                        z + (radius * cos(i * twicePi / triangleAmount))
                    );
                }
            glEnd();
        }

        //Chamada da função para desenhar as nuvens
        void Nuvem(GLfloat X, GLfloat Y, GLfloat Z){
            Circulo(X,Y,Z,1,1,1,1);
        }

        void Retangulo(GLfloat Z, GLfloat R, GLfloat G, GLfloat B){
            glColor3f(R,G,B);
            glBegin(GL_QUADS);
                glVertex3f(0,0,Z);
                glVertex3f(0,0,Z-1);
                glVertex3f(50,0,Z-1);
                glVertex3f(50,0,Z);
            glEnd();
        }

        //Desenha as nuvens no 0,0,0 e translada
        void DesenharNuvem(){
            glPushMatrix();
            glTranslatef(this->PosX,this->PosY,this->PosZ);
            glRotatef(90,1,0,0);
            glScalef(4,4,4);
            Nuvem(0,0,0);
            glPopMatrix();
        }

        void Faixa(GLfloat X, GLfloat Y, GLfloat Z, GLfloat lado){
            drawFilledTrunk(X,Y,Z,lado,5,0.5,0.5,0.5);
            Retangulo(5,1,1,1);
            glPushMatrix();
            glTranslatef(50,0,0);
            drawFilledTrunk(X,Y,Z,lado,5,0.5,0.5,0.5);
            glPopMatrix();
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

            drawFilledTrunk(X,Y,Z,lado,5,0,0,0);
            Retangulo(5,0,0,0);
            glPushMatrix();
            glTranslatef(50,0,0);
            drawFilledTrunk(X,Y,Z,lado,5,0,0,0);
            glPopMatrix();
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

        //Desenha o poste no 0,0,0 e translada para a posição desejada
        void DesenharFaixa(){
            glPushMatrix();
            glTranslatef(this->PosX,this->PosY,this->PosZ-2.5);
            glScalef(0.2,0.1,1);
            Faixa(0,0,0,0.1);
            glPopMatrix();
        }
};