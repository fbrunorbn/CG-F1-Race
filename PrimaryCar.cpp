#include <iostream>
#include "DrawCars.cpp"
#include "Luz.cpp"

using namespace std;

//Classe para instanciar o carro principal
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
            this->Velocidade = velocidade;//Determina a velocidade do meu carro
        }

        //Função para ajudar a limitar a velocidade total
        void DefineVelo(float velocidade){
            this->Velocidade = velocidade;
            if (this->Velocidade > 270){
                this->Velocidade = 270.0;
            }
            if (this->Velocidade <= 0.0){
                this->Velocidade = 0.0;
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

        //Chamada da função para desenhar o carro completo
        void DrawAllCar(float RotacaoPneu, Luz &luz){
            DrawCars Car = DrawCars(RotacaoPneu);
            Car.drawCar(1,0,0,luz,PosX,PosY,PosZ);
            Car.drawPneus(-0.15,0.352,0.115);
            Car.drawPneus(-0.15,0.122,0.115);
        }
};