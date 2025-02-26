#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Clipping window
float x_min, y_min, x_max, y_max;

// Line structure
struct Line {
    float x1, y1, x2, y2;
};

vector<Line> lines;

// Function to read input from file
void readInputFromFile() {
    ifstream inputFile("input.txt");  // Open input file
    if (!inputFile) {
        cerr << "Error: Could not open input.txt\n";
        exit(1);
    }

    inputFile >> x_min >> y_min >> x_max >> y_max;

    int N;
    inputFile >> N;
    for (int i = 0; i < N; i++) {
        Line line;
        inputFile >> line.x1 >> line.y1 >> line.x2 >> line.y2;
        lines.push_back(line);
    }

    inputFile.close();
}

// Function to check if a point is inside the clipping window
bool isInside(float x, float y) {
    return (x >= x_min && x <= x_max && y >= y_min && y <= y_max);
}

// Function to check if a line is completely inside
bool isCompletelyInside(float x1, float y1, float x2, float y2) {
    return isInside(x1, y1) && isInside(x2, y2);
}

// Function to check if a line is completely outside
bool isCompletelyOutside(float x1, float y1, float x2, float y2) {
    return (x1 < x_min && x2 < x_min) || (x1 > x_max && x2 > x_max) ||
           (y1 < y_min && y2 < y_min) || (y1 > y_max && y2 > y_max);
}

// Midpoint Subdivision Clipping Algorithm
void midpointSubdivisionClip(float x1, float y1, float x2, float y2, int depth = 0) {
    if (depth > 10) return;  // Prevent infinite recursion

    // If the line is fully inside, draw it in blue
    if (isCompletelyInside(x1, y1, x2, y2)) {
        glColor3f(0.0, 0.0, 1.0);  // Blue color
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
        return;
    }

    // If the line is fully outside, discard it
    if (isCompletelyOutside(x1, y1, x2, y2)) return;

    // Compute the midpoint
    float xm = (x1 + x2) / 2.0;
    float ym = (y1 + y2) / 2.0;

    // Add a small threshold to stop infinite recursion
    if (abs(x2 - x1) < 0.1 && abs(y2 - y1) < 0.1) return;

    // Recursively clip both halves
    midpointSubdivisionClip(x1, y1, xm, ym, depth + 1);
    midpointSubdivisionClip(xm, ym, x2, y2, depth + 1);
}

// OpenGL display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw clipping rectangle (green)
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x_min, y_min);
        glVertex2f(x_max, y_min);
        glVertex2f(x_max, y_max);
        glVertex2f(x_min, y_max);
    glEnd();

    // Draw original lines (red)
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    for (auto &line : lines) {
        glVertex2f(line.x1, line.y1);
        glVertex2f(line.x2, line.y2);
    }
    glEnd();

    // Apply Midpoint Subdivision Clipping
    for (auto &line : lines) {
        midpointSubdivisionClip(line.x1, line.y1, line.x2, line.y2);
    }

    glFlush();
}

// OpenGL initialization
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);  // White background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
}

// Main function
int main(int argc, char **argv) {
    readInputFromFile();  // Read input from file

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Midpoint Subdivision Line Clipping");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
