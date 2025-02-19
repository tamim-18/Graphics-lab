#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

#define PI 3.14159265

// Structure for spaceship
struct Spaceship {
    float x, y;
    float speed;
};

// Structure for bullets
struct Bullet {
    float x, y;
    float speed;
    bool active;
};

// Structure for asteroids
struct Asteroid {
    float x, y;
    float speed;
    bool active;
};

// Initialize spaceship
Spaceship spaceship = { 0.0, -40.0, 2.0 };
std::vector<Bullet> bullets;
std::vector<Asteroid> asteroids;
int killCount = 0;  // Kill counter

// Function to display text on screen
void renderText(float x, float y, void* font, std::string text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

// Function to draw spaceship
void drawSpaceship() {
    glPushMatrix();
    glTranslatef(spaceship.x, spaceship.y, 0);

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(-5, -5);
    glVertex2f(5, -5);
    glVertex2f(0, 10);
    glEnd();

    glPopMatrix();
}

// Function to draw bullets
void drawBullets() {
    for (size_t i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            glColor3f(1.0, 1.0, 1.0);
            glPushMatrix();
            glTranslatef(bullets[i].x, bullets[i].y, 0);
            glBegin(GL_QUADS);
            glVertex2f(-1, -1);
            glVertex2f(1, -1);
            glVertex2f(1, 1);
            glVertex2f(-1, 1);
            glEnd();
            glPopMatrix();
        }
    }
}

// Function to draw asteroids
void drawAsteroids() {
    for (size_t i = 0; i < asteroids.size(); i++) {
        if (asteroids[i].active) {
            glColor3f(1.0, 0.0, 0.0);
            glPushMatrix();
            glTranslatef(asteroids[i].x, asteroids[i].y, 0);
            glBegin(GL_POLYGON);
            for (int j = 0; j < 360; j += 30) {
                float rad = j * PI / 180;
                glVertex2f(4 * cos(rad), 4 * sin(rad));
            }
            glEnd();
            glPopMatrix();
        }
    }
}

// Function to update bullets
void updateBullets() {
    for (size_t i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            bullets[i].y += bullets[i].speed;
            if (bullets[i].y > 50) {
                bullets[i].active = false;
            }
        }
    }
}

// Function to update asteroids
void updateAsteroids() {
    for (size_t i = 0; i < asteroids.size(); i++) {
        if (asteroids[i].active) {
            asteroids[i].y -= asteroids[i].speed;
            if (asteroids[i].y < -50) {
                asteroids[i].active = false;
            }
        }
    }
}

// Function to check collision
bool detectCollision(Asteroid& asteroid, Bullet& bullet) {
    return bullet.x > asteroid.x - 4 && bullet.x < asteroid.x + 4 &&
           bullet.y > asteroid.y - 4 && bullet.y < asteroid.y + 4;
}

// Function to handle asteroid destruction
void checkCollisions() {
    for (size_t i = 0; i < bullets.size(); i++) {
        if (bullets[i].active) {
            for (size_t j = 0; j < asteroids.size(); j++) {
                if (asteroids[j].active && detectCollision(asteroids[j], bullets[i])) {
                    bullets[i].active = false;
                    asteroids[j].active = false;
                    killCount++;  // Increment kill count
                }
            }
        }
    }
}

// Function to display objects
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawSpaceship();
    drawBullets();
    drawAsteroids();

    // Display kill count
    glColor3f(1.0, 1.0, 1.0);
    renderText(-48, 45, GLUT_BITMAP_HELVETICA_18, "Kills: " + std::to_string(killCount));

    glutSwapBuffers();
}

// Function to spawn new asteroids randomly
void spawnAsteroids() {
    if (rand() % 30 == 0) {  // Random chance to spawn an asteroid
        float xPos = (rand() % 80) - 40;
        asteroids.push_back({ xPos, 50, 0.5f, true });
    }
}

// Timer function
void timer(int) {
    updateBullets();
    updateAsteroids();
    checkCollisions();
    spawnAsteroids();

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// Function to handle arrow key movement
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:  // Left Arrow Key ⬅️
            spaceship.x -= spaceship.speed;
            break;
        case GLUT_KEY_RIGHT:  // Right Arrow Key ➡️
            spaceship.x += spaceship.speed;
            break;
    }
    
    glutPostRedisplay();
}

// Function to handle spacebar shooting
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case ' ':  // Spacebar to shoot 🔫
            bullets.push_back({ spaceship.x, spaceship.y, 2.0, true });
            break;
        case 27:  // Escape key to exit ❌
            exit(0);
    }
    
    glutPostRedisplay();
}

// Initialize OpenGL settings
void init() {
    srand(time(0));
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50, 50, -50, 50, -1, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Space Shooter");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);  // Register arrow key handler
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
