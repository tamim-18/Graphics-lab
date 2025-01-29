#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <valarray>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
void DrawAxis();
double H,W;
double degree=0.0;
void init();
struct Point{
    double x;
    double y;
    Point(double a,double b)
    {
        x=a;
        y=b;
    }
    Point(){x=0,y=0;}
};
vector<Point>v;
void printShape()
{
    glBegin(GL_TRIANGLES);
    glColor3f(1,1,1);
    Point a;
    for(int i=0;i<v.size();i++)
    {
        a=v[i];
        glVertex2f(a.x,a.y);
       
    }

    glEnd();
    glFlush();
}
Point translate (Point p,Point t)
{
    p.x+=t.x;
    p.y+=t.y;
    return p;
}
Point rotate(Point p,double angle)
{
    angle = angle * (3.1416/180);
    double pointVector[2] = {p.x,p.y};
    double rotationMatrix[2][2] = {{cos(angle),-sin(angle)},{sin(angle),cos(angle)}};
    double result[2] = {0,0};
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<2;j++)
        {
            result[i] += pointVector[j] * rotationMatrix[i][j];
        }
    }
    return Point(result[0],result[1]);
}

void populatePoint(double degree)
{
    v.clear();
    v.push_back(Point(4,10));
    v.push_back(Point(1,7));
    v.push_back(Point(7,7));
    Point a,t;
    t.x=4;
    t.y=10;
    for(int i=0;i<v.size();i++)
    {
        
        v[i]=translate(v[i],Point(-t.x,-t.y));
        v[i]=rotate(v[i],degree);
        v[i]=translate(v[i],t);
    }
}
static void display(void)
{
    
    glClear (GL_COLOR_BUFFER_BIT);
    DrawAxis();
    populatePoint(degree);
    printShape();
    glutSwapBuffers();

}


void key(int key,int x,int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            degree+=1.0;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            degree-=1.0;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}




int main(int argc, char *argv[])
{
    int H=600,W=600;
    glutInit(&argc, argv);
    glutInitWindowSize(W,H);
    glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("Lab 4A - Task 1");
    init();
    glutDisplayFunc(display);
    glutSpecialFunc(key);
    glutMainLoop();

    return EXIT_SUCCESS;
}

void init() {
    H=30;
    W=30;
    glClearColor(0,0,0,0.0);
    glOrtho((-1.0)*(W/2),(H/2),(-1.0)*(W/2),(H/2),-10.0,10.0);
    glMatrixMode(GL_PROJECTION);
}
void DrawAxis ()
{
    
    glBegin(GL_LINES);
    glColor3f(1,1,1);
    glVertex2i((-1.0)*(W/2),0);
    glVertex2i((W/2),0);
    glVertex2i(0,(-1.0)*(H/2));
    glVertex2i(0,(H/2));
    glEnd();
    glFlush();

}
