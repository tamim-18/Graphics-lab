#include <cmath>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <GL/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

float xc = 3/2.0, yc = 7/2.0;  // Circle center coordinates
int radius = 43;        // Circle radius

// Function to plot symmetric points
void setPixel(float x, float y) {
    glVertex3f(xc + x, yc + y, 0.0);
    glVertex3f(xc - x, yc + y, 0.0);
    glVertex3f(xc + x, yc - y, 0.0);
    glVertex3f(xc - x, yc - y, 0.0);
    glVertex3f(xc + y, yc + x, 0.0);
    glVertex3f(xc - y, yc + x, 0.0);
    glVertex3f(xc + y, yc - x, 0.0);
    glVertex3f(xc - y, yc - x, 0.0);

    // Print the coordinates
    cout << "(" << xc + x << "," << yc + y << ") ";
    cout << "(" << xc - x << "," << yc + y << ") ";
    cout << "(" << xc + x << "," << yc - y << ") ";
    cout << "(" << xc - x << "," << yc - y << ") ";
    cout << "(" << xc + y << "," << yc + x << ") ";
    cout << "(" << xc - y << "," << yc + x << ") ";
    cout << "(" << xc + y << "," << yc - x << ") ";
    cout << "(" << xc - y << "," << yc - x << ") ";
    cout << endl;
}

// Bresenham’s Circle Algorithm
void drawCircle() {
    int x = 0, y = radius;
    int d = 3 - 2 * radius;  // Decision parameter

    while (x <= y) {
        setPixel(x, y);
        x++;

        if (d < 0) {
            d += 4 * x + 6;
        } else {
            y--;
            d += 4 * (x - y) + 10;
        }
    }
}

// OpenGL display function
static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);  // Set drawing color to white
    glPointSize(5.0);
    glBegin(GL_POINTS);

    drawCircle();  // Call function to draw the circle

    glEnd();
    glFlush();
}

// OpenGL Initialization
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50, 50, -50, 50, -10, 10);  // Adjust viewport for better visibility
}

// Main function
int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("Bresenham Circle in OpenGL");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return EXIT_SUCCESS;
}
