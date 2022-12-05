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

#define FPS 30 //Limite de FPS no jogo
#define MaxFaixaCentral 20 //Quantidade máxima de faixas centrais
#define MovimentacaoNuvem  0.5; //Serve para a translação das nuvens

using namespace std;

int MaxVelocidade = 240;
int Pressed_Key[5] = {0,0,0,0,0};//Controle de movimentacao do teclado
float VetorDir[5];//Vetor de direcao dos carros inimigos
float PosXGlobalCamera = 30.0;//Posição inicial da camera
float PosYGlobalCamera = 9.0;
float PosZGlobalCamera = 12.0;
float PosXApontaCamera = 30.0;
float PosYApontaCamera = 40.0;//Posição inicial da camera
float PosZApontaCamera = 10.0;//Posição inicial da camera
int QtdCarrosInimigos = 5; //Controla a qtd de carros inimigos
int RotacaoPneu = 0; //Serve para girar o pneu quando o carro anda
float RotacaoSemaforo = 0.0;
int Temporizador = 0; //Serve para os estados de 1-2-3-GO
float Chegada = 0;
float Colisao = 0;
float RotacaoColisao = 0;
float TempoColisao = 0;
float TamTextX = 50;
float TamTextY = 50;
float frustrum_min = 0.9;
int Nitro = 3;
int TempoNitro = 0;
GLuint textID[15];
GLuint textID_vel[271];

PrimaryCar MyCar = PrimaryCar(32,11.5,10.11,0.0);
StaticObjetos ObjetosEstaticos = StaticObjetos();
SemaforoLargada Semaphore = SemaforoLargada(30,15,13);
vector <FaixaCentral> VecFaixasCentrais;
vector <EnemyCar> VecEnemyCars;
vector <Tree> VecArvores;
vector <Post> VecPostes;
vector <Clouds> VecNuvens;
vector <FaixaLargada> VecFaixasLargada;

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
        VecFaixasCentrais.push_back(Faixa);
        distY -= 4.0;
    }  
}

//Instanciando as arvores em quantidade fixa
void criarArvores(){
    for (int i = -40; i < 40 ; i+= 3){        
        for (int ladoEsq = -30; ladoEsq <= 22; ladoEsq += 4){
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
    for (int i = -30; i <= 40 ; i+= 11){
        Post Poste = Post(24,i,11);
        VecPostes.push_back(Poste);
    }
}

//Instanciando os inimigos em faixa aleatoria da pista, e de forma dinâmica
void criarEnemyCar(){
    float distY = 12.5;
    for (int i = 1; i <= QtdCarrosInimigos; i++){
        distY += i/3;
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
    for (int i = 0; i <= 20; i++){
        int x = i *24;
        int z = 0;
        if(i%2==0){
            z = (rand() % 35) + 11;
        }else{
            z = rand() % 35;
        }
        
        Clouds Nuvem = Clouds(x,60,z,textID[10]);
        VecNuvens.push_back(Nuvem);
    }
}

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
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glGenTextures(15, textID);//Gerando na memoria a textura com seu id
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
        glPushMatrix();
        float x = VecFaixasCentrais[i].getPosX();
        float y = VecFaixasCentrais[i].getPosY();
        float z = VecFaixasCentrais[i].getPosZ();
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
        float x = VecEnemyCars[i].getPosX();
        float y = VecEnemyCars[i].getPosY();
        float z = VecEnemyCars[i].getPosZ();
        glTranslatef(x,y,z);
        VecEnemyCars[i].DrawAllCar(RotacaoPneu);
        glPopMatrix();
    }
}

//Funcao de desenho do mundo completo, para chamar o desenho de todos os objetos, estaticos e dinâmicos
void drawWorld(){
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-4,4,-1,1,frustrum_min,40);
    glFrustum(-4,4,-1,1,0.9,60);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,1200,600);
    glLoadIdentity();
    gluLookAt(PosXGlobalCamera, PosYGlobalCamera,PosZGlobalCamera, //posição da câmera
              PosXApontaCamera, PosYApontaCamera,PosZApontaCamera,//Posição inicial da camera, //para onde a câmera olha
              0, 0, 1); //para onde o topo da câmera aponta
    //gluLookAt(PosXGlobalCamera, 9,12, //posição da câmera
    //          PosXGlobalCamera, PosYApontaCamera,10, //para onde a câmera olha
    //          0, 0, 1); //para onde o topo da câmera aponta

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

    for (int i = 0; i < VecArvores.size(); i++){
        VecArvores[i].DesenharArvore(textID[8],textID[9]);
    }

    for (int i = 0; i < VecPostes.size(); i++){
        VecPostes[i].DesenharPoste(textID[9]);
    }

    for (int i = 0; i < VecNuvens.size(); i++){
        VecNuvens[i].DesenharNuvem();
    }

    for (int i = 0; i < VecFaixasLargada.size(); i++){
        VecFaixasLargada[i].DesenharFaixa();
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
    }else if (Temporizador > 1 && Temporizador < 45){//Contador "1"
        Semaphore.drawSemaphore(RotacaoSemaforo, textID[1]);
    }else if (Temporizador >= 45 && Temporizador < 90){//Contador "2"
        Semaphore.drawSemaphore(RotacaoSemaforo, textID[2]);
    }else if (Temporizador >= 90 && Temporizador < 135){//Contador "3"
        Semaphore.drawSemaphore(RotacaoSemaforo, textID[3]);
    }else if (Temporizador >= 135 && Temporizador <= 180){//"GO!"
        Semaphore.drawSemaphore(RotacaoSemaforo, textID[4]);
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
        glViewport(200,100,850,500);
        glColor3f(1,0,0);
        glBindTexture(GL_TEXTURE_2D, textID_vel[int(MyCar.getVelocidade())]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(PosXGlobalCamera-0.8,10,12.45);
            glTexCoord2f(1.0,0.0); glVertex3f(PosXGlobalCamera+0.8,10,12.45);
            glTexCoord2f(1.0,1.0); glVertex3f(PosXGlobalCamera+0.8,10.03,13.01);
            glTexCoord2f(0.0,1.0); glVertex3f(PosXGlobalCamera-0.8,10.03,13.01);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, aux_text);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(PosXGlobalCamera-1.8,10,12.45);
            glTexCoord2f(1.0,0.0); glVertex3f(PosXGlobalCamera-0.8,10,12.45);
            glTexCoord2f(1.0,1.0); glVertex3f(PosXGlobalCamera-0.8,10.03,13.01);
            glTexCoord2f(0.0,1.0); glVertex3f(PosXGlobalCamera-1.8,10.03,13.01);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    if(TempoNitro>0 and TempoNitro <= 90){
        //Deixar a marca de pneu no chao ao usar o nitro **A ser feita**
        if ((TempoNitro-1)%10 == 0){
            /*
            glBegin(GL_QUADS);
                glTexCoord2f(0.0,0.0); glVertex3f(MyCar.getPosX(),MyCar.getPosY()+0.5,12.45);
                glTexCoord2f(1.0,0.0); glVertex3f(MyCar.getPosX(),MyCar.getPosY(),12.45);
                glTexCoord2f(1.0,1.0); glVertex3f(MyCar.getPosX()+0.1,MyCar.getPosY(),13.01);
                glTexCoord2f(0.0,1.0); glVertex3f(MyCar.getPosX()+0.1,MyCar.getPosY()+0.5,13.01);
            glEnd();
            */
        }

    }
   
    glutSwapBuffers();
}

//Função que vai realmente fazer a movimentação das coordenadas de cada objeto para a posição desejada
void ocioso(int v){
    if (Temporizador != 0 && Temporizador > 0){
        Temporizador ++;
    }
    //Movendo as nuvens de forma constante
    for (int i = 0; i < VecNuvens.size(); i ++){
        float PosX = VecNuvens[i].getPosX();
        PosX -= MovimentacaoNuvem;
        VecNuvens[i].setPosX(PosX);
    }
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
    if (Temporizador == 0){
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }else if (Temporizador > 1 && Temporizador <= 45){
        if (Temporizador > 30){
            RotacaoSemaforo -= 6;
        }
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }else if (Temporizador > 45 && Temporizador <= 90){
        if (Temporizador > 75){
            RotacaoSemaforo -= 6;
        }
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }else if (Temporizador > 90 && Temporizador <= 135){
        if (Temporizador > 120){
            RotacaoSemaforo -= 6;
        }
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }else if (Temporizador > 135 && Temporizador <= 180){
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }else if (Temporizador > 180 && Temporizador <=210){
        PosXGlobalCamera += (MyCar.getPosX() - PosXGlobalCamera)/10;
        PosXApontaCamera += (MyCar.getPosX() - PosXApontaCamera)/10;
        glutTimerFunc(1000.0/FPS, ocioso, 0);
        glutPostRedisplay();
    }else if (Temporizador != -1 && Temporizador != -2){
        //Definindo as coordenadas da textura do chão (indo de X-50 a 70, o mesmo para Y, para mudar a qtd da textura no chao, assim ela é replicada mantendo seu tamanho, e n sendo esticada se as coordenadas fossem 0 e 1 normal, que é o desejado)
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
        PosXApontaCamera = MyCar.getPosX() + 0.5;

        if (Pressed_Key[0] == 1){
            //W
            MyCar.DefineVelo(MyCar.getVelocidade() + 1.3);
        }
        if (Pressed_Key[1] == 1){
            //A
            MyCar.setPosX(MyCar.getPosX() - 0.07);
            MyCar.DefineVelo(MyCar.getVelocidade() - 0.7);
        }
        if (Pressed_Key[2] == 1){
            //S
            MyCar.DefineVelo(MyCar.getVelocidade() - 2.0);
        }
        if (Pressed_Key[3] == 1){
            //D
            MyCar.setPosX(MyCar.getPosX() + 0.07);
            MyCar.DefineVelo(MyCar.getVelocidade() - 0.7);
        }
        if (Pressed_Key[4] == 1){
            //D
            if (PosYApontaCamera > 0){
                PosYApontaCamera = PosYApontaCamera*(-1.0);
            }
        }
        if (Pressed_Key[4] == 0){
            //D
            if (PosYApontaCamera < 0){
                PosYApontaCamera = PosYApontaCamera*(-1.0);
            }
        }
        
        //Determinar as posições finais das Faixas Centrais
        for (int i = 0; i < MaxFaixaCentral; i ++){
            VecFaixasCentrais[i].DefineVelo(MyCar.getVelocidade(),MaxVelocidade);
            VecFaixasCentrais[i].MoverFaixaCentral();
        }

        //Movendo os carros inimigos com suas velocidades fixas em relação a velocidade relativa ao meu carro
        for (int i = 0; i < QtdCarrosInimigos; i ++){
            int veloFixa = VecEnemyCars[i].getVeloFixa();
            float PosX = VecEnemyCars[i].getPosX();
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
            VecEnemyCars[i].setPosX(PosX);
            VecEnemyCars[i].DefineVeloFixa(veloFixa);
            VecEnemyCars[i].DefineVeloRelativa(MyCar.getVelocidade(),MaxVelocidade);
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

        for (int i = 0; i < VecFaixasLargada.size(); i ++){
            VecFaixasLargada[i].DefineVelo(MyCar.getVelocidade(),MaxVelocidade);
            VecFaixasLargada[i].MoverFaixaChegada();
            Chegada += VecFaixasLargada[i].getPosY()/FPS;
        }
        
        if (Colisao == 0){
            float PosX, PosY;
            PosX = MyCar.getPosX();
            PosY = MyCar.getPosY();
            for (int i = 0; i < QtdCarrosInimigos; i ++){
                float XsupDir,YsupDir,XinfEsq,YinfEsq;
                XsupDir = VecEnemyCars[i].getPosX() + 1;//Canto superior direito
                YsupDir = VecEnemyCars[i].getPosY() + 0.5;

                XinfEsq = VecEnemyCars[i].getPosX();//Canto inferior esquerdo
                YinfEsq = VecEnemyCars[i].getPosY();

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
        if (TempoNitro != 0 and Nitro>=0){
            cout<<Nitro<<endl;
            TempoNitro+=3;
            if (TempoNitro <= 180){
                MaxVelocidade = 270.0;
                if (TempoNitro <= 90){
                    MyCar.DefineVelo(MyCar.getVelocidade() + 1.5);
                }else if(TempoNitro > 90 and TempoNitro <= 180){
                    MyCar.DefineVelo(MyCar.getVelocidade() - 2.0);
                }
            }else{
                TempoNitro = 0;
                MaxVelocidade = 240;
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