#include <iostream>
#include "DrawCars.cpp"

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

        void DrawAllCar(float RotacaoPneu){
            DrawCars Car = DrawCars(RotacaoPneu);
            Car.drawCar(1,0,0);
            Car.drawPneus(-0.15,0.352,0.115);
            Car.drawPneus(-0.15,0.122,0.115);
        }
};