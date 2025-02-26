#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Clipping window boundaries
float x_min, y_min, x_max, y_max;

// Structure for a point
struct Point {
    float x, y;
    bool intersection = false;  // Marks intersection points
};

// Polygons
vector<Point> subjectPolygon;  // Original (concave)
vector<Point> clippedPolygon;  // Clipped result

// Function to read concave polygon from file
void readPolygonFromFile() {
    ifstream inputFile("concave_polygon.txt");
    if (!inputFile) {
        cerr << "Error: Could not open concave_polygon.txt\n";
        exit(1);
    }

    inputFile >> x_min >> y_min >> x_max >> y_max;

    int N;
    inputFile >> N;
    for (int i = 0; i < N; i++) {
        Point p;
        inputFile >> p.x >> p.y;
        subjectPolygon.push_back(p);
    }

    inputFile.close();
}

// Function to check if a point is inside the clipping rectangle
bool isInside(Point p) {
    return (p.x >= x_min && p.x <= x_max && p.y >= y_min && p.y <= y_max);
}

// Function to compute intersection between a line and the clipping window
Point getIntersection(Point p1, Point p2, int edge) {
    Point iP;
    float dx = p2.x - p1.x, dy = p2.y - p1.y;
    float m = (dx == 0) ? 0 : dy / dx;  // Slope

    switch (edge) {
        case 0:  // Left (x = x_min)
            iP.x = x_min;
            iP.y = p1.y + m * (x_min - p1.x);
            break;
        case 1:  // Right (x = x_max)
            iP.x = x_max;
            iP.y = p1.y + m * (x_max - p1.x);
            break;
        case 2:  // Bottom (y = y_min)
            iP.y = y_min;
            iP.x = p1.x + (y_min - p1.y) / m;
            break;
        case 3:  // Top (y = y_max)
            iP.y = y_max;
            iP.x = p1.x + (y_max - p1.y) / m;
            break;
    }
    iP.intersection = true;
    return iP;
}

// Weiler-Atherton Polygon Clipping Algorithm
void weilerAthertonClip() {
    vector<Point> outputPolygon = subjectPolygon;  // Start with the subject polygon

    for (int edge = 0; edge < 4; edge++) {
        vector<Point> newPolygon;
        int n = outputPolygon.size();

        for (int i = 0; i < n; i++) {
            Point p1 = outputPolygon[i];
            Point p2 = outputPolygon[(i + 1) % n];  // Next vertex (wrap around)

            bool inside1 = isInside(p1);
            bool inside2 = isInside(p2);

            if (inside1 && inside2) {
                newPolygon.push_back(p2);  // Both inside, keep p2
            } else if (inside1 && !inside2) {
                newPolygon.push_back(getIntersection(p1, p2, edge)); // Add intersection
            } else if (!inside1 && inside2) {
                newPolygon.push_back(getIntersection(p1, p2, edge)); // Add intersection
                newPolygon.push_back(p2); // Keep p2
            }
        }

        outputPolygon = newPolygon; // Update for next iteration
    }

    clippedPolygon = outputPolygon;
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

    // Draw original concave polygon (red)
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    for (auto &p : subjectPolygon) {
        glVertex2f(p.x, p.y);
    }
    glEnd();

    // Perform polygon clipping
    weilerAthertonClip();

    // Draw clipped polygon (blue)
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINE_LOOP);
    for (auto &p : clippedPolygon) {
        glVertex2f(p.x, p.y);
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
    readPolygonFromFile();  // Read input from file

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Weiler-Atherton Polygon Clipping");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
