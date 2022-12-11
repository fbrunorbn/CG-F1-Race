//Curso: Ciência da computação
//Cadeira: Computação Gráfica - 2022
//Trabalho 2 - F1 Race
//Alunos:
//Anne Caroline da Silva Rocha - 470124
//Anne Mikaelly Barreto de Abreu - 404805
//Francisco Bruno Rebouças do Nascimento - 479278

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "FaixaCentral.cpp"
#include "Tree.cpp"
#include "FaixaLargada.cpp"
#include "Post.cpp"
#include "Clouds.cpp"
#include "SemaforoLargada.cpp"
#include "EnemyCar.cpp"
#include "PrimaryCar.cpp"
#include "StaticObjetos.cpp"
#include <vector>
#include <cstdlib>
#include <math.h>
#include "stb_image.cpp"
#include "Luz.cpp"
#include "LuzSpot.cpp"

#define FPS 30 //Limite de FPS no jogo
#define MaxFaixaCentral 25 //Quantidade máxima de faixas centrais
#define MovimentacaoNuvem  0.5; //Serve para a translação das nuvens

using namespace std;

int MaxVelocidade = 240;
int Pressed_Key[5] = {0,0,0,0,0};//Controle de movimentacao do teclado
float VetorDir[5];//Vetor de direcao dos carros inimigos
float PosXGlobalCamera = 30.0;//Posição inicial da camera (LookAt)
float PosYGlobalCamera = 9.0;//Posição inicial da camera (LookAt)
float PosZGlobalCamera = 12.0;//Posição inicial da camera (LookAt)
float PosXApontaCamera = 30.0;//Posição inicial para onde a camera aponta (LookAt)
float PosYApontaCamera = 40.0;//Posição inicial para onde a camera aponta (LookAt)
float PosZApontaCamera = 10.0;//Posição inicial para onde a camera aponta (LookAt)
int QtdCarrosInimigos = 5; //Controla a qtd de carros inimigos
int RotacaoPneu = 0; //Serve para girar o pneu quando o carro anda
float RotacaoSemaforo = 0.0; //Serve girar o semáforo da contagem de inicio 1-2-3-Go
int Temporizador = 0; //Serve para os estados do jogo
float Chegada = 0; //Verifica se tem um ganhador
float Colisao = 0; //Estado de colisão
float RotacaoColisao = 0; //Rotacao do carro quando colide
float TempoColisao = 0; //Tempo de estado de colisão
float TamTextX = 10; //Coordenadas da textura da grama
float TamTextY = 10; //Coordenadas da textura da grama
int Nitro = 3; //Contador de nitro restante
int TempoNitro = 0; //Tempo de estado de aceleração
GLuint textID[16]; //Vetor dos sprites
GLuint textID_vel[271]; //Vetor dos sprites do velocimetro
Luz luz(glm::vec3(50,40,40)); //Posição da luz
LuzSpot luzSpot(glm::vec3(24,20,11)); //Posição da luz

//Inicializações de tudo que vai ficar pra "sempre" no mapa
PrimaryCar MyCar = PrimaryCar(32,11.5,10.11,0.0); //Criação do carro principal
StaticObjetos ObjetosEstaticos = StaticObjetos(); //Criação dos objetos estáticos
SemaforoLargada Semaphore = SemaforoLargada(30,15,13); //Criando o bloco para usar as texturas do semaforo 1-2-3
vector <FaixaCentral> VecFaixasCentrais; //Vetor das 20 faixas centrais
vector <EnemyCar> VecEnemyCars; //Vetor dos 5 carros inimigos
vector <Tree> VecArvores; //Vetor das arvores
vector <Post> VecPostes; //Vetor dos postes
vector <Clouds> VecNuvens; //Vetor das nuvens
vector <FaixaLargada> VecFaixasLargada; //Vetor da faixa de largada e chegada

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
    float distY = 60.0;
    for (int i = -40; i<MaxFaixaCentral; i++){
        FaixaCentral Faixa = FaixaCentral(30.0,distY,10.05);
        VecFaixasCentrais.push_back(Faixa);
        distY -= 4.0;
    }  
}

//Instanciando as arvores em quantidade fixa
void criarArvores(){
    for (int i = -40; i < 40 ; i+= 3){        
        for (int ladoEsq = -31; ladoEsq <= 21; ladoEsq += 4){
            int tree_aleat_pos = rand() % 5;
            Tree Arvore = Tree(ladoEsq,tree_aleat_pos + i,11);
            VecArvores.push_back(Arvore);
        }
        for (int ladoDir = 38; ladoDir <= 80; ladoDir += 4){
            int tree_aleat_pos = rand() % 5;
            Tree Arvore = Tree(ladoDir,tree_aleat_pos + i,11);
            VecArvores.push_back(Arvore);
        }
    }
}

//Instanciando as postes em quantidade fixa
void criarPoste(){
    Post Poste = Post(24,20,11);
    VecPostes.push_back(Poste);
}

//Instanciando os inimigos em faixa aleatoria da pista, e de forma dinâmica
void criarEnemyCar(){
    float distY = 12.5;
    for (int i = 1; i <= QtdCarrosInimigos; i++){
        distY += i/3; //Mudar a posicao na pista mais para frente do carro
        int x;
        //Nascer no lado direito ou esquerdo da pista
        if ((i%2)==0){
            x = 32.0;
        }else{
            x = 27.0;
        }
        EnemyCar Car = EnemyCar(x,distY,10.1);
        //Determinar a velocidade randomica do carro inimigo
        float rand_aux = rand() % 55;
        rand_aux += 180;
        Car.setVeloMax(rand_aux);
        VecEnemyCars.push_back(Car);

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
    for (int i = 0; i <= 5; i++){
        int x = i *90;
        int z = 0;
        if(i%2==0){
            z = (rand() % 20) + 25;
        }else{
            z = rand() % 35 + 25;
        }
        Clouds Nuvem = Clouds(x,50,z,textID[10]);
        VecNuvens.push_back(Nuvem);
    }
}

//Instanciando a faixa de largada e chagada
void criarLargada(){
    for (int i = 0; i < 1; i++){
        FaixaLargada FaixaLargadas = FaixaLargada(25,16.5,10.3);
        VecFaixasLargada.push_back(FaixaLargadas);
    }
}

//Inicializando todos os objetos de quantidade fixa pre-determinada, como meu carro, faixas centrais, nuvens, postes e árvores
void initializeGL(){
    glClearColor(0.53, 0.84, 0.90, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMPILE);
    glGenTextures(16, textID);//Gerando na memoria a textura com seu id
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
    carregaTextura(textID[10],"sprites/Nuvem.png");
    carregaTextura(textID[11],"sprites/Nitro-3.png");
    carregaTextura(textID[12],"sprites/Nitro-2.png");
    carregaTextura(textID[13],"sprites/Nitro-1.png");
    carregaTextura(textID[14],"sprites/Nitro-0.png");
    carregaTextura(textID[15],"sprites/Largada.png");


    glGenTextures(271, textID_vel);//Gerando na memoria a textura da velocidade com seu id
    for (int i = 0; i <= 270; i++){
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
        VecFaixasCentrais[i].drawFaixaCentral(luz,luzSpot);
    }
}

//Funcao para desenhar os carros inimigos, e já os transladar para suas posições corretas
void drawEnemyCars(){
    for (int i = 0; i < QtdCarrosInimigos; i++){
        glPushMatrix();
        float x = VecEnemyCars[i].getPosX();
        float y = VecEnemyCars[i].getPosY();
        float z = VecEnemyCars[i].getPosZ();
        glTranslatef(x,y,z);
        VecEnemyCars[i].DrawAllCar(RotacaoPneu,luz,luzSpot);
        glPopMatrix();
    }
}

//Funcao de desenho do mundo completo, para chamar o desenho de todos os objetos, estaticos e dinâmicos
void drawWorld(){
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-4,4,-1,1,0.9,50);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,1800,900);
    glLoadIdentity();
    gluLookAt(PosXGlobalCamera, PosYGlobalCamera,PosZGlobalCamera, //posição da câmera
              PosXApontaCamera, PosYApontaCamera,PosZApontaCamera,//Posição inicial da camera, //para onde a câmera olha
              0, 0, 1); //para onde o topo da câmera aponta

    ObjetosEstaticos.EstaticObjects(TamTextX,TamTextY,textID[7],luz,luzSpot);
    
    //Desenhar meu carro
    glPushMatrix();
    glTranslatef(MyCar.getPosX()+0.5,MyCar.getPosY()+0.25,MyCar.getPosZ());
    //Mostrar a colisao
    if (Colisao == 1){
        glRotatef(RotacaoColisao,0,0,1);
    }
    glTranslatef(-0.5,-0.25,0);
    MyCar.DrawAllCar(RotacaoPneu,luz,luzSpot);
    glPopMatrix();

    //Desenhar as 20 faixas centrais
    drawFaixaCentral();

    //Desenhar os inimigos
    drawEnemyCars();

    //Desenhar as arvores
    for (int i = 0; i < VecArvores.size(); i++){
        VecArvores[i].DesenharArvore(textID[8],textID[9],luz);
    }

    //Desenhar os postes
    for (int i = 0; i < VecPostes.size(); i++){
        VecPostes[i].DesenharPoste(textID[9],luz);
    }

    //Desenhar as nuvens
    for (int i = 0; i < VecNuvens.size(); i++){
        VecNuvens[i].DesenharNuvem();
    }

    //Desenhar a faixa de largada e consequente chegada
    for (int i = 0; i < VecFaixasLargada.size(); i++){
        VecFaixasLargada[i].DesenharFaixa(textID[15]);
    }

    //Verificando os "estado do jogo"(tela de menu, contagem antes de iniciar a corrida, inicio da corrida, e chegada)
    if (Temporizador == 0){//Tela de "Press Start"
        glColor3f(0.8,0.1,0.5);
        glBegin(GL_QUADS);
            glVertex3f(27,10,11);
            glVertex3f(33,10,11);
            glVertex3f(32.75,10.05,12.92);
            glVertex3f(27.25,10.05,12.92);
        glEnd();
        glColor3f(1,1,1);
        glBindTexture(GL_TEXTURE_2D, textID[0]);
        glBegin(GL_QUADS);//(0,0)
            glTexCoord2f(0.0,0.0); glVertex3f(28,9.9,11.3);
            glTexCoord2f(1.0,0.0); glVertex3f(32,9.9,11.3);
            glTexCoord2f(1.0,1.0); glVertex3f(32,10,12.7);
            glTexCoord2f(0.0,1.0); glVertex3f(28,10,12.7);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }else if (Temporizador > 1 && Temporizador < 45){//Contador "1"
        Semaphore.drawSemaphore(RotacaoSemaforo, textID[1]);
    }else if (Temporizador >= 45 && Temporizador < 90){//Contador "2"
        Semaphore.drawSemaphore(RotacaoSemaforo, textID[2]);
    }else if (Temporizador >= 90 && Temporizador < 135){//Contador "3"
        Semaphore.drawSemaphore(RotacaoSemaforo, textID[3]);
    }else if (Temporizador >= 135 && Temporizador <= 180){//"GO!"
        Semaphore.drawSemaphore(RotacaoSemaforo, textID[4]);
    }else if (Temporizador == -1){//"Perdeu"
        glColor3f(1,1,1);
        glBindTexture(GL_TEXTURE_2D, textID[5]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(27,10,11);
            glTexCoord2f(1.0,0.0); glVertex3f(33,10,11);
            glTexCoord2f(1.0,1.0); glVertex3f(33,10.11,12.92);
            glTexCoord2f(0.0,1.0); glVertex3f(27,10.11,12.92);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }else if (Temporizador == -2){//"Ganhou"
        glColor3f(1,1,1);
        glBindTexture(GL_TEXTURE_2D, textID[6]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(27,10,11);
            glTexCoord2f(1.0,0.0); glVertex3f(33,10,11);
            glTexCoord2f(1.0,1.0); glVertex3f(33,10.11,12.92);
            glTexCoord2f(0.0,1.0); glVertex3f(27,10.11,12.92);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }else if (Temporizador > 150){
        GLuint aux_text = 0;
        if (Nitro == 3){
            aux_text = textID[11];
        }else if (Nitro == 2){
            aux_text = textID[12];
        }else if (Nitro == 1){
            aux_text = textID[13];
        }else{
            aux_text = textID[14];
        }
        //glViewport(200,100,850,500);
        glColor3f(1,1,1);
        glBindTexture(GL_TEXTURE_2D, textID_vel[int(MyCar.getVelocidade())]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(PosXGlobalCamera-0.5,10,12.45);
            glTexCoord2f(1.0,0.0); glVertex3f(PosXGlobalCamera+0.8,10,12.45);
            glTexCoord2f(1.0,1.0); glVertex3f(PosXGlobalCamera+0.8,10.03,13.01);
            glTexCoord2f(0.0,1.0); glVertex3f(PosXGlobalCamera-0.5,10.03,13.01);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, aux_text);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(PosXGlobalCamera-1.5,10,12.45);
            glTexCoord2f(1.0,0.0); glVertex3f(PosXGlobalCamera-0.5,10,12.45);
            glTexCoord2f(1.0,1.0); glVertex3f(PosXGlobalCamera-0.5,10.03,13.01);
            glTexCoord2f(0.0,1.0); glVertex3f(PosXGlobalCamera-1.5,10.03,13.01);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
 
    glutSwapBuffers();
}

//Função que vai realmente fazer a movimentação das coordenadas de cada objeto para a posição desejada a cada FPS
void ocioso(int v){
    //Tempo do jogo após start
    if (Temporizador != 0 && Temporizador > 0){
        Temporizador ++;
    }
    //Movendo as nuvens de forma constante
    for (int i = 0; i < VecNuvens.size(); i ++){
        float PosX = VecNuvens[i].getPosX();
        PosX -= MovimentacaoNuvem;
        VecNuvens[i].setPosX(PosX);
    }
    //Verificar se faixa de largada passou uma vez e verificar se tem um ganhador
    if (Chegada > 300 && Temporizador != -1 && Temporizador != -2){
        for (int i = 0; i < VecEnemyCars.size(); i++){
            if (VecFaixasLargada[0].getPosY() <= VecEnemyCars[i].getPosY()){
                Temporizador = -1;
            }
        }
        if (VecFaixasLargada[0].getPosY() <= MyCar.getPosY()){
                Temporizador = -2;
        }
        if (Temporizador == -1){
            PosXGlobalCamera = 30.0;
            PosXApontaCamera = 30.0;
            glutTimerFunc(1000.0/FPS, ocioso, 0);
            glutPostRedisplay();
        }
        if (Temporizador == -2){
            PosXGlobalCamera = 30.0;
            PosXApontaCamera = 30.0;
            glutTimerFunc(1000.0/FPS, ocioso, 0);
            glutPostRedisplay();
        }
    }
    if (Temporizador == 0){//Matem a tela de "menu"
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }else if (Temporizador > 1 && Temporizador <= 45){//O semaforo com a textura "3"
        if (Temporizador > 30){
            RotacaoSemaforo -= 6;
        }
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }else if (Temporizador > 45 && Temporizador <= 90){//O semaforo com a textura "2"
        if (Temporizador > 75){
            RotacaoSemaforo -= 6;
        }
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }else if (Temporizador > 90 && Temporizador <= 135){//O semaforo com a textura "1"
        if (Temporizador > 120){
            RotacaoSemaforo -= 6;
        }
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }else if (Temporizador > 135 && Temporizador <= 180){//O semaforo com a textura "GO"
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }else if (Temporizador > 180 && Temporizador <=210){//Transição da camera para iniciar
        PosXGlobalCamera += (MyCar.getPosX() - PosXGlobalCamera)/10;
        PosXApontaCamera += (MyCar.getPosX() - PosXApontaCamera)/10;
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }else if (Temporizador != -1 && Temporizador != -2){

        //ANNE MIKAELLY ********************************************

        //Definindo as coordenadas da textura do chão (indo de X-10 a 725, o mesmo para Y, para mudar a qtd da textura no chao, assim ela é replicada mantendo seu tamanho, e n sendo esticada se as coordenadas fossem 0 e 1 normal, que é o desejado)
        if(MyCar.getVelocidade() > 0){
            TamTextX += 0.2;
            TamTextY += 0.2;
            if (TamTextX >= 15){
                TamTextX = 10;
            }
            if (TamTextY >= 15){
                TamTextY = 10;
            }
        }
        
        //Aqui o carro perde velocidade constantemente sempre que não está sendo apertado a tecla de ir para frente
        MyCar.DefineVelo(MyCar.getVelocidade() - 0.3);

        //Mantendo a posição da camera de acordo com a posição do meu carro
        PosXGlobalCamera = MyCar.getPosX() + 0.5;
        PosXApontaCamera = MyCar.getPosX() + 0.5;

        //Verificação das teclas pressionadas
        if (Pressed_Key[0] == 1){
            //W - Acelera
            MyCar.DefineVelo(MyCar.getVelocidade() + 1.3);
        }
        if (Pressed_Key[1] == 1){
            //A - Vai para esquerda e desacelera
            MyCar.setPosX(MyCar.getPosX() - 0.07);
            MyCar.DefineVelo(MyCar.getVelocidade() - 0.7);
        }
        if (Pressed_Key[2] == 1){
            //S - Desacelera
            MyCar.DefineVelo(MyCar.getVelocidade() - 2.0);
        }
        if (Pressed_Key[3] == 1){
            //D - Vai para a direita e desacelera
            MyCar.setPosX(MyCar.getPosX() + 0.07);
            MyCar.DefineVelo(MyCar.getVelocidade() - 0.7);
        }
        if (Pressed_Key[4] == 1){
            //R - Olha para trás com a camera
            if (PosYApontaCamera > 0){
                PosYApontaCamera = PosYApontaCamera*(-1.0);
            }
        }
        if (Pressed_Key[4] == 0){
            //R - Ao deixar de pressionar volta a olhar para o local original
            if (PosYApontaCamera < 0){
                PosYApontaCamera = PosYApontaCamera*(-1.0);
            }
        }
        
        //Determinar as posições finais das Faixas Centrais a medida que o meu carro se "movimenta"
        for (int i = 0; i < MaxFaixaCentral; i ++){
            VecFaixasCentrais[i].DefineVelo(MyCar.getVelocidade(),MaxVelocidade);
            VecFaixasCentrais[i].MoverFaixaCentral();
        }

        //Movendo as árvores com velocidade relativa ao meu carro
        for (int i = 0; i < VecArvores.size(); i ++){
            VecArvores[i].DefineVelo(MyCar.getVelocidade(),MaxVelocidade);
            VecArvores[i].MoverObjetoOffPista();
        }

        //Movendo os postes com velocidade relativa ao meu carro
        for (int i = 0; i < VecPostes.size(); i ++){
            VecPostes[i].DefineVelo(MyCar.getVelocidade(),MaxVelocidade);
            VecPostes[i].MoverObjetoOffPista();
        }

        //ANNE CAROLINNE ********************************************

        //Move a feixa de largada com velocidade relativa ao meu carro
        for (int i = 0; i < VecFaixasLargada.size(); i ++){
            VecFaixasLargada[i].DefineVelo(MyCar.getVelocidade(),MaxVelocidade);
            VecFaixasLargada[i].MoverFaixaChegada();
            Chegada += VecFaixasLargada[i].getPosY()/FPS;//Verifica a distancia percorrida pela faixa
        }

        //Movendo os carros inimigos com suas velocidades fixas em relação a velocidade relativa ao meu carro
        for (int i = 0; i < QtdCarrosInimigos; i ++){
            int veloFixa = VecEnemyCars[i].getVeloFixa();
            float PosX = VecEnemyCars[i].getPosX();
            //Faz uma verificação que se rand_aux cair entre 60 e 65 o carro inimigo muda de direção no deslocamento lateral
            float rand_aux = rand() % 1000;            
            if (rand_aux >= 60 && rand_aux <= 65){
                VetorDir[i] = VetorDir[i]*(-1);
            }

            veloFixa += 1.5; //Os carros inimigos vão acelerando 1.5 a cada FPS, até seu limite de velocidade          
            PosX += VetorDir[i];//Fazendo o desclocamento lateral do carro inimigo
            //Verificando se o carro inimigo não sai da pista
            if (PosX >= 33.8){
                VetorDir[i] = VetorDir[i]*(-1);
                PosX = 33.75;
            }else if(PosX <= 25.2){
                VetorDir[i] = VetorDir[i]*(-1);
                PosX = 25.25;
            }
            VecEnemyCars[i].setPosX(PosX);
            VecEnemyCars[i].DefineVeloFixa(veloFixa);//Vai ser somada até chegar no limite dela, feito na criação do carro
            VecEnemyCars[i].DefineVeloRelativa(MyCar.getVelocidade(),MaxVelocidade);//Velocidade do inimigo em relação ao meu carro, fazendo ou ele se afastar ou se aproximar
            VecEnemyCars[i].MoveCar();
            
        }

        //Defini os limites do Meu Carro, para n sair da pista, ter velocidade negativa ou acima da velocidade máxima
        if (MyCar.getPosX() > 33.6){
            MyCar.setPosX(33.6);
        }else if(MyCar.getPosX() < 25.4){
            MyCar.setPosX(25.4);
        }else if(MyCar.getVelocidade() <= 0){
            MyCar.DefineVelo(0.0);
        }else if(MyCar.getVelocidade() > MaxVelocidade){
            if(MaxVelocidade == 240){
                MyCar.DefineVelo(MaxVelocidade);
            }            
        }

        //Fazendo a rotação dos pneus caso o carro esteja em movimento
        if (RotacaoPneu < 360 && MyCar.getVelocidade() > 0.0){
            RotacaoPneu += 2;
        }
        if (RotacaoPneu >= 360){
            RotacaoPneu = 0;
        }
        
        //FRANCISCO BRUNO ********************************************

        //Tratamento de colisão, onde verifica se meu carro colidiu com algum dos outros carros inimigos
        if (Colisao == 0){
            float PosX, PosY;
            PosX = MyCar.getPosX();
            PosY = MyCar.getPosY();
            for (int i = 0; i < QtdCarrosInimigos; i ++){
                float XsupDir,YsupDir,XinfEsq,YinfEsq;//Estou pegando as posições do vertice esquerdo inferior e direito superior do meu carro
                XsupDir = VecEnemyCars[i].getPosX() + 1;//Canto superior direito
                YsupDir = VecEnemyCars[i].getPosY() + 0.5;

                XinfEsq = VecEnemyCars[i].getPosX();//Canto inferior esquerdo
                YinfEsq = VecEnemyCars[i].getPosY();

                //Verifico se algum vertice do carro "entrou" dentro do bouding box do carro inimigo
                if ((PosX + 1) >= XinfEsq && (PosX + 1) <= XsupDir && (PosY + 0.5) >= YinfEsq && (PosY + 0.5) <= YsupDir){
                    Colisao = 1;//Se sim, entra em estado de colisão
                }else if ((PosX + 1) >= XinfEsq && (PosX + 1) <= XsupDir && (PosY) >= YinfEsq && (PosY) <= YsupDir){
                    Colisao = 1;
                }else if ((PosX) >= XinfEsq && (PosX) <= XsupDir && (PosY + 0.5) >= YinfEsq && (PosY + 0.5) <= YsupDir){
                    Colisao = 1;
                }else if ((PosX) >= XinfEsq && (PosX) <= XsupDir && (PosY) >= YinfEsq && (PosY) <= YsupDir){
                    Colisao = 1;
                }
            }
        }
        //Se está em estado de colisão, ele vai perder uma vez 100 de velocidade, 1 de velocidade a cada 1 FPS, durante 1 segundo(30FPS)
        if (Colisao == 1){
            if (RotacaoColisao == 0){
                MyCar.DefineVelo(MyCar.getVelocidade() - 100);
                RotacaoColisao = 5;
                if (MyCar.getVelocidade() <= 0){
                    MyCar.DefineVelo(0);
                }
            }else{
                MyCar.DefineVelo(MyCar.getVelocidade() - 1);
                if (MyCar.getVelocidade() <= 0){
                    MyCar.DefineVelo(0);
                }
                if(int(TempoColisao)%2==0){
                    PosXGlobalCamera += 0.05;
                }else{
                    PosXGlobalCamera -= 0.05;
                }
            }
            if (RotacaoColisao > 0){//Define a rotação do carro, para fixar "chaqualando"
                RotacaoColisao = RotacaoColisao*(-1);
            }else{
                RotacaoColisao = RotacaoColisao*(-1);
            }
            TempoColisao += 3;
            if (TempoColisao >= 90){//Vai mante no tempo de colisão por 1segundo(30FPS)
                RotacaoColisao = 0;
                Colisao = 0;
                TempoColisao = 0;
            }
        }
        //Faz durante o estado de nitro, receber 1.5 a mais de velocidade a cada FPS por 1segundo, e perder 1.5 de velocidade no segundo final
        if (TempoNitro != 0 and Nitro>=0){
            TempoNitro+=3;
            if (TempoNitro <= 180){
                MaxVelocidade = 270.0;
                if (TempoNitro <= 90){
                    MyCar.DefineVelo(MyCar.getVelocidade() + 1.5);
                }else if(TempoNitro > 90 and TempoNitro <= 180){
                    MyCar.DefineVelo(MyCar.getVelocidade() - 1.5);
                }
            }else{
                TempoNitro = 0;
                MaxVelocidade = 240;
            }
        }
        //Serve para mandar a posição da camera para o especular da luz
        glm::vec3 PosCamera(PosXGlobalCamera, PosYGlobalCamera,PosZGlobalCamera);
        luz.setPosicaoCamera(PosCamera);

        //LuzSpot
        luzSpot.setPosicaoCamera(PosCamera);
        glm::vec3 PosLuzSpot(VecPostes[0].getPosX(),VecPostes[0].getPosY(),(VecPostes[0].getPosZ()+5));
        luzSpot.setPosicao(PosLuzSpot);
        glm::vec3 PosLuzAponta(VecPostes[0].getPosX()+5,VecPostes[0].getPosY(),(VecPostes[0].getPosZ()-5));
        luzSpot.setPosicaoAponta(PosLuzAponta);

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
    if (key == 'f' || key == 'F'){
        Nitro -= 1;
        TempoNitro = 1;
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
    glutInitWindowSize(1800,900);
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