#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#ifdef _WIN32
    #include <windows.h> // For Sleep() on Windows
    #define SLEEP(ms) Sleep(ms)
#else
    #include <unistd.h> // For usleep() on Linux
    #define SLEEP(ms) usleep(ms * 1000)
#endif

// Window size
const int WIDTH = 800, HEIGHT = 600;

// Given endpoints (renamed `y1` to `y_1` to avoid conflicts)
int x1 = 2, y_1 = 6;
int x2 = 6, y2 = 13;

// Multiply by an arbitrary constant to fit in screen
const int SCALE = 50;
const int OFFSET = 150; // Move down by 150 pixels
int X1 = x1 * SCALE, Y1 = y_1 * SCALE - OFFSET;
int X2 = x2 * SCALE, Y2 = y2 * SCALE - OFFSET;

void drawBresenham() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0); // Green color

    int dx = abs(X2 - X1), dy = abs(Y2 - Y1);
    int sx = (X1 < X2) ? 1 : -1;
    int sy = (Y1 < Y2) ? 1 : -1;
    int err = dx - dy;
    
    int x = X1, y = Y1;
    int count = 0; // Counter for dot spacing

    printf("Plotted Points (Bresenham's Algorithm):\n");

    while (1) {
        // Create a DOTTED LINE effect: draw a dot every 5th point
        if (count % 5 == 0) {
            glBegin(GL_POINTS);
            glVertex2i(x, y);
            glEnd();
            glFlush();
        }

        // Print the point
        printf("Point %d: (%d, %d)\n", count, x, y);
        
        SLEEP(50); // Add delay (50ms)

        if (x == X2 && y == Y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 < dx) { err += dx; y += sy; }

        count++;
    }

    // Ensure final point is drawn
    glBegin(GL_POINTS);
    glVertex2i(X2, Y2);
    glEnd();
    glFlush();
}

// OpenGL initialization
void init() {
    glClearColor(0, 0, 0, 1); // Black background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Bresenham's Algorithm - C (Fixed y1 Error)");
    init();
    glutDisplayFunc(drawBresenham);
    glutMainLoop();
    return 0;
}
