#include <iostream>
#include "DrawCars.cpp"
#include "Luz.cpp"

using namespace std;

//Classe para instanciar cada um dos carros inimigos
class EnemyCar{
    private:
        float PosX;
        float PosY;
        float PosZ;
        float velocidadeFixa;//Velocidade que vai ser somada com a aceleração do carro até a velocidade max
        float velocidadeRelativa;//Vai servir para determinar a nova posição, pois na velocidade relativa se sabe se o carro inimigo vai se mover para tras ou para frente do carro principal
        float velocidadeMax;//Velocidade máxima do carro inimigo

    public:
        EnemyCar(float PosX, float PosY, float PosZ){
            this->PosX = PosX;
            this->PosY = PosY;
            this->PosZ = PosZ;
            this->velocidadeFixa = 0.0;
        }

        //Determina a velocidade atual do carro inimigo, que vai variar com a aceleração até a velocidade maxima
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

        //Função para mover os carros inimigos para suas posições corretas,a partir da velocidade relativa
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

        //Comando para desenhar os carros
        void DrawAllCar(float RotacaoPneu, Luz &luz){
            DrawCars Car = DrawCars(RotacaoPneu);
            Car.drawCar(0,1,0,luz,PosX,PosY,PosZ);
            Car.drawPneus(-0.15,0.352,0.115);
            Car.drawPneus(-0.15,0.122,0.115);
        }
};