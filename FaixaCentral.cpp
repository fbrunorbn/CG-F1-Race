#include <iostream>

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
};