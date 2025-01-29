/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */
#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <bits/stdc++.h>
using namespace std;


float one = 0.01745329251;
float ang = 0;
float x[] = {4, 1, 4, 7};
float y[] = {8.5, 7, 10, 7};

void setPoint(float x, float y) {
    x -= 4; y -= 17;
    float xx = x*cos(ang) - y*sin(ang);
    float yy = x*sin(ang) + y*cos(ang);
    xx += 4; yy += 17;
    //cout<<fixed<<setprecision(2)<<xx<<","<<yy<<" "<<ang<<endl;
    glVertex3f(xx, yy, 0.0f);
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);

    //center point
    glColor3f (1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    glVertex3f(4, 17, 0.0f);
    glEnd();

    //polygon
    glColor3f (1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);

    for(int i=0; i<4; i++) {
        setPoint(x[i], y[i]);
    }




    glEnd(); //End quadrilateral coordinates

    glFlush ();
    glutPostRedisplay();
    glutSwapBuffers();
}

void key(int key,int x,int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            //dy++;
            ang = ang + one;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            //dy--;
            ang = ang - one;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            //dx++;
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT:
            //dx--;
            glutPostRedisplay();
            break;
    }
}

/* Program entry point */

void init (void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0, 100, -50.0, 100, -10.0, 10.0);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (600, 600);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Demo");
    init ();
    glutDisplayFunc(display);
    glutSpecialFunc(key);
    glutMainLoop();
    return 0;
}

