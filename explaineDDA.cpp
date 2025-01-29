#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#ifdef _WIN32
    #include <windows.h> // For Sleep() on Windows
    #define SLEEP(ms) Sleep(ms) // Windows sleep function
#else
    #include <unistd.h> // For usleep() on Linux
    #define SLEEP(ms) usleep(ms * 1000) // Linux sleep function
#endif

// Define window size
const int WIDTH = 800, HEIGHT = 600;

// Define endpoints of the line (original points)
int x1 = 2, y_1 = 6;  // Renamed y1 to y_1 to avoid conflicts
int x2 = 6, y2 = 13;  

// Scale factor to enlarge the drawing on screen
const int SCALE = 50;  
const int OFFSET = 150; // Moves the line down by 150 pixels for better visibility

// Convert given points to scaled screen coordinates
float X1 = x1 * SCALE, Y1 = y_1 * SCALE - OFFSET;
float X2 = x2 * SCALE, Y2 = y2 * SCALE - OFFSET;

// Function to implement DDA Line Drawing Algorithm
void drawDDA() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
    glColor3f(1.0, 0.0, 0.0); // Set color to red

    // Calculate dx, dy
    float dx = X2 - X1;  
    float dy = Y2 - Y1;

    // Determine the number of steps (maximum difference)
    float steps = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);

    // Calculate increments for each step
    float Xinc = dx / steps;
    float Yinc = dy / steps;

    // Start plotting from the first point
    float X = X1, Y = Y1;
    int count = 0; // Counter for the dotted effect

    printf("Plotted Points (DDA Algorithm):\n");

    // Loop through the number of steps
    for (int i = 0; i <= steps; i++) {
        // Create a **dotted effect**: draw a point every 5th iteration
        if (count % 5 == 0) {
            glBegin(GL_POINTS);
            glVertex2i((int)X, (int)Y); // Plot the current point
            glEnd();
            glFlush(); // Render the point immediately
        }

        // Print the point to the console
        printf("Point %d: (%d, %d)\n", count, (int)X, (int)Y);

        // Add a small delay for visualization
        SLEEP(50);

        // Move to the next point
        X += Xinc;
        Y += Yinc;
        count++;
    }

    // Ensure the final point is drawn
    glBegin(GL_POINTS);
    glVertex2i(X2, Y2);
    glEnd();
    glFlush();
}

// OpenGL initialization function
void init() {
    glClearColor(0, 0, 0, 1); // Set background color to black
    glMatrixMode(GL_PROJECTION); 
    gluOrtho2D(0, WIDTH, 0, HEIGHT); // Set 2D projection with screen dimensions
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set display mode to single buffer and RGB color
    glutInitWindowSize(WIDTH, HEIGHT); // Set window size
    glutCreateWindow("DDA Algorithm - C (Dotted Line)"); // Create the window

    init(); // Call the initialization function

    // Register the drawing function
    glutDisplayFunc(drawDDA);

    glutMainLoop(); // Start the OpenGL event loop
    return 0;
}
