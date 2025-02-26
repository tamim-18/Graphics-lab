#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

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

// Liang-Barsky Line Clipping Algorithm
bool liangBarskyClip(float &x1, float &y1, float &x2, float &y2) {
    float dx = x2 - x1, dy = y2 - y1;
    float t0 = 0.0, t1 = 1.0;
    float p[4] = {-dx, dx, -dy, dy};
    float q[4] = {x1 - x_min, x_max - x1, y1 - y_min, y_max - y1};

    for (int i = 0; i < 4; i++) {
        if (p[i] == 0) { // Parallel case
            if (q[i] < 0) return false; // Outside
        } else {
            float t = q[i] / p[i];
            if (p[i] < 0)
                t0 = max(t0, t); // Update entry point
            else
                t1 = min(t1, t); // Update exit point
        }
    }

    if (t0 > t1) return false; // Reject the line

    // Compute new clipped points
    x1 += t0 * dx;
    y1 += t0 * dy;
    x2 += t1 * dx;
    y2 += t1 * dy;

    return true;
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

    // Draw clipped lines (blue)
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    for (auto &line : lines) {
        float x1 = line.x1, y1 = line.y1, x2 = line.x2, y2 = line.y2;
        if (liangBarskyClip(x1, y1, x2, y2)) {
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
        }
    }
    glEnd();

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
    glutCreateWindow("Liang-Barsky Line Clipping");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
