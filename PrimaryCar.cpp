#include <iostream>
#include "DrawCars.cpp"
#include "Luz.cpp"

using namespace std;

//Classe para instanciar cada um dos carros inimigos
class PrimaryCar{
    private:
        float PosX;
        float PosY;
        float PosZ;
        float Velocidade;

    public:
        PrimaryCar(float PosX, float PosY, float PosZ, float velocidade){
            this->PosX = PosX;
            this->PosY = PosY;
            this->PosZ = PosZ;
            this->Velocidade = velocidade;//Tem velocidade 100, seu movimento se dá relação a velocidade relativa ao meu carro, onde caso a velocidade do carro inimigo seja maior vai se afastar, e caso seja menor, vai se aproximar
        }

        //Funções para definir a velocidade realtiva do carro inimigo
        void DefineVelo(float velocidade){
            this->Velocidade = velocidade;
            if (this->Velocidade > 270){
                this->Velocidade = 270.0;
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

        float getVelocidade(){
            return Velocidade;
        }

        void setPosX(float PosX){
            this->PosX = PosX;
        }

        void setPosY(float PosY){
            this->PosY = PosY;
        }

        void setPosZ(float PosZ){
            this->PosZ = PosZ;
        }

        void DrawAllCar(float RotacaoPneu, Luz &luz){
            DrawCars Car = DrawCars(RotacaoPneu);
            
            Car.drawCar(1,0,0,luz,PosX,PosY,PosZ);
            //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            //Car.drawCar(0,0,0,luz,PosX,PosY,PosZ);
            //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            Car.drawPneus(-0.15,0.352,0.115);
            Car.drawPneus(-0.15,0.122,0.115);
        }
};