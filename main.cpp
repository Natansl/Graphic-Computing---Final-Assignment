#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include "CarregarArquivo.cpp"
using namespace std;

#define PI 3.1415

static int Rot_rodas = 0, Rot_carro = 0, Rot_cam = 0;
static float Trans_carro_x = 0.0, Trans_carro_z = 0.0, Velocidade = 0.0;
GLfloat angle = 60, fAspect;
GLint filtro = GL_NEAREST;
float final = 1.0;

static GLubyte lado[2560][1920][3];
static GLubyte textImg[512][512][3];
static CarregarArquivo modeloCarro;
static CarregarArquivo modeloRodas;
static GLuint textura_id, textura_id2;

void DefineIluminacao (void)
{
    GLfloat luzAmbiente[4]= {0.2,0.2,0.2,1.0};
    GLfloat luzDifusa[4]= {0.7,0.7,0.7,1.0}; // "cor"
    GLfloat luzEspecular[4]= {1.0, 1.0, 1.0, 1.0}; // "brilho"
    GLfloat posicaoLuz[4]= {0.0, 5.0, 5.0, 1.0};
// Capacidade de brilho do material
    GLfloat especularidade[4]= {1.0,1.0,1.0,1.0};
    GLint especMaterial = 60;
// Define a refletância do material
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
// Define a concentração do brilho
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
// Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
// Define os parâmetros da luz de número 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
}

void carregarText(){
    try{
        ifstream arq("E:/UFOP/TrabalhoFinalCG/wall.bmp" ,ios::binary);
        char c;
        if(!arq)
            cout << "Erro ao abrir";
        for(int i = 0; i < 138 ; i++)
            c = arq.get();
        for(int i = 0; i < 512 ; i++)
            for(int j = 0; j < 512 ; j++)
            {
                c = arq.get();
                textImg[i][j][2] = c;
                c =  arq.get();
                textImg[i][j][1] = c ;
                c =  arq.get();
                textImg[i][j][0] = c;
            }

        arq.close();
        arq.clear();
    }catch(...){
        cout << "Erro ao ler imagem" << endl;
    }
    glGenTextures(1,&textura_id);
    glBindTexture(GL_TEXTURE_2D, textura_id);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 512, 512, 0, GL_RGB,GL_UNSIGNED_BYTE, textImg);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    try{
        ifstream arq("E:/UFOP/TrabalhoFinalCG/5cc42deac977e.bmp" ,ios::binary);
        char c;
        if(!arq)
            cout << "Erro ao abrir";
        for(int i = 0; i < 138 ; i++)
            c = arq.get();
        for(int i = 0; i < 2560 ; i++)
            for(int j = 0; j < 1920 ; j++)
            {
                c = arq.get();
                lado[i][j][2] = c;
                c =  arq.get();
                lado[i][j][1] = c ;
                c =  arq.get();
                lado[i][j][0] = c;
            }

        arq.close();
        arq.clear();
    }catch(...){
        cout << "Erro ao ler imagem" << endl;
    }

    glGenTextures(1,&textura_id2);
    glBindTexture(GL_TEXTURE_2D, textura_id2);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 2560, 1920, 0, GL_RGB,GL_UNSIGNED_BYTE, lado);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Piso(float scale, float height){

    glPushMatrix();
    glTranslatef(0.0, height, 0.0);
    glScalef(scale,scale,scale);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textura_id);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBegin(GL_POLYGON);
    glNormal3f(0,1,0); // Normal da face
    glTexCoord2f(scale,scale);
    glVertex3f(100.0, 0.0, 100.0);
    glTexCoord2f(0,scale);
    glVertex3f(-100.0, 0.0, 100.0);
    glTexCoord2f(0,0);
    glVertex3f(-100.0, 0.0, -100.0);
    glTexCoord2f(scale,0);
    glVertex3f(100.0, 0.0, -100.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void Skybox(float scale){

    glPushMatrix();
    glScalef(scale,scale,scale);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textura_id2);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBegin(GL_POLYGON); //Face frontal
    glNormal3f(0,0,1); // Normal da face
    glTexCoord2f(0.75,0.66); glVertex3f(100.0, 30.00, 100.0);
    glTexCoord2f(0.75,0.33); glVertex3f(100.00, -1.0, 100.0);
    glTexCoord2f(1.0,0.33); glVertex3f(-100.00, -1.0, 100.0);
    glTexCoord2f(1.0,0.66); glVertex3f(-100.0, 30.0, 100.0);
    glEnd();

    glBegin(GL_POLYGON); //Face Posterior
    glNormal3f(0,0,-1); // Normal da face
    glTexCoord2f(0.5,0.66); glVertex3f(100.0, 30.00, -100.0);
    glTexCoord2f(0.5,0.33); glVertex3f(100.00, -1.0, -100.0);
    glTexCoord2f(0.25,0.33); glVertex3f(-100.00, -1.0, -100.0);
    glTexCoord2f(0.25,0.66); glVertex3f(-100.0, 30.0, -100.0);
    glEnd();

    glBegin(GL_POLYGON); //Face Esquerda
    glNormal3f(-1,0,0); // Normal da face
    glTexCoord2f(0.75,0.66); glVertex3f(100.0, 30.00, 100.0);
    glTexCoord2f(0.75,0.33); glVertex3f(100.00, -1.0, 100.0);
    glTexCoord2f(0.5,0.33); glVertex3f(100.00, -1.0, -100.0);
    glTexCoord2f(0.5,0.66); glVertex3f(100.0, 30.0, -100.0);
    glEnd();

    glBegin(GL_POLYGON); //Face Direita
    glNormal3f(1,0,0); // Normal da face
    glTexCoord2f(0.0,0.66); glVertex3f(-100.0, 30.00, 100.0);
    glTexCoord2f(0.0,0.33); glVertex3f(-100.00, -1.0, 100.0);
    glTexCoord2f(0.25,0.33); glVertex3f(-100.00, -1.0, -100.0);
    glTexCoord2f(0.25,0.66); glVertex3f(-100.0, 30.0, -100.0);
    glEnd();

    glBegin(GL_POLYGON); //Face Superior
    glNormal3f(0,-1,0); // Normal da face
    glTexCoord2f(0.75,1.0); glVertex3f(100.0, 30.00, 100.0);
    glTexCoord2f(0.75,0.67); glVertex3f(-100.00, 30.0, 100.0);
    glTexCoord2f(1.0,0.67); glVertex3f(-100.00, 30.0, -100.0);
    glTexCoord2f(1.0,1.0); glVertex3f(100.0, 30.0, -100.0);
    glEnd();


    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void myModel(float scale, CarregarArquivo modelo){
  glPushMatrix();
  glScalef(scale,scale,scale);
  for (unsigned int j = 0; j < (modelo.faces).size(); ++j){
    glBegin ( GL_POLYGON );
    for (unsigned int i = 0; i < (modelo.faces[j]).size() ; ++i ){
      GLfloat vert[3] = {(modelo.vertices[modelo.faces[j][i][0]][0]),(modelo.vertices[modelo.faces[j][i][0]][1]),(modelo.vertices[modelo.faces[j][i][0]][2])};
      glVertex3fv ( vert );
    }
    glEnd();
  }
  glPopMatrix();
}

void Carro(int rot_carro, float trans_x,float trans_z){

    glPushMatrix();
    glTranslatef(trans_x,0.0,trans_z);
    glRotatef(rot_carro,0.0,1.0,0.0);

    glColor3f(0.25,0.5,0.35);
    myModel(0.15, modeloCarro);
  glPopMatrix();
}

void Rodas(int rot_rodas, int rot_carro, float trans_x,float trans_z){
  glColor3f(0.0,0.0,0.0);
  glPushMatrix();
    glTranslatef(trans_x - (2.3 * cos(rot_carro* PI/180) + 2.6 * sin(rot_carro* PI/180)),0.0,trans_z - (2.6 * cos(rot_carro* PI/180) - 2.3 * sin (rot_carro * PI/180)));
    glRotatef(rot_carro,0.0,1.0,0.0);
    glRotatef(Velocidade,1.0, 0.0, 0.0);
    myModel(0.15, modeloRodas);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(trans_x - (-2.3 * cos(rot_carro* PI/180) + 2.6 * sin(rot_carro* PI/180)),0.0,trans_z - (2.6 * cos(rot_carro* PI/180) + 2.3 * sin (rot_carro * PI/180)));
    glRotatef(rot_carro + 180,0.0,1.0,0.0);
    glRotatef(Velocidade,1.0, 0.0, 0.0);
    myModel(0.15, modeloRodas);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(trans_x - (2.3 * cos(rot_carro* PI/180) - 2.6 * sin(rot_carro* PI/180)),0.0,trans_z - (-2.6 * cos(rot_carro* PI/180) - 2.3 * sin (rot_carro * PI/180)));
    glRotatef(rot_carro + rot_rodas*1.5,0.0,1.0,0.0);
    glRotatef(Velocidade,1.0, 0.0, 0.0);
    myModel(0.15, modeloRodas);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(trans_x - (-2.3 * cos(rot_carro* PI/180) - 2.6 * sin(rot_carro* PI/180)),0.0,trans_z - (-2.6 * cos(rot_carro* PI/180) + 2.3 * sin (rot_carro * PI/180)));
    glRotatef(rot_carro + rot_rodas*1.5,0.0,1.0,0.0);
    glRotatef(Velocidade,1.0, 0.0, 0.0);
    myModel(0.15, modeloRodas);
  glPopMatrix();
}

void Desenha(void){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DefineIluminacao();
    int flag = 1;
    if (Velocidade < 0)
        flag = -1;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(Trans_carro_x - 25 * sin(Rot_cam*PI/180),10,Trans_carro_z - 25 * cos(Rot_cam*PI/180), Trans_carro_x,0, Trans_carro_z, 0,1,0); // Especifica posição do observador e do alvo

    glPushMatrix();
    Piso(3.0, -1.0);
    Skybox(3.0);
    Carro(Rot_carro, Trans_carro_x, Trans_carro_z);
    Rodas(Rot_rodas, Rot_carro, Trans_carro_x, Trans_carro_z);

    glPopMatrix();
    glutSwapBuffers();
}


void Teclado(unsigned char key, int x, int y){
  switch (key) {
    case 's':
      if (Velocidade > -0.25)
        Velocidade -= 0.25;
      glutPostRedisplay();
      break;

    case 'w':
      if (Velocidade < 1.25)
        Velocidade += 0.25;
      glutPostRedisplay();
      break;

    case 'a':
      if (Rot_rodas < 40)
        Rot_rodas = (Rot_rodas + 10) % 360;
      glutPostRedisplay();
      break;

    case 'd':
      if (Rot_rodas > -40)
        Rot_rodas = (Rot_rodas - 10) % 360;
      glutPostRedisplay();
      break;
  }
}

void idle(){
    int inc = 0;
    if (Rot_rodas > 0 && Rot_rodas < 25)
        inc = 2;
    else if (Rot_rodas >= 25 && Rot_rodas <= 45)
        inc = 3;
    else if (Rot_rodas < 0 && Rot_rodas > -25)
        inc = -2;
    else if (Rot_rodas <= -25 && Rot_rodas > -45)
        inc = -3;

    if (Velocidade > 0)
        Rot_carro += inc;
    else if (Velocidade < 0)
        Rot_carro -= inc;

    if (Rot_carro - Rot_cam > 35)
        Rot_cam += 3;
    else if (Rot_carro - Rot_cam < -35)
        Rot_cam -= 3;
    else if (Rot_carro - Rot_cam > 20)
        Rot_cam += 2;
    else if(Rot_carro - Rot_cam < -20)
        Rot_cam -= 2;
    else if (Rot_carro > Rot_cam)
        Rot_cam += 1;
    else if (Rot_carro < Rot_cam)
        Rot_cam -= 1;

    if (Velocidade > 0){
        Trans_carro_x = Trans_carro_x + Velocidade*sin((Rot_carro + Rot_rodas)*PI/180);
        Trans_carro_z = Trans_carro_z + Velocidade*cos((Rot_carro + Rot_rodas)*PI/180);

    }else {
        Trans_carro_x = Trans_carro_x + Velocidade*sin((Rot_carro - Rot_rodas)*PI/180);
        Trans_carro_z = Trans_carro_z + Velocidade*cos((Rot_carro - Rot_rodas)*PI/180);
    }

  glutPostRedisplay();
}

void AlteraTamanhoJanela (int w, int h){
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  if(h == 0) h = 1;

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(65, (GLfloat) w/(GLfloat) h, 0.5, 500);
}

void Inicializa(void){
  glClearColor (1.0, 1.0, 1.0, 0.0);
  glColor3f(0.4, 0.30, 0.68);
  angle = 60;
  glEnable(GL_DEPTH_TEST);

  carregarText();
  modeloCarro.Carregar("E:/UFOP/TrabalhoFinalCG/car.obj");
  modeloRodas.Carregar("E:/UFOP/TrabalhoFinalCG/rodas.obj");
}

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  glutInitWindowSize (800, 800);
  glutInitWindowPosition ((1280 - 800)/2, (800 - 800)/2);

  glutCreateWindow ("Computação Gráfica: Trabalho Final");

  Inicializa();
  glutDisplayFunc(Desenha);
  glutReshapeFunc(AlteraTamanhoJanela);
  glutKeyboardFunc(Teclado);
  glutIdleFunc(idle);
  glutMainLoop();
  return 0;
}
