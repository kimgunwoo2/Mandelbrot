#define GLUT_DISABLE_ATEXIT_HACK
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECUreal_NO_WARNINGS
#include <gl/glut.h>
#include <math.h>
#define H 500
#define W 500
//구조체
typedef struct
{
   GLdouble x;
   GLdouble y;
}complex;

//전역변수
unsigned char  imgage[H][W];
double w, h;
double comp_x, comp_y;
int deltal;
bool initFlag = true;
GLint TopLeftX, TopLeftY, BottomRightX, BottomRightY;

//함수 선언
void renderMandelbrot();
void MyDisplay();
void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y);
void MyMouseMove(GLint X, GLint Y);
void MyInit();

int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB);
   glutInitWindowSize(W, H);
   glutInitWindowPosition(0, 0);
   glutCreateWindow("Mandelbrot");
   MyInit();
   glutDisplayFunc(MyDisplay);
   glutMouseFunc(MyMouseClick);
   glutMotionFunc(MyMouseMove);
   glutMainLoop();
   return 0;
}

void renderMandelbrot()
{
   int i, j, k;
   complex z;
   double a, b;
   double light;
   glBegin(GL_POINTS);

   for (i = 0; i < W; i++) {
      for (j = 0; j < H; j++) {
         a = w * i / (W - 1) - w / 2 + comp_x;
         b = h * j / (H - 1) - h / 2 + comp_y;
         z.x = 0;
         z.y = 0;
         for (k = 0; k < 100+deltal; k++) {
            double temp = z.x;
            z.x = z.x * z.x - z.y * z.y + a;
            z.y = 2 * temp*z.y + b;
            light = z.x*z.x + z.y * z.y;
            if (light > 4.) break;
         }
         
         if (light>4.) light = 1.;
         imgage[i][j] = (unsigned char)(light*255.);
         glColor3ub(imgage[i][j], imgage[i][j], imgage[i][j]);
         glVertex3i(i, j, 0.0f);
      }
   }
   glEnd();
   
}
void MyDisplay()
{
   if (initFlag) {   //망델브로 
      glClear(GL_COLOR_BUFFER_BIT);
      renderMandelbrot();
      initFlag = false;
      glFlush();
   }
   else if (initFlag == false)   //마우스 선택영역 표시
   {
      double light;
      glClear(GL_COLOR_BUFFER_BIT);
      glBegin(GL_POINTS);

      for (int i = 0; i < H; i++) {
         for (int j = 0; j < W; j++) {
            glColor3ub(imgage[i][j], imgage[i][j], imgage[i][j]);
            glVertex3i(i, j, 0.0f);
         }
      }
      glEnd();
      glColor3f(1.0, 0., 0.);
      glLineWidth(2.0);
      glBegin(GL_LINE_LOOP);
      glVertex3f(TopLeftX, (500 - TopLeftY), 0.0);
      glVertex3f(TopLeftX, (500 - BottomRightY), 0.0);
      glVertex3f(BottomRightX, (500 - BottomRightY), 0.0);
      glVertex3f(BottomRightX, (500 - TopLeftY), 0.0);
      glEnd();
   }
   
   glFlush();
}
void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) {
   int point_x, point_y;
   if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) {

      TopLeftX = X;
      TopLeftY = Y;
   }
   else if (Button == GLUT_LEFT_BUTTON && State == GLUT_UP) {
      point_x = TopLeftX + (BottomRightX - TopLeftX) / 2;
      point_y = TopLeftY + (BottomRightY - TopLeftY) / 2;
      comp_x += (point_x - W / 2)*w / W;
      comp_y -= (point_y - H / 2)*h / H;
      h = w = abs(BottomRightX - TopLeftX)*h / H;
      deltal = 10 * (int)(1 - log10(h) / log10(2.));
      initFlag = true;

      glutPostRedisplay();
   }
}
void MyMouseMove(GLint X, GLint Y) {

   BottomRightX = X;
   BottomRightY = Y;
   glutPostRedisplay();
}
void MyInit() {
   w = h = 2;
   comp_x = -0.5; comp_y = 0.0;
   glClearColor(1.0, 1.0, 1.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, 500., 0, 500., -1.0, 1.0);
}