#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include "CarregarArquivo.cpp"
using namespace std;

#define PI 3.1415

static int Rot_rodas = 0, Rot_carro = 0, Rot_cam = 0;
static float Trans_carro_x = 0.0, Trans_carro_z = 0.0, Velocidade = 0.0;
GLfloat angle = 60, fAspect;

void Inicializa(void){
  glClearColor (1.0, 1.0, 1.0, 0.0);
  glColor3f(0.4, 0.30, 0.68);
  angle = 60;
  glEnable(GL_DEPTH_TEST);
}

void Piso(float scale, float height){
  glPushMatrix();
  glTranslatef(0.0, height, 0.0);
  glScalef(scale,scale,scale);
  glColor3f(0.1, 0.15, 0.28);
  glBegin(GL_POLYGON);
  glVertex3f(100.0, 0.0, 100.0);
  glVertex3f(-100.0, 0.0, 100.0);
  glVertex3f(-100.0, 0.0, -100.0);
  glVertex3f(100.0, 0.0, -100.0);
  glEnd();
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
  CarregarArquivo modelo;
  modelo.Carregar("E:/UFOP/TrabalhoFinalCG/car.obj");
  glPushMatrix();
  glTranslatef(trans_x,0.0,trans_z);
  glRotatef(rot_carro,0.0,1.0,0.0);
  glColor3f(0.45,0.6,0.4);
  myModel(0.15, modelo);
  glPopMatrix();
}

void Rodas(int rot_rodas, int rot_carro, float trans_x,float trans_z){
  CarregarArquivo modelo;
  modelo.Carregar("E:/UFOP/TrabalhoFinalCG/tire.obj");
  glColor3f(0.0,0.0,0.0);

  glPushMatrix();
    glTranslatef(trans_x - (2.3 * cos(rot_carro* PI/180) + 2.6 * sin(rot_carro* PI/180)),0.0,trans_z - (2.6 * cos(rot_carro* PI/180) - 2.3 * sin (rot_carro * PI/180)));
    glRotatef(rot_carro,0.0,1.0,0.0);
    myModel(0.15, modelo);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(trans_x - (-2.3 * cos(rot_carro* PI/180) + 2.6 * sin(rot_carro* PI/180)),0.0,trans_z - (2.6 * cos(rot_carro* PI/180) + 2.3 * sin (rot_carro * PI/180)));
    glRotatef(rot_carro + 180,0.0,1.0,0.0);
    myModel(0.15, modelo);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(trans_x - (2.3 * cos(rot_carro* PI/180) - 2.6 * sin(rot_carro* PI/180)),0.0,trans_z - (-2.6 * cos(rot_carro* PI/180) - 2.3 * sin (rot_carro * PI/180)));
    glRotatef(rot_carro + rot_rodas,0.0,1.0,0.0);
    myModel(0.15, modelo);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(trans_x - (-2.3 * cos(rot_carro* PI/180) - 2.6 * sin(rot_carro* PI/180)),0.0,trans_z - (-2.6 * cos(rot_carro* PI/180) + 2.3 * sin (rot_carro * PI/180)));
    glRotatef(rot_carro + rot_rodas,0.0,1.0,0.0);
    myModel(0.15, modelo);
  glPopMatrix();
}

void Desenha(void){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(Trans_carro_x - 20 * sin(Rot_cam*PI/180),10,Trans_carro_z - 20 * cos(Rot_cam*PI/180), Trans_carro_x,0,Trans_carro_z, 0,1,0); // Especifica posição do observador e do alvo

    glPushMatrix();
    Piso(1.0, -2.0);
    Carro(Rot_carro, Trans_carro_x, Trans_carro_z);
    Rodas(Rot_rodas, Rot_carro, Trans_carro_x, Trans_carro_z);

    glPopMatrix();
    glutSwapBuffers();
}


void Teclado(unsigned char key, int x, int y){
  switch (key) {
    case 's':
      if (Velocidade > 0.2)
        Velocidade -= 0.3;
      else if (Velocidade > -0.2)
        Velocidade -= -0.1;
      else
        Velocidade = 0.0;
      glutPostRedisplay();
      break;

    case 'w':
      if (Velocidade < 1)
        Velocidade += 0.1;
      glutPostRedisplay();
      break;

    case 'a':
      if (Rot_rodas < 55)
        Rot_rodas = (Rot_rodas + 5) % 360;
      glutPostRedisplay();
      break;

    case 'd':
      if (Rot_rodas > -55)
        Rot_rodas = (Rot_rodas - 5) % 360;
      glutPostRedisplay();
      break;
  }
}

void idle(){
if (Velocidade > 0){
    if (Rot_carro < Rot_rodas)
        Rot_carro += 2.5;
    else if (Rot_carro > Rot_rodas)
        Rot_carro -= 2.5;

    if (Rot_carro > Rot_cam)
        Rot_cam += 1.25;
    else if (Rot_carro < Rot_cam)
        Rot_cam -= 1.25;
}

  Trans_carro_x = Trans_carro_x + Velocidade*sin(Rot_carro*PI/180);
  Trans_carro_z = Trans_carro_z + Velocidade*cos(Rot_carro*PI/180);
  glutPostRedisplay();
}

void AlteraTamanhoJanela (int w, int h){
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  if(h == 0) h = 1;

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(65, (GLfloat) w/(GLfloat) h, 0.5, 500);
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
