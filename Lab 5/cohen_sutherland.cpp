#include <GL/glut.h>
#include <iostream>
#include <vector>

using namespace std;

// Clipping window coordinates
float x_min, y_min, x_max, y_max;

// Line segment structure
struct Line {
    float x1, y1, x2, y2;
};

vector<Line> lines;  // Store input lines

// Cohen-Sutherland region codes
const int INSIDE = 0;  // 0000
const int LEFT = 1;     // 0001
const int RIGHT = 2;    // 0010
const int BOTTOM = 4;   // 0100
const int TOP = 8;      // 1000

// Function to compute region code
int computeCode(float x, float y) {
    int code = INSIDE;
    if (x < x_min)      code |= LEFT;
    else if (x > x_max) code |= RIGHT;
    if (y < y_min)      code |= BOTTOM;
    else if (y > y_max) code |= TOP;
    return code;
}

// Cohen-Sutherland Line Clipping Algorithm
bool cohenSutherlandClip(float &x1, float &y1, float &x2, float &y2) {
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
    bool accept = false;

    while (true) {
        if ((code1 == 0) && (code2 == 0)) {
            accept = true;
            break;
        } else if (code1 & code2) {
            break;
        } else {
            int code_out;
            float x, y;

            code_out = (code1 != 0) ? code1 : code2;

            if (code_out & TOP) {
                x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
                y = y_max;
            } else if (code_out & BOTTOM) {
                x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
                y = y_min;
            } else if (code_out & RIGHT) {
                y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
                x = x_max;
            } else if (code_out & LEFT) {
                y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
                x = x_min;
            }

            if (code_out == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            } else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
    return accept;
}

// OpenGL display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw clipping window (rectangle)
    glColor3f(0.0, 1.0, 0.0);  // Green
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
        if (cohenSutherlandClip(x1, y1, x2, y2)) {
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
    gluOrtho2D(0, 800, 0, 600);  // 2D coordinate system
}

// Main function
int main(int argc, char **argv) {
    cout << "Enter clipping rectangle (x_min y_min x_max y_max): ";
    cin >> x_min >> y_min >> x_max >> y_max;

    int N;
    cout << "Enter number of line segments: ";
    cin >> N;

    cout << "Enter " << N << " lines (x1 y1 x2 y2):\n";
    for (int i = 0; i < N; i++) {
        Line line;
        cin >> line.x1 >> line.y1 >> line.x2 >> line.y2;
        lines.push_back(line);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cohen-Sutherland Line Clipping");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
