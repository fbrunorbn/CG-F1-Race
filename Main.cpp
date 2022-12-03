//Aluno: Francisco Bruno Rebouças do Nascimento
//Matrícula: 479278
//Curso: Ciência da Computação
//Cadeira: Computação Gráfica

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <iostream>
#include "FaixaCentral.cpp"
#include "ObjetosOffPista.cpp"
#include "EnemyCar.cpp"
#include "PrimaryCar.cpp"
#include "StaticObjetos.cpp"
#include <vector>
#include <cstdlib>
#include <math.h>
#include "stb_image.cpp"

#define FPS 30 //Limite de FPS no jogo
#define MaxFaixaCentral 20 //Quantidade máxima de faixas centrais
#define MaxVelocidade 240 //Velocidade máxima atingida pelo carro
#define MovimentacaoNuvem  0.5; //Serve para a translação das nuvens

using namespace std;

int Pressed_Key[5] = {0,0,0,0,0};//Controle de movimentacao do teclado
float VetorDir[5];//Vetor de direcao dos carros inimigos
float PosXGlobalCamera = 30.0;//Posição inicial da camera
float PosYGlobalCamera = 9.0;
float PosZGlobalCamera = 12.0;
float PosYApontaCamera = 40.0;//Posição inicial da camera
float PosZApontaCamera = 10.0;//Posição inicial da camera
int QtdCarrosInimigos = 5; //Controla a qtd de carros inimigos
int RotacaoPneu = 0; //Serve para girar o pneu quando o carro anda
int Temporizador = 0; //Serve para os estados de 1-2-3-GO
float Chegada = 0;
float Colisao = 0;
float RotacaoColisao = 0;
float TempoColisao = 0;
float TamTextX = 50;
float TamTextY = 50;
float frustrum_min = 0.9;
GLuint textID[13];
GLuint textID_vel[241];

PrimaryCar MyCar = PrimaryCar(32,11.5,10.11,0.0);
StaticObjetos ObjetosEstaticos = StaticObjetos();
vector <FaixaCentral> FaixasCentrais;
vector <EnemyCar> EnemyCars;
vector <ObjetosOffPista> Arvores;
vector <ObjetosOffPista> Postes;
vector <ObjetosOffPista> Nuvens;
vector <ObjetosOffPista> FaixasLargada;

//Carregar as Texturas
void carregaTextura(GLuint tex_id, string filePath){
    unsigned char* imgData;
    int largura, altura, canais;

    stbi_set_flip_vertically_on_load(true);
    imgData = stbi_load(filePath.c_str(), &largura, &altura, &canais, 4);
    if (imgData){
        glBindTexture(GL_TEXTURE_2D,tex_id);//Juntando a região com o ID
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,largura,altura,0,GL_RGBA,GL_UNSIGNED_BYTE,imgData);

        //Casos de maior ou menor
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPLACE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPLACE);

        stbi_image_free(imgData);
    }else{
        cout << "Erro: Nao foi possivel carregar a textura! " << filePath.c_str() << endl;
    }

    
}

//Instanciando as faixas centrais em quantidade fixa
void criarFaixasCentrais(){
    float distY = 40.0;
    for (int i = -40; i<MaxFaixaCentral; i++){
        FaixaCentral Faixa = FaixaCentral(30.0,distY,10.05);
        FaixasCentrais.push_back(Faixa);
        distY -= 4.0;
    }  
}

//Instanciando as arvores em quantidade fixa
void criarArvores(){
    for (int i = -40; i < 40 ; i+= 3){        
        for (int ladoEsq = -30; ladoEsq <= 22; ladoEsq += 4){
            int tree_aleat_pos = rand() % 5;
            ObjetosOffPista Arvore = ObjetosOffPista(ladoEsq,tree_aleat_pos + i,11);
            Arvores.push_back(Arvore);
        }
        for (int ladoDir = 38; ladoDir <= 80; ladoDir += 4){
            int tree_aleat_pos = rand() % 5;
            ObjetosOffPista Arvore = ObjetosOffPista(ladoDir,tree_aleat_pos + i,11);
            Arvores.push_back(Arvore);
        }
    }
    
}

//Instanciando as postes em quantidade fixa
void criarPoste(){
    for (int i = -30; i <= 40 ; i+= 11){
        ObjetosOffPista Poste = ObjetosOffPista(24,i,11);
        Postes.push_back(Poste);
    }
}

//Instanciando os inimigos em faixa aleatoria da pista, e de forma dinâmica
void criarEnemyCar(){
    float distY = 12.5;
    for (int i = 1; i <= QtdCarrosInimigos; i++){
        distY += i/2;
        int x;
        if ((i%2)==0){
            x = 32.0;
        }else{
            x = 27.0;
        }
        EnemyCar Car = EnemyCar(x,distY,10.1);
        float rand_aux = rand() % 55;
        rand_aux += 180;
        Car.setVeloMax(rand_aux);
        EnemyCars.push_back(Car);

        //Velocidade com que os carros inimigos se movem continuamente para esquerda ou direita
        rand_aux = rand() % 2;//Gerar 1 ou 2
        if (int(rand_aux) % 2 == 0){
            VetorDir[i-1] = -0.1;
        }else{
            VetorDir[i-1] = 0.1;
        }
    }    
}

//Instanciando as nuvens em quantidade fixa
void criarNuvem(){
    for (int i = 0; i <= 40; i++){
        int x = (rand() % 300) - 20;
        int z = (rand() % 20) + 11;
        ObjetosOffPista Nuvem = ObjetosOffPista(x,45,z);
        Nuvens.push_back(Nuvem);
    }
}

void criarLargada(){
    for (int i = 0; i < 1; i++){
        ObjetosOffPista FaixaLargada = ObjetosOffPista(25,15,10.3);
        FaixasLargada.push_back(FaixaLargada);
    }
}

//Inicializando todos os objetos de quantidade fixa pre-determinada, como meu carro, faixas centrais, nuvens, postes e árvores
void initializeGL(){
    glClearColor(0.53, 0.84, 0.90, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glGenTextures(10, textID);//Gerando na memoria a textura com seu id
    carregaTextura(textID[0],"sprites/PressStart.jpeg");
    carregaTextura(textID[1],"sprites/1.png");
    carregaTextura(textID[2],"sprites/2.png");
    carregaTextura(textID[3],"sprites/3.png");
    carregaTextura(textID[4],"sprites/Go.png");
    carregaTextura(textID[5],"sprites/YouLose.jpg");
    carregaTextura(textID[6],"sprites/YouWin.png");
    carregaTextura(textID[7],"sprites/Grama.jpg");
    carregaTextura(textID[8],"sprites/Copa.jpg");
    carregaTextura(textID[9],"sprites/Tronco.jpg");

    glGenTextures(241, textID_vel);//Gerando na memoria a textura da velocidade com seu id
    for (int i = 0; i <= 240; i++){
        string path = "sprites/sprites_velocimetro/";
        string convertion = to_string(i);
        string format = ".png";
        path = path + convertion + format;
        carregaTextura(textID_vel[i],path);
    }

    criarFaixasCentrais();
    criarArvores();
    criarPoste();
    criarNuvem();
    criarEnemyCar();
    criarLargada();
}

//Funcao para desenhar as faixas centrais na posicao 0,0,0
void drawFaixaCentral(){
    for (int i = 0; i < MaxFaixaCentral; i++){
        glPushMatrix();
        float x = FaixasCentrais[i].getPosX();
        float y = FaixasCentrais[i].getPosY();
        float z = FaixasCentrais[i].getPosZ();
        glTranslatef(x,y,z);
        glBegin(GL_QUADS);
            glColor3f(1,1,1);
            glVertex3f(-0.25,-1,0);
            glVertex3f(0.25,-1,0);
            glVertex3f(0.25,1,0);
            glVertex3f(-0.25,1,0);
        glEnd();
        glPopMatrix();
    }
}

//Funcao para desenhar os carros inimigos, e já os transladar para suas posições corretas
void drawEnemyCars(){
    for (int i = 0; i < QtdCarrosInimigos; i++){
        glPushMatrix();
        float x = EnemyCars[i].getPosX();
        float y = EnemyCars[i].getPosY();
        float z = EnemyCars[i].getPosZ();
        glTranslatef(x,y,z);
        EnemyCars[i].DrawAllCar(RotacaoPneu);
        glPopMatrix();
    }
}

//Funcao de desenho do mundo completo, para chamar o desenho de todos os objetos, estaticos e dinâmicos
void drawWorld(){
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-4,4,-1,1,frustrum_min,40);
    glFrustum(-4,4,-1,1,0.9,40);

    glMatrixMode(GL_MODELVIEW);
    //glViewport(0,0,1800,900);
    glLoadIdentity();
    //gluLookAt(PosXGlobalCamera, PosYGlobalCamera,PosZGlobalCamera, //posição da câmera
    //          PosXGlobalCamera, PosYApontaCamera,PosZApontaCamera,//Posição inicial da camera, //para onde a câmera olha
    //          0, 0, 1); //para onde o topo da câmera aponta
    glLoadIdentity();
    gluLookAt(PosXGlobalCamera, 9,12, //posição da câmera
              PosXGlobalCamera, PosYApontaCamera,10, //para onde a câmera olha
              0, 0, 1); //para onde o topo da câmera aponta

    ObjetosEstaticos.EstaticObjects(TamTextX,TamTextY,textID[7]);
    
    
    glPushMatrix();
    glTranslatef(MyCar.getPosX()+0.5,MyCar.getPosY()+0.25,MyCar.getPosZ());
    if (Colisao == 1){
        glRotatef(RotacaoColisao,0,0,1);
    }
    glTranslatef(-0.5,-0.25,0);
    MyCar.DrawAllCar(RotacaoPneu);
    glPopMatrix();

    drawFaixaCentral();

    drawEnemyCars();

    for (int i = 0; i < Arvores.size(); i++){
        Arvores[i].DesenharArvore(textID[8],textID[9]);
    }

    for (int i = 0; i < Postes.size(); i++){
        Postes[i].DesenharPoste(textID[9]);
    }

    for (int i = 0; i < Nuvens.size(); i++){
        Nuvens[i].DesenharNuvem();
    }

    for (int i = 0; i < FaixasLargada.size(); i++){
        FaixasLargada[i].DesenharFaixa();
    }

    if (Temporizador == 0){//Tela de Press "Start"
        glColor3f(0.8,0.1,0.5);
        glBegin(GL_QUADS);
            glVertex3f(27,10,11);
            glVertex3f(33,10,11);
            glVertex3f(32.75,10.05,12.92);
            glVertex3f(27.25,10.05,12.92);
        glEnd();
        glColor3f(0,0,0);
        glBindTexture(GL_TEXTURE_2D, textID[0]);
        glBegin(GL_QUADS);//(0,0)
            glTexCoord2f(0.0,0.0); glVertex3f(28,9.9,11.3);
            glTexCoord2f(1.0,0.0); glVertex3f(32,9.9,11.3);
            glTexCoord2f(1.0,1.0); glVertex3f(32,10,12.7);
            glTexCoord2f(0.0,1.0); glVertex3f(28,10,12.7);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }else if (Temporizador > 1 && Temporizador <= 30){//Contador "1"
        glColor3f(1,0,0);
        glBindTexture(GL_TEXTURE_2D, textID[1]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(29.7,10,12.2);
            glTexCoord2f(1.0,0.0); glVertex3f(30.3,10,12.2);
            glTexCoord2f(1.0,1.0); glVertex3f(30.3,10,12.4);
            glTexCoord2f(0.0,1.0); glVertex3f(29.7,10,12.4);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }else if (Temporizador > 30 && Temporizador <= 60){//Contador "2"
        glColor3f(1,0,0);
        glBindTexture(GL_TEXTURE_2D, textID[2]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(29.7,10,12.2);
            glTexCoord2f(1.0,0.0); glVertex3f(30.3,10,12.2);
            glTexCoord2f(1.0,1.0); glVertex3f(30.3,10,12.4);
            glTexCoord2f(0.0,1.0); glVertex3f(29.7,10,12.4);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }else if (Temporizador > 60 && Temporizador <= 90){//Contador "3"
        glColor3f(1,0,0);
        glBindTexture(GL_TEXTURE_2D, textID[3]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(29.7,10,12.2);
            glTexCoord2f(1.0,0.0); glVertex3f(30.3,10,12.2);
            glTexCoord2f(1.0,1.0); glVertex3f(30.3,10,12.4);
            glTexCoord2f(0.0,1.0); glVertex3f(29.7,10,12.4);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }else if (Temporizador > 90 && Temporizador <= 110){//"GO!"
        glColor3f(1,0,0);
        glBindTexture(GL_TEXTURE_2D, textID[4]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(29.7,10,12.2);
            glTexCoord2f(1.0,0.0); glVertex3f(30.3,10,12.2);
            glTexCoord2f(1.0,1.0); glVertex3f(30.3,10,12.4);
            glTexCoord2f(0.0,1.0); glVertex3f(29.7,10,12.4);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }else if (Temporizador == -1){//"Perdeu"
        glColor3f(1,0,0);
        glBindTexture(GL_TEXTURE_2D, textID[5]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(27,10,11);
            glTexCoord2f(1.0,0.0); glVertex3f(33,10,11);
            glTexCoord2f(1.0,1.0); glVertex3f(33,10.11,12.92);
            glTexCoord2f(0.0,1.0); glVertex3f(27,10.11,12.92);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }else if (Temporizador == -2){//"Ganhou"
        glColor3f(1,0,0);
        glBindTexture(GL_TEXTURE_2D, textID[6]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(27,10,11);
            glTexCoord2f(1.0,0.0); glVertex3f(33,10,11);
            glTexCoord2f(1.0,1.0); glVertex3f(33,10.11,12.92);
            glTexCoord2f(0.0,1.0); glVertex3f(27,10.11,12.92);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }else if (Temporizador > 150){
        glColor3f(1,0,0);
        glBindTexture(GL_TEXTURE_2D, textID_vel[int(MyCar.getVelocidade())]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(PosXGlobalCamera-0.8,10,12.45);
            glTexCoord2f(1.0,0.0); glVertex3f(PosXGlobalCamera+0.8,10,12.45);
            glTexCoord2f(1.0,1.0); glVertex3f(PosXGlobalCamera+0.8,10.03,13.01);
            glTexCoord2f(0.0,1.0); glVertex3f(PosXGlobalCamera-0.8,10.03,13.01);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
   
    glutSwapBuffers();
}

//Função que vai realmente fazer a movimentação das coordenadas de cada objeto para a posição desejada
void ocioso(int v){
    if (Temporizador != 0 && Temporizador > 0){
        Temporizador ++;
    }
    //Movendo as nuvens de forma constante
    for (int i = 0; i < Nuvens.size(); i ++){
        float PosX = Nuvens[i].getPosX();
        PosX -= MovimentacaoNuvem;
        Nuvens[i].setPosX(PosX);
    }
    if (Chegada > 300 && Temporizador != -1 && Temporizador != -2){
        for (int i = 0; i < EnemyCars.size(); i++){
            if (FaixasLargada[0].getPosY() <= EnemyCars[i].getPosY()){
                Temporizador = -1;
            }
        }
        if (FaixasLargada[0].getPosY() <= MyCar.getPosY()){
                Temporizador = -2;
        }
        if (Temporizador == -1){
            PosXGlobalCamera = 30.0;
            glutTimerFunc(1000.0/FPS, ocioso, 0);
            glutPostRedisplay();
        }
        if (Temporizador == -2){
            PosXGlobalCamera = 30.0;
            glutTimerFunc(1000.0/FPS, ocioso, 0);
            glutPostRedisplay();
        }
    }
    if (Temporizador == 0){
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }else if (Temporizador > 1 && Temporizador <= 110){
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }else if (Temporizador > 110 && Temporizador <=150){
        PosXGlobalCamera += (MyCar.getPosX() - PosXGlobalCamera)/10;
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }else if (Temporizador != -1 && Temporizador != -2){
        if(MyCar.getVelocidade() > 0){
            TamTextX += 0.5;
            TamTextY += 0.5;
            if (TamTextX >= 70){
                TamTextX = 50;
            }
            if (TamTextY >= 70){
                TamTextY = 50;
            }
        }
        
        //Aqui o carro perde velocidade constantemente sempre que não está sendo apertado a tecla de ir para frente
        MyCar.DefineVelo(MyCar.getVelocidade() - 0.3);

        //Mantendo a posição da camera de acordo com a posição do meu carro
        PosXGlobalCamera = MyCar.getPosX() + 0.5;

        if (Pressed_Key[0] == 1){
            //W
            MyCar.DefineVelo(MyCar.getVelocidade() + 1.3);
        }
        if (Pressed_Key[1] == 1){
            //A
            MyCar.setPosX(MyCar.getPosX() - 0.07);
            MyCar.DefineVelo(MyCar.getVelocidade() - 0.3);
        }
        if (Pressed_Key[2] == 1){
            //S
            MyCar.DefineVelo(MyCar.getVelocidade() - 4.0);
        }
        if (Pressed_Key[3] == 1){
            //D
            MyCar.setPosX(MyCar.getPosX() + 0.07);
            MyCar.DefineVelo(MyCar.getVelocidade() - 0.3);
        }
        if (Pressed_Key[4] == 1){
            //D
            if (PosYApontaCamera > 0){
                PosYApontaCamera = PosYApontaCamera*(-1.0);
                frustrum_min = 0.2;
                PosXGlobalCamera = PosXGlobalCamera-0.5;
                PosYGlobalCamera = 12.30;
                PosZGlobalCamera = 11;
                PosZApontaCamera = 8;
            }
        }
        if (Pressed_Key[4] == 0){
            //D
            if (PosYApontaCamera < 0){
                PosYApontaCamera = PosYApontaCamera*(-1.0);
                frustrum_min = 0.9;
                PosYGlobalCamera = 9.0;
                PosZGlobalCamera = 12.0;
                PosXGlobalCamera = MyCar.getPosX() + 0.5;
            }
        }
        
        //Determinar as posições finais das Faixas Centrais
        for (int i = 0; i < MaxFaixaCentral; i ++){
            FaixasCentrais[i].DefineVelo(MyCar.getVelocidade(),MaxVelocidade);
            FaixasCentrais[i].MoverFaixaCentral();
        }

        //Movendo os carros inimigos com suas velocidades fixas em relação a velocidade relativa ao meu carro
        for (int i = 0; i < QtdCarrosInimigos; i ++){
            int veloFixa = EnemyCars[i].getVeloFixa();
            float PosX = EnemyCars[i].getPosX();
            float rand_aux = rand() % 1000;            
            if (rand_aux >= 60 && rand_aux <= 65){
                VetorDir[i] = VetorDir[i]*(-1);
            }

            veloFixa += 1.5;            
            PosX += VetorDir[i];
            if (PosX >= 33.8){
                VetorDir[i] = VetorDir[i]*(-1);
                PosX = 33.75;
            }else if(PosX <= 25.2){
                VetorDir[i] = VetorDir[i]*(-1);
                PosX = 25.25;
            }
            EnemyCars[i].setPosX(PosX);
            EnemyCars[i].DefineVeloFixa(veloFixa);
            EnemyCars[i].DefineVeloRelativa(MyCar.getVelocidade(),MaxVelocidade);
            EnemyCars[i].MoveCar();
            
        }

        //Defini os limites do Meu Carro, para n sair da pista, ter velocidade negativa ou acima da velocidade máxima
        if (MyCar.getPosX() > 33.6){
            MyCar.setPosX(33.6);
        }else if(MyCar.getPosX() < 25.4){
            MyCar.setPosX(25.4);
        }else if(MyCar.getVelocidade() <= 0){
            MyCar.DefineVelo(0.0);
        }else if(MyCar.getVelocidade() >= MaxVelocidade){
            MyCar.DefineVelo(MaxVelocidade);
        }

        //Fazendo a rotação dos pneus caso o carro esteja em movimento
        if (RotacaoPneu < 360 && MyCar.getVelocidade() > 0.0){
            RotacaoPneu += 2;
        }
        if (RotacaoPneu >= 360){
            RotacaoPneu = 0;
        }

        //Movendo as árvores com velocidade relativa ao meu carro
        for (int i = 0; i < Arvores.size(); i ++){
            Arvores[i].DefineVelo(MyCar.getVelocidade(),MaxVelocidade);
            Arvores[i].MoverObjetoOffPista();
        }

        //Movendo os postes com velocidade relativa ao meu carro
        for (int i = 0; i < Postes.size(); i ++){
            Postes[i].DefineVelo(MyCar.getVelocidade(),MaxVelocidade);
            Postes[i].MoverObjetoOffPista();
        }

        for (int i = 0; i < FaixasLargada.size(); i ++){
            FaixasLargada[i].DefineVelo(MyCar.getVelocidade(),MaxVelocidade);
            FaixasLargada[i].MoverFaixaChegada();
            Chegada += FaixasLargada[i].getPosY()/FPS;
        }
        
        if (Colisao == 0){
            float PosX, PosY;
            PosX = MyCar.getPosX();
            PosY = MyCar.getPosY();
            for (int i = 0; i < QtdCarrosInimigos; i ++){
                float XsupDir,YsupDir,XinfEsq,YinfEsq;
                XsupDir = EnemyCars[i].getPosX() + 1;//Canto superior direito
                YsupDir = EnemyCars[i].getPosY() + 0.5;

                XinfEsq = EnemyCars[i].getPosX();//Canto inferior esquerdo
                YinfEsq = EnemyCars[i].getPosY();

                if ((PosX + 1) >= XinfEsq && (PosX + 1) <= XsupDir && (PosY + 0.5) >= YinfEsq && (PosY + 0.5) <= YsupDir){
                    Colisao = 1;
                }else if ((PosX + 1) >= XinfEsq && (PosX + 1) <= XsupDir && (PosY) >= YinfEsq && (PosY) <= YsupDir){
                    Colisao = 1;
                }else if ((PosX) >= XinfEsq && (PosX) <= XsupDir && (PosY + 0.5) >= YinfEsq && (PosY + 0.5) <= YsupDir){
                    Colisao = 1;
                }else if ((PosX) >= XinfEsq && (PosX) <= XsupDir && (PosY) >= YinfEsq && (PosY) <= YsupDir){
                    Colisao = 1;
                }
            }
        }
        if (Colisao == 1){
            if (RotacaoColisao == 0){
                MyCar.DefineVelo(MyCar.getVelocidade() - 50);
                RotacaoColisao = 5;
                if (MyCar.getVelocidade() <= 0){
                    MyCar.DefineVelo(0);
                }
            }else{
                MyCar.DefineVelo(MyCar.getVelocidade() - 0.5);
                if (MyCar.getVelocidade() <= 0){
                    MyCar.DefineVelo(0);
                }
            }
            if (RotacaoColisao > 0){
                RotacaoColisao = RotacaoColisao*(-1);
            }else{
                RotacaoColisao = RotacaoColisao*(-1);
            }
            TempoColisao += 3;
            if (TempoColisao >= 90){
                RotacaoColisao = 0;
                Colisao = 0;
                TempoColisao = 0;
            }
        }
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }
}

//Função para tratamento de teclado
void Key(unsigned char key, int x, int y){
    if (key == 'w' || key == 'W'){
        Pressed_Key[0] = 1;
    }
    if (key == 'a' || key == 'A'){
        Pressed_Key[1] = 1;
    }
    if (key == 's' || key == 'S'){
        Pressed_Key[2] = 1;
    }
    if (key == 'd' || key == 'D'){
        Pressed_Key[3] = 1;
    }
    if (key == 'r' || key == 'R'){
        Pressed_Key[4] = 1;
    }
    if (key == 'm' || key == 'M'){
        Temporizador = 1;
    }
}

void KeyUp(unsigned char key, int x, int y){
    if (key == 'w' || key == 'W'){
        Pressed_Key[0] = 0;
    }
    if (key == 'a' || key == 'A'){
        Pressed_Key[1] = 0;
    }
    if (key == 's' || key == 'S'){
        Pressed_Key[2] = 0;
    }
    if (key == 'd' || key == 'D'){
        Pressed_Key[3] = 0;
    }
    if (key == 'r' || key == 'R'){
        Pressed_Key[4] = 0;
    }
}

//Main
int main(int argc, char **argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(1200,600);
    glutCreateWindow("F1 Race");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initializeGL();

    glutDisplayFunc(drawWorld);
    glutKeyboardFunc(Key);
    glutKeyboardUpFunc(KeyUp);
    glutTimerFunc(1000.0/FPS, ocioso, 0);

    glutMainLoop();
    return 0;
}