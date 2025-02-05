#include <GL/glut.h>
#include <cmath>
#include <iostream>

using namespace std;

int xc = 0, yc = 0; // Circle center
int r = 10;         // Circle radius
float startAngle = 30 * M_PI / 180.0; // Start angle in radians
float endAngle = 120 * M_PI / 180.0;  // End angle in radians

bool drawSector = true; // Set to false for arc (only curved part)

// Function to convert Cartesian coordinates to polar and check if within sector
bool isWithinSector(int x, int y) {
    float angle = atan2(y - yc, x - xc); // Compute angle in radians
    if (angle < 0) angle += 2 * M_PI; // Normalize to [0, 2π]
    
    return (startAngle <= angle && angle <= endAngle);
}

// Function to plot symmetric points using Midpoint Algorithm
void setPixel(int x, int y) {
    if (isWithinSector(xc + x, yc + y)) glVertex2i(xc + x, yc + y);
    if (isWithinSector(xc - x, yc + y)) glVertex2i(xc - x, yc + y);
    if (isWithinSector(xc + x, yc - y)) glVertex2i(xc + x, yc - y);
    if (isWithinSector(xc - x, yc - y)) glVertex2i(xc - x, yc - y);
    if (isWithinSector(xc + y, yc + x)) glVertex2i(xc + y, yc + x);
    if (isWithinSector(xc - y, yc + x)) glVertex2i(xc - y, yc + x);
    if (isWithinSector(xc + y, yc - x)) glVertex2i(xc + y, yc - x);
    if (isWithinSector(xc - y, yc - x)) glVertex2i(xc - y, yc - x);
}

// Midpoint Circle Algorithm to plot an arc or sector
void drawCircleSector() {
    int x = 0, y = r;
    int p = 1 - r; // Decision parameter

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

// Function to draw sector boundary (radii)
void drawSectorLines() {
    glBegin(GL_LINES);
    glVertex2i(xc, yc);
    glVertex2i(xc + r * cos(startAngle), yc + r * sin(startAngle)); // Start boundary
    glVertex2i(xc, yc);
    glVertex2i(xc + r * cos(endAngle), yc + r * sin(endAngle)); // End boundary
    glEnd();
}

// OpenGL Display Function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw arc or sector
    glBegin(GL_POINTS);
    drawCircleSector();
    glEnd();
    
    // Draw sector boundary if needed
    if (drawSector) {
        drawSectorLines();
    }

    glFlush();
}

// OpenGL Initialization
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-20, 20, -20, 20);
}

// Main Function
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Midpoint Circle Arc & Sector with Polar Coordinates");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
