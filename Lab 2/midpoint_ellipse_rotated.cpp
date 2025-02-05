#include <GL/glut.h>
#include <cmath>
#include <iostream>

using namespace std;

float a = 10, b = 5;   // Semi-major and semi-minor axes
float theta = 60 * M_PI / 180.0; // Convert 45° to radians
float xc = -9, yc = 9; // Center of the ellipse

// Function to plot rotated points
void setPixel(float x, float y) {
    // Apply rotation transformation
    float xRot = x * cos(theta) - y * sin(theta);
    float yRot = x * sin(theta) + y * cos(theta);

    // Translate to the given center
    int px = round(xRot + xc);
    int py = round(yRot + yc);

    glVertex2i(px, py);
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
        setPixel(-x, y);
        setPixel(x, -y);
        setPixel(-x, -y);

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
        setPixel(-x, y);
        setPixel(x, -y);
        setPixel(-x, -y);

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
    glutCreateWindow("Midpoint Ellipse Algorithm (Rotated)");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
