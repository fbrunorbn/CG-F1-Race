#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <math.h>
#pragma once

class Luz{//Classe para determinar a iluminação dos objetos determinados
    private:
        glm::vec3 posicao;                     //Fonte de luz pontual
        glm::vec3 ambiente, difusa, especular; //Propriedades da fonte de luz
        glm::vec3 posicaoCamera;               //Posição da Camera para a especular
        glm::vec3 especularMaterial;           //Propiedade especular do objeto
    public:
        Luz(glm::vec3 posicao){
            this->posicao = posicao;
            ambiente  = glm::vec3(0.3);
            difusa    = glm::vec3(1.0); 
            especular = glm::vec3(1.0);
            especularMaterial = glm::vec3(0.0);
        }

        glm::vec3 getPosicao(){
            return posicao;
        }

        void setPosicao(glm::vec3 posicao){
            this->posicao = posicao;
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
            glm::vec3 Rd = glm::max(glm::dot(L,N),0.2f) * color; //Color é aqui a propriedade difusa do material
            float Rs;
            if (K < 0){
                Rs = pow(0,16);
            }else{
                Rs = pow(K,16);
            }

            //Determinando os componentes finais da luz
            glm::vec3 ambiente = Ia * Ra;
            glm::vec3 difusa   = Id * Rd;
            glm::vec3 especularR   = Is * Rs * especularMaterial;

            //Cor final com a soma dos componentes da luz
            glm::vec3 corFinal = ambiente + difusa + especularR;

            return corFinal;
        }

        //Determinando a normal entre 3 pontos e seus 2 vetores, que geram os vetores V1=(P2-P1) e V2=(P3-P1)
        glm::vec3 calculoNormal(glm::vec3 point1,glm::vec3 point2, glm::vec3 point3){
            glm::vec3 V1 = point2 - point1;
            glm::vec3 V2 = point3 - point1;
            glm::vec3 Normal = glm::cross(V2,V1);
            return Normal;
        }

};