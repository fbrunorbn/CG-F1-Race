#include <iostream>
#include "DrawCars.cpp"

using namespace std;

//Classe para instanciar cada um dos carros inimigos
class EnemyCar{
    private:
        float PosX;
        float PosY;
        float PosZ;
        float velocidadeFixa;
        float velocidadeRelativa;
        float velocidadeMax;

    public:
        EnemyCar(float PosX, float PosY, float PosZ){
            this->PosX = PosX;
            this->PosY = PosY;
            this->PosZ = PosZ;
            this->velocidadeFixa = 0.0;
        }

        void DefineVeloFixa(float velocidade){
            if (velocidade > this->velocidadeMax){
                this->velocidadeFixa = velocidadeMax;
            }else{
                this->velocidadeFixa = velocidade;
            }
        }

        //Funções para definir a velocidade realtiva do carro inimigo
        void DefineVeloRelativa(float velocidade, float MaxVelo){
            this->velocidadeRelativa = (this->velocidadeFixa-velocidade)/MaxVelo;
        }

        //Função para mover os carros inimigos para suas posições corretas
        void MoveCar(){
            this->PosY += this->velocidadeRelativa;
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

        float getVeloFixa(){
            return velocidadeFixa;
        }
        
        void setVeloMax(float velocidade){
            this->velocidadeMax = velocidade;
        }

        void setPosX(float PosX){
            this->PosX = PosX;
        }

        void DrawAllCar(float RotacaoPneu){
            DrawCars Car = DrawCars(RotacaoPneu);
            Car.drawCar(0,1,0);
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            Car.drawCar(0,0,0);
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            Car.drawPneus(-0.15,0.352,0.115);
            Car.drawPneus(-0.15,0.122,0.115);
        }
};