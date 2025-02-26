#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Clipping window boundaries
float x_min, y_min, x_max, y_max;

// Structure for a point
struct Point {
    float x, y;
};

// Polygon vertices (original & clipped)
vector<Point> polygon, clippedPolygon;

// Function to read polygon from file
void readPolygonFromFile() {
    ifstream inputFile("polygon.txt"); // Open input file
    if (!inputFile) {
        cerr << "Error: Could not open polygon.txt\n";
        exit(1);
    }

    inputFile >> x_min >> y_min >> x_max >> y_max;

    int N;
    inputFile >> N;
    for (int i = 0; i < N; i++) {
        Point p;
        inputFile >> p.x >> p.y;
        polygon.push_back(p);
    }

    inputFile.close();
}

// Function to check if a point is inside the clipping window for a given edge
bool isInside(Point p, int edge) {
    switch (edge) {
        case 0: return p.x >= x_min;  // Left
        case 1: return p.x <= x_max;  // Right
        case 2: return p.y >= y_min;  // Bottom
        case 3: return p.y <= y_max;  // Top
    }
    return false;
}

// Function to compute intersection point with a given edge
Point getIntersection(Point p1, Point p2, int edge) {
    Point iP;
    float m;

    if (p1.x != p2.x) // Avoid division by zero
        m = (p2.y - p1.y) / (p2.x - p1.x);

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
    return iP;
}

// Sutherland-Hodgman Polygon Clipping Algorithm
void sutherlandHodgmanClip() {
    clippedPolygon = polygon; // Start with original polygon

    for (int edge = 0; edge < 4; edge++) {
        vector<Point> newPolygon;

        int n = clippedPolygon.size();
        for (int i = 0; i < n; i++) {
            Point p1 = clippedPolygon[i];
            Point p2 = clippedPolygon[(i + 1) % n]; // Next vertex (wrap around)

            bool inside1 = isInside(p1, edge);
            bool inside2 = isInside(p2, edge);

            if (inside1 && inside2) { 
                newPolygon.push_back(p2);  // Both inside, keep p2
            } else if (inside1 && !inside2) { 
                newPolygon.push_back(getIntersection(p1, p2, edge)); // Add intersection
            } else if (!inside1 && inside2) { 
                newPolygon.push_back(getIntersection(p1, p2, edge)); // Add intersection
                newPolygon.push_back(p2); // Keep p2
            }
        }
        clippedPolygon = newPolygon;
    }
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

    // Draw original polygon (red)
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    for (auto &p : polygon) {
        glVertex2f(p.x, p.y);
    }
    glEnd();

    // Perform polygon clipping
    sutherlandHodgmanClip();

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
    glutCreateWindow("Sutherland-Hodgman Polygon Clipping");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
