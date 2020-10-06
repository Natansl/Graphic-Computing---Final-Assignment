#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include "CarregarArquivo.cpp"
using namespace std;

static int handx = 0, fingerfinalz = 0, shoulderx = 0, shouldery = 0,
                            shoulderz = 0, elbow = 0;

GLfloat angle = 60, fAspect;
CarregarArquivo modelo;

void Inicializa(void){
  glClearColor (1.0, 1.0, 1.0, 0.0);
  glColor3f(0.0, 0.3, 0.2);
  angle = 60;
  modelo.Carregar("E:/UFOP/TrabalhoFinalCG/TruckNoTire.obj");
}

void myModel(float scale){
    glPushMatrix();
    glScalef(scale,scale,scale);
    for (unsigned int j = 0; j < (modelo.faces).size(); ++j )
    {

        glBegin ( GL_POLYGON );

        for (unsigned int i = 0; i < (modelo.faces[j]).size() ; ++i )        {
            GLfloat vert[3] = {(modelo.vertices[modelo.faces[j][i][0]][0]),(modelo.vertices[modelo.faces[j][i][0]][1]),(modelo.vertices[modelo.faces[j][i][0]][2])};
            glVertex3fv ( vert );
        }

        glEnd( );
    }
    glPopMatrix();


}

void Desenha(void){
  glClear (GL_COLOR_BUFFER_BIT);

  glPushMatrix();
    glRotatef(90,0,1,0);
    myModel(0.005);
  glPopMatrix();

  glutSwapBuffers();
}




void AlteraTamanhoJanela (int w, int h){
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  if(h == 0) h = 1;

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(65, (GLfloat) w/(GLfloat) h, 0.5, 500);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0,0,13, 0,0,0, 0,1,0); // Especifica posição do observador e do alvo
}

void Teclado(unsigned char key, int x, int y){
  switch (key) {
  case 's':
    shoulderx= (shoulderx + 5) % 360;
    glutPostRedisplay();
    break;
  case 'S':
    shoulderx= (shoulderx - 5) % 360;
    glutPostRedisplay();
    break;
  case 'h':
    handx = (handx + 5) % 360;
    glutPostRedisplay();
    break;
  case 'H':
    handx= (handx - 5) % 360;
    glutPostRedisplay();
    break;
  case 'f':
    fingerfinalz = (fingerfinalz + 5) % 360;
    if(fingerfinalz >= 0)fingerfinalz = 0;
    glutPostRedisplay();
    break;
  case 'F':
    fingerfinalz = (fingerfinalz - 5) % 360;
    if(fingerfinalz <= -90)fingerfinalz = -90;
    glutPostRedisplay();
    break;
  case 'a':
    shouldery= (shouldery + 5) % 360;
    glutPostRedisplay();
    break;
  case 'A':
    shouldery= (shouldery - 5) % 360;
    glutPostRedisplay();
    break;
  case 'd':
    shoulderz = (shoulderz+ 5) % 360;
    glutPostRedisplay();
    break;
  case 'D':
    shoulderz = (shoulderz- 5) % 360;
    glutPostRedisplay();
    break;
  case 'e':
    elbow = (elbow + 5) % 360;
    if(elbow > 120 % 360) elbow = 120;
    glutPostRedisplay();
    break;
  case 'E':
    elbow = (elbow - 5) % 360;
    if(elbow < -120) elbow = -120;
    glutPostRedisplay();
    break;
  case 27:
    exit(0);
    break;
  default:
    break;
  }
}

int main(int argc, char** argv){
  glutInit(&argc, argv);

  glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

  glutInitWindowSize (800, 800);

  glutInitWindowPosition ((1280 - 800)/2, (800 - 800)/2);

  glutCreateWindow ("Computação Gráfica: Braço Mecânico Aramado");

  Inicializa();

  glutDisplayFunc(Desenha);

  glutReshapeFunc(AlteraTamanhoJanela);

  glutKeyboardFunc(Teclado);

  glutMainLoop();
  return 0;
}
