#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>

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

                    glTexCoord2f(1.0,0.0); 
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
};