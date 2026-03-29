#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>

// --- Global Variables for Animation ---
int scene = 1;

int timerTicks = 0;
float elapsedTime = 0.0f; // Scale to 300 seconds (5 mins)

float vikramX = -10.0f;
float vikramY = -2.0f;  
float legAngle = 0.0f;  
float legDirection = 1.0f;   

float betalX = 4.0f;    
float betalY = 3.0f;
float betalAngle = 180.0f; 
float swingDirection = 1.0f;

int winWidth = 800, winHeight = 600;

void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void drawEyes(float cx, float cy) {
    glColor3f(0.0f, 0.0f, 0.0f); // Black eyes
    glPointSize(6.0f);
    glBegin(GL_POINTS);
    glVertex2f(cx - 0.15f, cy + 0.1f);
    glVertex2f(cx + 0.15f, cy + 0.1f);
    glEnd();
}

void drawMoon() {
    glColor3f(0.9f, 0.9f, 0.8f);
    drawCircle(-5.0f, 5.0f, 1.2f, 50); 
}

void drawTree() {
    glColor3f(0.4f, 0.2f, 0.0f); // Fast brown
    glBegin(GL_QUADS);
    glVertex2f(3.5f, -4.0f);
    glVertex2f(4.5f, -4.0f);
    glVertex2f(4.5f, 2.0f); 
    glVertex2f(3.5f, 2.0f); 
    glEnd();

    glLineWidth(8.0f);
    glBegin(GL_LINES);
    glVertex2f(4.0f, 1.0f); glVertex2f(2.0f, 3.0f);
    glVertex2f(4.0f, 1.5f); glVertex2f(6.0f, 3.5f);
    glEnd();

    glColor3f(0.0f, 0.5f, 0.0f); 
    drawCircle(4.0f, 3.0f, 2.0f, 20);
    drawCircle(2.5f, 3.0f, 1.5f, 20);
    drawCircle(5.5f, 3.0f, 1.5f, 20);
    drawCircle(4.0f, 4.5f, 1.8f, 20);
}

void drawVikram(float x, float y, bool carryingBetal) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f); 
    
    float shoulderY = 1.0f;
    float legStartY = -0.8f;

    // Body (Rectangle)
    glColor3f(0.8f, 0.2f, 0.2f); // Royal red
    glBegin(GL_QUADS);
    glVertex2f(-0.6f, legStartY);
    glVertex2f(0.6f, legStartY);
    glVertex2f(0.5f, shoulderY + 0.2f);
    glVertex2f(-0.5f, shoulderY + 0.2f);
    glEnd();

    // Head (Circle)
    glColor3f(1.0f, 0.8f, 0.6f); // Skin color
    drawCircle(0.0f, 1.7f, 0.5f, 20);
    drawEyes(0.0f, 1.7f); 

    // Crown (Triangle)
    glColor3f(1.0f, 0.8f, 0.0f); // Gold
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.4f, 2.0f);
    glVertex2f(0.4f, 2.0f);
    glVertex2f(0.0f, 2.8f);
    glEnd();

    // Sword
    if (!carryingBetal) {
        glColor3f(0.8f, 0.8f, 0.8f); 
        glLineWidth(4.0f);
        glBegin(GL_LINES);
        float swordBaseX = 0.8f;
        float swordBaseY = 0.0f - sinf(legAngle * 3.14159f / 180.0f) * 0.5f;
        glVertex2f(swordBaseX, swordBaseY);
        glVertex2f(swordBaseX + 1.2f, swordBaseY + 0.5f); 
        glEnd();
    }

    // Arms
    glColor3f(1.0f, 0.8f, 0.6f); 
    glLineWidth(6.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.5f, shoulderY);
    if (carryingBetal) glVertex2f(-0.8f, 1.5f); 
    else glVertex2f(-0.8f, 0.0f + sinf(legAngle * 3.14159f / 180.0f) * 0.5f); 
    
    glVertex2f(0.5f, shoulderY);
    if (carryingBetal) glVertex2f(0.8f, 1.5f); 
    else glVertex2f(0.8f, 0.0f - sinf(legAngle * 3.14159f / 180.0f) * 0.5f);
    glEnd();

    // Legs
    glColor3f(1.0f, 0.8f, 0.6f);
    glBegin(GL_LINES);
    glVertex2f(-0.3f, legStartY);
    glVertex2f(-0.3f - sinf(legAngle * 3.14159f / 180.0f) * 0.8f, -2.0f); 
    glVertex2f(0.3f, legStartY);
    glVertex2f(0.3f + sinf(legAngle * 3.14159f / 180.0f) * 0.8f, -2.0f);
    glEnd();

    glPopMatrix();
}

void drawBetal(float x, float y, float angle) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f); 
    glRotatef(angle, 0.0f, 0.0f, 1.0f); 
    
    // Body (Rectangle) 
    glColor3f(0.7f, 0.8f, 0.9f); // Ghostly pale/white
    glBegin(GL_QUADS);
    glVertex2f(-0.4f, -0.6f);
    glVertex2f(0.4f, -0.6f);
    glVertex2f(0.3f, 0.6f);
    glVertex2f(-0.3f, 0.6f);
    glEnd();

    // Head (Circle)
    glColor3f(0.8f, 0.9f, 1.0f); // Brighter head
    drawCircle(0.0f, 1.0f, 0.45f, 20);
    drawEyes(0.0f, 1.0f); 

    // Arms
    glColor3f(0.7f, 0.8f, 0.9f); 
    glLineWidth(5.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.3f, 0.4f);
    glVertex2f(-0.8f, -0.2f);
    glVertex2f(0.3f, 0.4f);
    glVertex2f(0.8f, -0.2f);
    glEnd();

    // Legs
    glBegin(GL_LINES);
    glVertex2f(-0.2f, -0.6f);
    glVertex2f(-0.4f, -1.5f);
    glVertex2f(0.2f, -0.6f);
    glVertex2f(0.4f, -1.5f);
    glEnd();

    glPopMatrix();
}

void drawBackground() {
    // Night sky
    glColor3f(0.05f, 0.05f, 0.15f); 
    glBegin(GL_QUADS);
    glVertex2f(-10.0f, -4.0f); glVertex2f(10.0f, -4.0f);
    glVertex2f(10.0f, 10.0f); glVertex2f(-10.0f, 10.0f);
    glEnd();

    // Ground
    glColor3f(0.1f, 0.3f, 0.1f); 
    glBegin(GL_QUADS);
    glVertex2f(-10.0f, -10.0f); glVertex2f(10.0f, -10.0f);
    glVertex2f(10.0f, -4.0f); glVertex2f(-10.0f, -4.0f);
    glEnd();
}

void drawSuitorsAndPrincess(int state) {
    // state 0 = alive & standing, state 1 = dead & lying down, state 2 = resurrected & alive
    
    // Princess Body
    glColor3f(1.0f, 0.5f, 0.8f); // Pink dress
    glBegin(GL_QUADS);
    if (state == 1) { 
        glVertex2f(-2.0f, -5.5f); glVertex2f(2.0f, -5.5f);
        glVertex2f(2.0f, -4.5f); glVertex2f(-2.0f, -4.5f);
    } else { 
        glVertex2f(-0.8f, -6.0f); glVertex2f(0.8f, -6.0f);
        glVertex2f(0.5f, -2.5f); glVertex2f(-0.5f, -2.5f);
    }
    glEnd();
    
    // Princess Head
    glColor3f(1.0f, 0.8f, 0.6f); 
    if (state == 1) drawCircle(-2.5f, -5.0f, 0.4f, 20); 
    else drawCircle(0.0f, -2.0f, 0.4f, 20);

    // Suitor 1 (Blue)
    glColor3f(0.2f, 0.4f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-4.5f, -6.0f); glVertex2f(-3.0f, -6.0f);
    glVertex2f(-3.0f, -2.5f); glVertex2f(-4.5f, -2.5f);
    glEnd();
    glColor3f(1.0f, 0.8f, 0.6f); drawCircle(-3.75f, -2.0f, 0.4f, 20);

    // Suitor 2 (Green)
    glColor3f(0.2f, 0.8f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-6.5f, -6.0f); glVertex2f(-5.0f, -6.0f);
    glVertex2f(-5.0f, -2.5f); glVertex2f(-6.5f, -2.5f);
    glEnd();
    glColor3f(1.0f, 0.8f, 0.6f); drawCircle(-5.75f, -2.0f, 0.4f, 20);

    // Suitor 3 (Orange)
    glColor3f(1.0f, 0.6f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(3.0f, -6.0f); glVertex2f(4.5f, -6.0f);
    glVertex2f(4.5f, -2.5f); glVertex2f(3.0f, -2.5f);
    glEnd();
    glColor3f(1.0f, 0.8f, 0.6f); drawCircle(3.75f, -2.0f, 0.4f, 20);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT); 
    drawBackground();
    drawMoon();
    
    if (scene < 4 || scene == 8) {
        drawTree();
    }
    
    if (scene == 1) {
        drawBetal(betalX, betalY, betalAngle);
        drawVikram(vikramX, vikramY, false);
    }
    else if (scene == 2) {
        drawBetal(betalX, betalY, betalAngle);
        drawVikram(vikramX, vikramY, false);
    }
    else if (scene == 3) {
        drawVikram(vikramX, vikramY, true); 
        drawBetal(betalX, betalY, betalAngle); 
    }
    else if (scene == 4) {
        drawVikram(-7.0f, -2.0f, true); 
        drawBetal(-7.0f, -2.0f + 1.2f, -90.0f);
        
        glColor3f(0.1f, 0.1f, 0.1f); 
        glBegin(GL_QUADS); 
        glVertex2f(-7.0f, -7.0f); glVertex2f(8.0f, -7.0f); 
        glVertex2f(8.0f, 4.0f); glVertex2f(-7.0f, 4.0f); 
        glEnd();
        
        drawSuitorsAndPrincess(0);
    }
    else if (scene == 5) {
        drawVikram(-7.0f, -2.0f, true); 
        drawBetal(-7.0f, -2.0f + 1.2f, -90.0f);
        
        glColor3f(0.1f, 0.1f, 0.1f); 
        glBegin(GL_QUADS); 
        glVertex2f(-7.0f, -7.0f); glVertex2f(8.0f, -7.0f); 
        glVertex2f(8.0f, 4.0f); glVertex2f(-7.0f, 4.0f); 
        glEnd();
        
        drawSuitorsAndPrincess(1);
    }
    else if (scene == 6) {
        drawVikram(-7.0f, -2.0f, true); 
        drawBetal(-7.0f, -2.0f + 1.2f, -90.0f);
        
        glColor3f(0.1f, 0.1f, 0.1f); 
        glBegin(GL_QUADS); 
        glVertex2f(-7.0f, -7.0f); glVertex2f(8.0f, -7.0f); 
        glVertex2f(8.0f, 4.0f); glVertex2f(-7.0f, 4.0f); 
        glEnd();
        
        drawSuitorsAndPrincess(2);
    }
    else if (scene == 7) {
        drawTree();
        drawVikram(-7.0f, -2.0f, true); 
        drawBetal(-7.0f, -2.0f + 1.2f, -90.0f);
    }
    else if (scene == 8) {
        drawTree();
        drawVikram(-7.0f, -2.0f, false);
        drawBetal(betalX, betalY, betalAngle);
    }

    glutSwapBuffers(); 
}

void timer(int value) {
    timerTicks++;
    elapsedTime = timerTicks * 0.03f; // 30ms per tick

    // Scene timeline based on absolute elapsed time
    if (elapsedTime < 20.0f) {
        scene = 1; 
        if (vikramX < 3.0f) {
            vikramX += 0.015f; 
            legAngle += legDirection * 2.0f;
            if (legAngle > 45.0f || legAngle < -45.0f) legDirection *= -1;
        } else { legAngle = 0.0f; }
        
        betalAngle += swingDirection * 1.0f;
        if (betalAngle > 190.0f || betalAngle < 170.0f) swingDirection *= -1.0f;
    }
    else if (elapsedTime < 40.0f) {
        scene = 2; 
        float targetX = vikramX;
        float targetY = vikramY + 1.2f;
        betalX += (targetX - betalX) * 0.02f;
        betalY += (targetY - betalY) * 0.02f;
        betalAngle += (-90.0f - betalAngle) * 0.02f;
        legAngle = 0.0f; 
    }
    else if (elapsedTime < 60.0f) {
        scene = 3; 
        betalX = vikramX;
        betalY = vikramY + 1.2f;
        betalAngle = -90.0f;

        if (vikramX > -7.0f) {
            vikramX -= 0.015f;
            legAngle += legDirection * 2.0f;
            if (legAngle > 45.0f || legAngle < -45.0f) legDirection *= -1;
        } else { legAngle = 0.0f; }
    }
    else if (elapsedTime < 110.0f) { scene = 4; legAngle = 0.0f; } 
    else if (elapsedTime < 160.0f) { scene = 5; } 
    else if (elapsedTime < 210.0f) { scene = 6; } 
    else if (elapsedTime < 260.0f) { scene = 7; } 
    else if (elapsedTime < 300.0f) {
        scene = 8; 
        float targetX = 4.0f;
        float targetY = 3.0f;
        betalX += (targetX - betalX) * 0.02f;
        betalY += (targetY - betalY) * 0.02f;
        betalAngle += (180.0f - betalAngle) * 0.02f;
    }
    else {
        // End of 5 minutes: loop the animation
        timerTicks = 0; 
        vikramX = -10.0f;
        betalX = 4.0f;
        betalY = 3.0f;
        betalAngle = 180.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(30, timer, 0);
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluOrtho2D(-10.0, 10.0, -8.0, 8.0); 
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Vikram and Betal - 5 Minute Story Animation");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0); 
    glutMainLoop();
    return 0;
}
