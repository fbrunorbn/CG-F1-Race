#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <math.h>
#pragma once

class LuzSpot{//Classe para determinar a iluminação dos objetos determinados
    private:
        glm::vec3 posicao;                     //Fonte de luz pontual
        glm::vec3 ambiente, difusa, especular; //Propriedades da fonte de luz
        glm::vec3 posicaoApontaLuz;            //Posição para onde a luz spot aponta
        glm::vec3 posicaoCamera;               //Posição da Camera para a especular
        glm::vec3 especularMaterial;           //Propiedade especular do objeto
        glm::vec3 difusaMaterial;           //Propiedade especular do objeto
        int anguloAbertura;
    public:
        LuzSpot(glm::vec3 posicao){
            this->posicao = posicao;
            ambiente  = glm::vec3(0.1);
            difusa    = glm::vec3(0.8,0.8,0.0); 
            especular = glm::vec3(0.8,0.8,0.0);
            especularMaterial = glm::vec3(0.1);
            anguloAbertura = 20;
        }

        glm::vec3 getPosicao(){
            return posicao;
        }

        void setPosicao(glm::vec3 posicao){
            this->posicao = posicao;
        }

        void setPosicaoAponta(glm::vec3 posicao){
            this->posicaoApontaLuz = posicao;
        }

        void setPosicaoCamera(glm::vec3 posicao){
            this->posicaoCamera = posicao;
        }

        void setAmbiente(glm::vec3 ambiente){
            this->ambiente = glm::clamp(ambiente, glm::vec3(0), glm::vec3(1));
        }

        void setDifusa(glm::vec3 difusa){
            this->difusa = glm::clamp(difusa, glm::vec3(0), glm::vec3(1));
        }

        void setEspecular(glm::vec3 especular){
            this->especular = glm::clamp(especular, glm::vec3(0), glm::vec3(1));
        }

        void setEspecularMaterial(glm::vec3 especularMaterial){
            this->especularMaterial = especularMaterial;
        }

        void setDifusaMaterial(glm::vec3 difusaMaterial){
            this->difusaMaterial = difusaMaterial;
        }

        glm::vec3 ilumina(glm::vec3 pos, glm::vec3 normal, glm::vec3 color){
            glm::vec3 Ia = this->ambiente;
            glm::vec3 Id = this->difusa;
            glm::vec3 Is = this->especular;

            //calculando os vetores normal e vetor que aponta na direção da fonte de luz
            glm::vec3 N = glm::normalize(normal);
            glm::vec3 L = glm::normalize(posicao - pos);
            //calculando os vetores que aponta de P para a camera e vetor da reflexão perfeita de N e L
            glm::vec3 V = glm::normalize(posicaoCamera - pos);
            glm::vec3 R = 2*(glm::dot(N,L))*(N-L);
            float K = glm::dot(V,R);

            //calculando os componentes ambiente, difuso e especular
            glm::vec3 Ra = glm::vec3(0.2);
            glm::vec3 Rd = glm::max(glm::dot(L,N),0.0f) * (difusaMaterial); //Color é aqui a propriedade difusa do material
            float Rs;
            if (K < 0){
                Rs = pow(0,2);
            }else{
                Rs = pow(K,2);
            }

            //Determinando os componentes finais da luz
            glm::vec3 ambiente = Ia * Ra;
            glm::vec3 difusa   = Id * Rd;
            glm::vec3 especularR   = Is * Rs * especularMaterial;

            //Cor final com a soma dos componentes da luz
            glm::vec3 corFinal = ambiente + difusa+ especularR;

            return corFinal;
        }

        //Determinando se o ponto vai ser iluminado ou não pela fonte de luz spot
        int calculoDefPointSpot(glm::vec3 point1){
            glm::vec3 l = posicao - point1;
            glm::vec3 d = posicaoApontaLuz - posicao;
            l = glm::normalize(l);
            d = glm::normalize(d);
            float resultado = (glm::dot(l,d))*(-1);
            if (resultado>= cos(anguloAbertura*(M_PI/180.0))){
                return 0;//Ponto iluminado
            }else{
                return -1;//Ponto não iluminado
            }
        }

};