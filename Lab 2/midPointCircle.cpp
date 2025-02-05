#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

float xc = 3/2.0, yc = 7/2.0; // Floating-point center
float r = 43; // Floating-point radius

// Function to plot symmetrical points with rounding and print them
void setPixel(float x, float y) {
    int px1 = round(xc + x), py1 = round(yc + y);
    int px2 = round(xc - x), py2 = round(yc + y);
    int px3 = round(xc + x), py3 = round(yc - y);
    int px4 = round(xc - x), py4 = round(yc - y);
    int px5 = round(xc + y), py5 = round(yc + x);
    int px6 = round(xc - y), py6 = round(yc + x);
    int px7 = round(xc + y), py7 = round(yc - x);
    int px8 = round(xc - y), py8 = round(yc - x);

    // Print points to the console
    cout << "(" << px1 << "," << py1 << ") ";
    cout << "(" << px2 << "," << py2 << ") ";
    cout << "(" << px3 << "," << py3 << ") ";
    cout << "(" << px4 << "," << py4 << ") ";
    cout << "(" << px5 << "," << py5 << ") ";
    cout << "(" << px6 << "," << py6 << ") ";
    cout << "(" << px7 << "," << py7 << ") ";
    cout << "(" << px8 << "," << py8 << ") ";
    cout << endl;

    // Plot points in OpenGL
    glVertex2i(px1, py1);
    glVertex2i(px2, py2);
    glVertex2i(px3, py3);
    glVertex2i(px4, py4);
    glVertex2i(px5, py5);
    glVertex2i(px6, py6);
    glVertex2i(px7, py7);
    glVertex2i(px8, py8);
}

// Midpoint Circle Algorithm (Floating-Point Version)
void drawCircle() {
    int x = 0, y = round(r);
    int p = 1 - y; // Initial decision parameter

    while (x <= y) {
        setPixel(x, y);
        x++;

        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
    }
}

// OpenGL Display Function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    glPointSize(5.0);
    drawCircle();
    glEnd();
    glFlush();
}

// OpenGL Initialization
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-50, 50, -50, 50);
}

// Main Function
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Midpoint Circle Algorithm (Float)");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
