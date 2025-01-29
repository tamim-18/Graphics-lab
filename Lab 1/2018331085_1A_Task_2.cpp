///2018331085

#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <bits/stdc++.h>
using namespace std;

void setPixel(float x, float y){
    glVertex3f(x, y, 0.0);
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);

    glBegin(GL_POINTS);

    float x1 = -300.0, y1 = 800.0, x2 = -500, y2 = 1500;
    float x = x1, dy = y2 - y1, dx = x2 - x1;
    float d = -dy;

    for(float y = y1; y <= y2; y++){
        if(y < 850 || y > 1450)
            cout << "(" << x << "," << y <<")";
        else cout << ".";

        setPixel(x, y);
        if(d >= 0){
            x--;
            d = d - 2*dy - 2*dx;
        }
        else {
            d = d - 2*dx;
        }
    }


    glEnd();
    glFlush();

}

void init(){
    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-600.0, 900.0, 600.0, 1600.0, -10.0, 10.0);
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(1024,1024);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("Demo");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return EXIT_SUCCESS;
}
