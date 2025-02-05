#include <GL/glut.h>
#include <cmath>
#include <iostream>

using namespace std;

float xc = 0, yc = 0; // Center of the ellipse
float a = 10, b = 5;  // Semi-major and semi-minor axes
float thetaStart = 30 * M_PI / 180.0; // Start angle in radians (30 degrees)
float thetaEnd = 270 * M_PI / 180.0;  // End angle in radians (120 degrees)

// Function to check if a point is within the desired angle range
bool isWithinSector(float x, float y) {
    float angle = atan2(y - yc, x - xc); // Compute angle relative to ellipse center
    if (angle < 0) angle += 2 * M_PI; // Normalize to [0, 2π]
    
    return (thetaStart <= angle && angle <= thetaEnd);
}

// Function to plot symmetric points within the sector
void setPixel(float x, float y) {
    if (isWithinSector(xc + x, yc + y)) glVertex2i(round(xc + x), round(yc + y));
    if (isWithinSector(xc - x, yc + y)) glVertex2i(round(xc - x), round(yc + y));
    if (isWithinSector(xc + x, yc - y)) glVertex2i(round(xc + x), round(yc - y));
    if (isWithinSector(xc - x, yc - y)) glVertex2i(round(xc - x), round(yc - y));
}

// Midpoint Ellipse Algorithm to plot a sector
void drawEllipseSector() {
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

// Function to draw sector boundary (radii)
void drawSectorLines() {
    glBegin(GL_LINES);
    glVertex2i(xc, yc);  // Center of ellipse
    glVertex2i(round(xc + a * cos(thetaStart)), round(yc + b * sin(thetaStart))); // Start boundary
    glVertex2i(xc, yc);
    glVertex2i(round(xc + a * cos(thetaEnd)), round(yc + b * sin(thetaEnd))); // End boundary
    glEnd();
}

// OpenGL Display Function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw ellipse sector
    glBegin(GL_POINTS);
    drawEllipseSector();
    glEnd();
    
    // Draw sector boundary
    drawSectorLines();

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
    glutCreateWindow("Ellipse Sector (Midpoint Algorithm)");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
