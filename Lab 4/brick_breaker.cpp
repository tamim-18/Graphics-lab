#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

// Ball structure
struct Ball {
    float x, y;
    float dx, dy;
    float speed;
};

// Paddle structure
struct Paddle {
    float x, width;
};

// Brick structure
struct Brick {
    float x, y;
    bool active;
};

// Game variables
Ball ball = { 0.0, -30.0, 0.3, 0.5, 0.5 };
Paddle paddle = { 0.0, 20.0 };
vector<Brick> bricks;
int score = 0;

// Initialize bricks
void initBricks() {
    bricks.clear();
    for (float y = 30.0; y <= 45.0; y += 5.0) {
        for (float x = -40.0; x <= 40.0; x += 10.0) {
            bricks.push_back({ x, y, true });
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

// Draw paddle
void drawPaddle() {
    glPushMatrix();
    glTranslatef(paddle.x, -40, 0);
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(-paddle.width, 2);
    glVertex2f(paddle.width, 2);
    glVertex2f(paddle.width, -2);
    glVertex2f(-paddle.width, -2);
    glEnd();
    glPopMatrix();
}

// Draw ball
void drawBall() {
    glPushMatrix();
    glTranslatef(ball.x, ball.y, 0);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 30) {
        float rad = i * 3.14159265 / 180;
        glVertex2f(2 * cos(rad), 2 * sin(rad));
    }
    glEnd();
    glPopMatrix();
}

// Draw bricks
void drawBricks() {
    for (size_t i = 0; i < bricks.size(); i++) {
        if (bricks[i].active) {
            glPushMatrix();
            glTranslatef(bricks[i].x, bricks[i].y, 0);
            glColor3f(1.0, 0.0, 0.0);
            glBegin(GL_QUADS);
            glVertex2f(-5, 2);
            glVertex2f(5, 2);
            glVertex2f(5, -2);
            glVertex2f(-5, -2);
            glEnd();
            glPopMatrix();
        }
    }
}

// Check ball and brick collision
bool detectCollision(Brick& brick) {
    return ball.x > brick.x - 5 && ball.x < brick.x + 5 &&
           ball.y > brick.y - 2 && ball.y < brick.y + 2;
}

// Handle collisions
void checkCollisions() {
    for (size_t i = 0; i < bricks.size(); i++) {
        if (bricks[i].active && detectCollision(bricks[i])) {
            bricks[i].active = false;
            ball.dy *= -1;
            score++;
            cout << "Bricks Destroyed: " << score << endl;
        }
    }
}

// Update ball position
void updateBall() {
    ball.x += ball.dx * ball.speed;
    ball.y += ball.dy * ball.speed;

    // Wall collision
    if (ball.x > 48 || ball.x < -48) ball.dx *= -1;
    if (ball.y > 48) ball.dy *= -1;

    // Paddle collision
    if (ball.y < -38 && ball.x > paddle.x - paddle.width && ball.x < paddle.x + paddle.width) {
        ball.dy *= -1;
    }

    // Ball falls below paddle (Game Over)
    if (ball.y < -50) {
        cout << "Game Over! Final Score: " << score << endl;
        exit(0);
    }
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawPaddle();
    drawBall();
    drawBricks();

    // Display score
    glColor3f(1.0, 1.0, 0.0);
    string scoreText = "Bricks Destroyed: " + to_string(score);
    renderText(-48, 45, GLUT_BITMAP_HELVETICA_18, scoreText.c_str());

    glutSwapBuffers();
}

// Timer function
void timer(int) {
    updateBall();
    checkCollisions();
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// Handle paddle movement with arrow keys
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT: paddle.x -= 3; break;
        case GLUT_KEY_RIGHT: paddle.x += 3; break;
    }
    glutPostRedisplay();
}

// Initialize OpenGL settings
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50, 50, -50, 50, -1, 1);
    initBricks();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Brick Breaker");

    init();

    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
