#include <GL/glut.h>
#include <cmath>
#include <iostream>

using namespace std;

float xc = 7, yc = 5;  // Floating-point center
float a = 5, b = 7;   // Semi-major and semi-minor axes

// Function to plot symmetric points
void setPixel(float x, float y) {
    int px1 = round(xc + x), py1 = round(yc + y);
    int px2 = round(xc - x), py2 = round(yc + y);
    int px3 = round(xc + x), py3 = round(yc - y);
    int px4 = round(xc - x), py4 = round(yc - y);

    glVertex2i(px1, py1);
    glVertex2i(px2, py2);
    glVertex2i(px3, py3);
    glVertex2i(px4, py4);

    // Print the points
    cout << "(" << px1 << "," << py1 << ") ";
    cout << "(" << px2 << "," << py2 << ") ";
    cout << "(" << px3 << "," << py3 << ") ";
    cout << "(" << px4 << "," << py4 << ") ";
    cout << endl;
}

// Midpoint Ellipse Algorithm
void drawEllipse() {
    float x = 0, y = b;
    float a2 = a * a, b2 = b * b;
    float fx = 0, fy = 2 * a2 * y;
    
    // Initial decision parameter for Region 1
    float p1 = b2 - (a2 * b) + (0.25 * a2);

    // Region 1: High slope
    while (fx < fy) {
        setPixel(x, y);
        x++;
        fx += 2 * b2;

        if (p1 < 0) {
            p1 += fx + b2;
        } else {
            y--;
            fy -= 2 * a2;
            p1 += fx - fy + b2;
        }
    }

    // Initial decision parameter for Region 2
    float p2 = (b2 * (x + 0.5) * (x + 0.5)) + (a2 * (y - 1) * (y - 1)) - (a2 * b2);

    // Region 2: Low slope
    while (y > 0) {
        setPixel(x, y);
        y--;
        fy -= 2 * a2;

        if (p2 >= 0) {
            p2 += a2 - fy;
        } else {
            x++;
            fx += 2 * b2;
            p2 += fx - fy + a2;
        }
    }
}

// OpenGL Display Function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    drawEllipse();
    glEnd();
    glFlush();
}

// OpenGL Initialization
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-30, 30, -30, 30);
}

// Main Function
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Midpoint Ellipse Algorithm (Float)");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
