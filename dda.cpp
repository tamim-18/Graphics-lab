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
float x1 = 2, y_1 = 6;
float x2 = 6, y2 = 13;

// Multiply by an arbitrary constant to fit in screen
const int SCALE = 50;
const int OFFSET = 150; // Move down by 150 pixels
float X1 = x1 * SCALE, Y1 = y_1 * SCALE - OFFSET;
float X2 = x2 * SCALE, Y2 = y2 * SCALE - OFFSET;

void drawDDA() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0); // Red color

    float dx = X2 - X1, dy = Y2 - Y1;
    float steps = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);
    float Xinc = dx / steps, Yinc = dy / steps;
    
    float X = X1, Y = Y1;

    printf("Plotted Points (DDA Algorithm):\n");

    for (int i = 0; i <= steps; i++) {
        // Print the current point in the console
        printf("Point %d: (%d, %d)\n", i, (int)X, (int)Y);

        // Create a dotted effect by skipping some points
        if (i % 5 == 0) {
            glBegin(GL_POINTS);
            glVertex2i((int)X, (int)Y);
            glEnd();
            glFlush();
        }

        SLEEP(50); // Add delay (50ms)

        X += Xinc;
        Y += Yinc;
    }

    // Ensure final point is rendered
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
    glutCreateWindow("DDA Algorithm - C (Fixed y1 Error)");
    init();
    glutDisplayFunc(drawDDA);
    glutMainLoop();
    return 0;
}
