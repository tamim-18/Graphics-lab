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

int color[25][25];
int x4[] = {1, -1, 0, 0};
int y4[] = {0, 0, -1, 1};

bool valid(int x, int y){
    if(min(x,y) < 0 || max(x,y) >24 || color[x][y] == 1)
        return false;
    return true;
}
void setPixel(float x, float y){
    glVertex3f(x, y, 0.0);
}

void boundary_fill(int x, int y) {
    //cout << x << " , " << y << endl;
    setPixel(x, y);
    color[x][y] = 1;

    for(int i = 0; i < 4; i++){
        int xx = x + x4[i];
        int yy = y + y4[i];

        if(valid(xx, yy))
            boundary_fill(xx, yy);
    }
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);

    glBegin(GL_POINTS);

    for(int i = 1; i <= 5; i++) {
        for(int j = 1; j <= 10; j++){
            if(i == 1 || i == 5 || j == 1 || j == 10) {
                //setPixel(i, j+i);
                color[i][j+i] = 1;
            }
        }
    }

    glColor3f(1.0, 1.0, 1.0);
    boundary_fill(2, 7);

    glEnd();
    glFlush();

}

void init(){
    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20.0, 20.0, -20.0, 20.0, -10.0, 10.0);
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
