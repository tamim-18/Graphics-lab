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

float xc, yc;
void setPixel(float x, float y){
    double mul = 1.0/ sqrt(2);
    float xx = x * mul - y * mul + xc;
    float yy = x * mul + y * mul + yc;

    glVertex3f(xx, yy, 0.0);
    cout<<"("<<xx <<","<<yy<<")";
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    glVertex3f(0, 0, 0.0);
    glVertex3f(-9, 9, 0.0);

    float a = 10, b = 5;
    xc = -9, yc = 9;
    float x = 0, y = b;
    float fx = 0;
    float fy = 2*a*a*b;
    float p = b*b + 0.25*a*a - a*a*b;

    while(fx<fy){
        setPixel(x, y);
        setPixel(-x, y);
        setPixel(x, -y);
        setPixel(-x, -y);

        //cout<<"("<<x+xc <<","<<y+yc<<")"<<endl;

        x++;
        fx += 2*b*b;
        if(p < 0)
            p += (fx + b*b);
        else{
            y--;
            fy -= (2*a*a);
            p += (fx-fy+b*b);
       }

    }
    setPixel(x, y);
    setPixel(-x, y);
    setPixel(x, -y);
    setPixel(-x, -y);
    //cout<<"("<<x+xc <<","<<y+yc<<")"<<endl;

    p = b*b*(x + 0.5)*(x + 0.5) + a*a*(y - 1)*(y - 1) - a*a*b*b;
    while(y>0){

        y--;
        fy -= 2*a*a;
        if(p >= 0)
            p += (a*a - fy);
        else{
            x++;
            fx += 2*b*b;
            p += (fx-fy+a*a);
       }

        setPixel(x, y);
        setPixel(-x, y);
        setPixel(x, -y);
        setPixel(-x, -y);
        //cout<<"("<<x+xc <<","<<y+yc<<")"<<endl;
    }


    glEnd();
    glFlush();

}

void init(){
    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-30, 30.0, -30, 30.0, -10.0, 10.0);
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
