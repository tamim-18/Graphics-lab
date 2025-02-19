#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

#define PI 3.14159265

// Tank structure
struct Tank {
    float x, y;  // attributes
    float angle;  // angle of rotation
    float speed;  // speed of tank
    bool alive;  // alive or not
    float r, g, b; // Color
};

// Bullet structure
struct Bullet {
    float x, y;
    float angle;
    float speed;
    bool active;
};

// Initialize Tanks
Tank tank1 = { -30.0, 0.0, 0.0, 2.0, true, 0.0, 1.0, 0.0 };
Tank tank2 = { 30.0, 0.0, 180.0, 2.0, true, 0.0, 0.0, 1.0 };

vector<Bullet> bullets1, bullets2;

// Function to draw a tank
void drawTank(Tank tank) {
    if (!tank.alive) return;

    glPushMatrix(); // Save the current matrix
    glTranslatef(tank.x, tank.y, 0); // Translate to the tank position
    glRotatef(tank.angle, 0, 0, 1); // Rotate the tank

    // Tank body
    glColor3f(tank.r, tank.g, tank.b); // Set the color
    glBegin(GL_POLYGON);  // Draw the tank body
    glVertex2f(-5, -5);  // Bottom left
    glVertex2f(5, -5); // Bottom right
    glVertex2f(5, 5);  // Top right
    glVertex2f(-5, 5); // Top left
    glEnd();

    // Tank turret
    glColor3f(1.0, 1.0, 1.0);  // Set the color
    glBegin(GL_LINES);  // Draw the tank turret
    glVertex2f(0, 0); // Turret base
    glVertex2f(0, 7); // Turret top
    glEnd();

    glPopMatrix(); // Restore the saved matrix
}

// Function to draw bullets
void drawBullets(vector<Bullet>& bullets) {
    for (size_t i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            glColor3f(1.0, 1.0, 1.0);
            glPushMatrix();
            glTranslatef(bullets[i].x, bullets[i].y, 0);
            glBegin(GL_QUADS);
            glVertex2f(-2, -2);
            glVertex2f(2, -2);
            glVertex2f(2, 2);
            glVertex2f(-2, 2);
            glEnd();
            glPopMatrix();
        }
    }
}

// Function to render text on screen
void renderText(float x, float y, void* font, const char* text) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

// Function to detect collision
bool detectCollision(Tank& tank, Bullet& bullet) {
    return bullet.x > tank.x - 5 && bullet.x < tank.x + 5 &&
           bullet.y > tank.y - 5 && bullet.y < tank.y + 5;  
           // Check if bullet is within the tank's bounding box.. How this works?
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (!tank1.alive) {
        glColor3f(1.0, 0.0, 0.0);
        renderText(-10, 0, GLUT_BITMAP_HELVETICA_18, "Player 2 Wins!");
    } else if (!tank2.alive) {
        glColor3f(1.0, 0.0, 0.0);
        renderText(-10, 0, GLUT_BITMAP_HELVETICA_18, "Player 1 Wins!");
    } else {
        drawTank(tank1);
        drawTank(tank2);
        drawBullets(bullets1);
        drawBullets(bullets2);
    }

    glutSwapBuffers();
}

// Move bullets
void updateBullets(vector<Bullet>& bullets, Tank& enemy) {
    for (size_t i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            bullets[i].x += bullets[i].speed * cos(bullets[i].angle * PI / 180.0);
            bullets[i].y += bullets[i].speed * sin(bullets[i].angle * PI / 180.0);

            if (bullets[i].x > 50 || bullets[i].x < -50 || bullets[i].y > 50 || bullets[i].y < -50) {
                bullets[i].active = false;
            }   // Check if bullet is out of bounds  

            if (detectCollision(enemy, bullets[i])) {  // Check if bullet hits the enemy
                enemy.alive = false;
                bullets[i].active = false;
            }
        }
    }
}

// Timer function
void timer(int) {
    updateBullets(bullets1, tank2);  // Update the bullets for tank 1
    updateBullets(bullets2, tank1);  // Update the bullets for tank 2
    glutPostRedisplay();  // Redisplay the scene
    glutTimerFunc(16, timer, 0);  // Update the bullets every 16ms
}   // Update the bullets every 16ms  

// Keyboard controls for movement and shooting
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    // Tank 1 Controls (A, D, W, S, Space)
    case 'a': tank1.angle += 5; break;
    case 'd': tank1.angle -= 5; break;
    case 'w':
        tank1.x += tank1.speed * cos(tank1.angle * PI / 180.0);
        tank1.y += tank1.speed * sin(tank1.angle * PI / 180.0);
        break;

        // Tank 1 Controls (A, D, W, S, Space)
        // A: Rotate left
        // D: Rotate right
        // W: Move forward
        // S: Move backward
        // Space: Shoot
    case 's':
        tank1.x -= tank1.speed * cos(tank1.angle * PI / 180.0);  
        tank1.y -= tank1.speed * sin(tank1.angle * PI / 180.0);
        break;
    case ' ':
        bullets1.push_back({ tank1.x, tank1.y, tank1.angle, 3.0, true });
        break;

    // Tank 2 Controls (J, L, I, K, Enter)
    case 'j': tank2.angle += 5; break;
    case 'l': tank2.angle -= 5; break;
    case 'i':
        tank2.x += tank2.speed * cos(tank2.angle * PI / 180.0);
        tank2.y += tank2.speed * sin(tank2.angle * PI / 180.0);
        break;
    case 'k':
        tank2.x -= tank2.speed * cos(tank2.angle * PI / 180.0);
        tank2.y -= tank2.speed * sin(tank2.angle * PI / 180.0);
        break;
    case '\r': // Enter key for shooting
        bullets2.push_back({ tank2.x, tank2.y, tank2.angle, 3.0, true });
        break;

    case 27: // Escape key
        exit(0);
    }

    glutPostRedisplay();
}

// Initialize OpenGL settings
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50, 50, -50, 50, -1, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("2D Tank Battle");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}


/*
🎯 Final Step-by-Step Summary
1️⃣ Loop through all bullets in the vector.
2️⃣ Check if the bullet is active (skip if not).
3️⃣ Move the bullet forward using cos(angle) and sin(angle).
4️⃣ Check if the bullet is out of bounds (deactivate if yes).
5️⃣ Check if the bullet hits the enemy tank (destroy if yes).
6️⃣ Repeat for all bullets and update in real-time with timer().
*/