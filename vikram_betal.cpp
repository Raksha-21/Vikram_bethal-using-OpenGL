#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>

// --- Global Variables ---
int scene = 0; // Starts at 0 for Title
int timerTicks = 0;
float elapsedTime = 0.0f; 

float vikramX = -10.0f, vikramY = -2.0f, legAngle = 0.0f, legDirection = 1.0f;   
float betalX = 4.0f, betalY = 3.0f, betalAngle = 180.0f, swingDirection = 1.0f;
float sageX = 10.0f, sageY = -2.0f, sageArmAngle = 0.0f;
float monkeyX = 8.0f, monkeyY = 8.0f;
bool jewelBlink = false;
int winWidth = 800, winHeight = 600;

// Basic 2D Drawing Helpers
void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_POLYGON);
    for (int i=0; i<num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        glVertex2f(r * cosf(theta) + cx, r * sinf(theta) + cy);
    }
    glEnd();
}

void drawEyes(float cx, float cy) {
    glColor3f(0,0,0); glPointSize(4.0f); glBegin(GL_POINTS);
    glVertex2f(cx-0.1f, cy+0.1f); glVertex2f(cx+0.1f, cy+0.1f); glEnd();
}

void drawMoonAndClouds() { 
    // Moon
    glColor3f(0.9f, 0.9f, 0.8f); drawCircle(-5.0f, 5.0f, 1.2f, 50); 
    // Clouds
    glColor3f(0.3f, 0.3f, 0.4f); 
    drawCircle(-2.0f, 6.0f, 0.8f, 20); drawCircle(-1.0f, 6.5f, 1.0f, 20); drawCircle(0.0f, 6.0f, 0.8f, 20);
    drawCircle(4.0f, 5.0f, 0.6f, 20); drawCircle(5.0f, 5.2f, 0.8f, 20); drawCircle(6.0f, 5.0f, 0.6f, 20);
}

void drawForestBG() {
    glColor3f(0.05f, 0.05f, 0.15f); glRectf(-10, -4, 10, 10);
    glColor3f(0.1f, 0.3f, 0.1f); glRectf(-10, -8, 10, -4);
}

// Pure 2D Palace Background (No Circles!)
void drawPalaceBackground() {
    // wall
    glColor3f(0.9f, 0.8f, 0.6f);
    glBegin(GL_QUADS); glVertex2f(-10, -7); glVertex2f(10, -7); glVertex2f(10, 7); glVertex2f(-10, 7); glEnd();
    
    // carpet
    glColor3f(0.7f, 0.1f, 0.1f);
    glBegin(GL_QUADS); glVertex2f(-10, -7); glVertex2f(10, -7); glVertex2f(10, -3); glVertex2f(-10, -3); glEnd();
    
    // carpet gold borders
    glColor3f(1.0f, 0.8f, 0.2f);
    glLineWidth(8.0f);
    glBegin(GL_LINES); 
    glVertex2f(-5.0f, -7); glVertex2f(-5.0f, -3); 
    glVertex2f(5.0f, -7);  glVertex2f(5.0f, -3); 
    glEnd();

    // pillars
    glColor3f(0.8f, 0.6f, 0.2f);
    for(int i=-8;i<=8;i+=8) {
        glBegin(GL_QUADS); glVertex2f(i,-7); glVertex2f(i+1,-7); glVertex2f(i+1,5); glVertex2f(i,5); glEnd();
    }
}


void drawTree() {
    glColor3f(0.4f, 0.2f, 0.0f); glRectf(3.5f, -4.0f, 4.5f, 2.0f);
    glLineWidth(8.0f); glBegin(GL_LINES); glVertex2f(4,1); glVertex2f(2,3); glVertex2f(4,1.5); glVertex2f(6,3.5); glEnd();
    glColor3f(0.0f, 0.5f, 0.0f); drawCircle(4,3,2,20); drawCircle(2.5,3,1.5,20); drawCircle(5.5,3,1.5,20); drawCircle(4,4.5,1.8,20);
}

// User Snippet: Throne
void drawThrone(float x, float y) {
    glColor3f(0.7, 0.5, 0.1);
    glBegin(GL_QUADS); glVertex2f(x-1, y-2); glVertex2f(x+1, y-2); glVertex2f(x+1, y); glVertex2f(x-1, y); glEnd();
    
    // Add red cushion
    glColor3f(0.8, 0.1, 0.1);
    glBegin(GL_QUADS); glVertex2f(x-0.8, y-2); glVertex2f(x+0.8, y-2); glVertex2f(x+0.8, y-1.5); glVertex2f(x-0.8, y-1.5); glEnd();
}

void drawFire(float x, float y, float scale) {
    glColor3f(1.0f, 0.3f, 0.0f); glBegin(GL_TRIANGLES); glVertex2f(x-scale, y); glVertex2f(x+scale, y); glVertex2f(x, y+scale*2.5); glEnd();
    glColor3f(1.0f, 0.8f, 0.0f); glBegin(GL_TRIANGLES); glVertex2f(x-scale/2, y); glVertex2f(x+scale/2, y); glVertex2f(x, y+scale*1.5); glEnd();
}

void drawKing(float x, float y, bool carryingBetal, bool onThrone, bool strike) {
    glPushMatrix(); glTranslatef(x, y, 0.0f); 
    float legStartY = -0.8f;
    glColor3f(0.8f, 0.2f, 0.2f); glRectf(-0.5f, legStartY, 0.5f, 1.2f); 
    
    // Face Details
    glColor3f(1.0f, 0.8f, 0.6f); drawCircle(0.0f, 1.5f, 0.4f, 20); 
    drawEyes(0.0f, 1.6f); 
    
    // Moustache
    glColor3f(0,0,0); glLineWidth(2.0f); glBegin(GL_LINES); glVertex2f(-0.15f, 1.45f); glVertex2f(0.15f, 1.45f); glEnd();

    // Crown
    glColor3f(1.0f, 0.8f, 0.0f); glBegin(GL_TRIANGLES); glVertex2f(-0.4, 1.8); glVertex2f(0.4, 1.8); glVertex2f(0.0, 2.6); glEnd(); 
    
    if (!carryingBetal && !onThrone) {
        glColor3f(0.8f, 0.8f, 0.8f); glLineWidth(4.0f); glBegin(GL_LINES);
        if (strike) { glVertex2f(0.8f, 1.0f); glVertex2f(2.0f, 0.0f); }
        else { glVertex2f(0.8f, 0.0f); glVertex2f(2.0f, 0.5f); }
        glEnd();
    }
    
    glColor3f(1.0f, 0.8f, 0.6f); glLineWidth(6.0f); glBegin(GL_LINES);
    glVertex2f(-0.5f, 1.0f); glVertex2f(-0.8f, carryingBetal ? 1.5f : 0.0f);
    glVertex2f(0.5f, 1.0f);  glVertex2f(0.8f, carryingBetal||strike ? 1.5f : 0.0f);
    glEnd();

    glColor3f(1.0f, 0.8f, 0.6f); glBegin(GL_LINES);
    if (onThrone) {
        glVertex2f(-0.3f, legStartY); glVertex2f(-0.3f, -1.5f); glVertex2f(-0.3f, -1.5f); glVertex2f(-0.8f, -2.5f);
        glVertex2f(0.3f, legStartY); glVertex2f(0.3f, -1.5f); glVertex2f(0.3f, -1.5f); glVertex2f(0.8f, -2.5f);
    } else {
        glVertex2f(-0.3f, legStartY); glVertex2f(-0.3f - sinf(legAngle*3.14f/180)*0.8, -2.0f); 
        glVertex2f(0.3f, legStartY); glVertex2f(0.3f + sinf(legAngle*3.14f/180)*0.8, -2.0f);
    }
    glEnd(); glPopMatrix();
}

void drawVetala(float x, float y, float angle, bool carried) {
    glPushMatrix(); glTranslatef(x, y, 0.0f); glRotatef(angle, 0.0f, 0.0f, 1.0f); 
    
    // Ghost Hair
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLES); glVertex2f(0.0f, 1.3f); glVertex2f(-0.6f, -0.6f); glVertex2f(0.6f, -0.6f); glEnd();

    // Body
    glColor3f(0.7f, 0.8f, 0.9f); 
    if (carried) glRectf(-0.3f, 0.1f, 0.3f, 0.6f); // Lifted body to reveal King's face!
    else glRectf(-0.3f, -0.6f, 0.3f, 0.6f); 

    // Head
    glColor3f(0.8f, 0.9f, 1.0f); drawCircle(0.0f, 1.0f, 0.45f, 20); drawEyes(0, 1.0f); 

    // Arms
    glColor3f(0.7f, 0.8f, 0.9f); glLineWidth(5.0f); glBegin(GL_LINES);
    if (carried) {
        glVertex2f(-0.3f, 0.4f); glVertex2f(-0.5f, 0.0f);
        glVertex2f(0.3f, 0.4f); glVertex2f(0.5f, 0.0f);
    } else {
        glVertex2f(-0.3f, 0.4f); glVertex2f(-0.8f, -0.2f); glVertex2f(0.3f, 0.4f); glVertex2f(0.8f, -0.2f);
    }
    glEnd();

    // Legs
    glBegin(GL_LINES);
    if (carried) {
        // Legs draping wide over the shoulders to avoid covering the face
        glVertex2f(-0.3f, 0.1f); glVertex2f(-0.6f, -1.8f);
        glVertex2f(0.3f, 0.1f); glVertex2f(0.6f, -1.8f);
    } else {
        glVertex2f(-0.2f, -0.6f); glVertex2f(-0.4f, -1.5f); glVertex2f(0.2f, -0.6f); glVertex2f(0.4f, -1.5f);
    }
    glEnd(); glPopMatrix();
}

void drawSage(float x, float y, float angle = 0.0f) {
    glPushMatrix(); glTranslatef(x, y, 0.0f); glRotatef(angle, 0.0f, 0.0f, 1.0f);
    float shoulderY = 1.0f, legStartY = -0.8f;
    glColor3f(1.0f, 0.5f, 0.0f); glRectf(-0.5f, legStartY, 0.5f, 1.2f); 
    glColor3f(1.0f, 0.8f, 0.6f); drawCircle(0.0f, 1.5f, 0.4f, 20); drawEyes(0.0f, 1.5f); 
    glColor3f(0.8f, 0.8f, 0.8f); drawCircle(0.0f, 1.1f, 0.35f, 15); 
    glColor3f(1.0f, 0.8f, 0.6f); glLineWidth(6.0f); glBegin(GL_LINES);
    glVertex2f(-0.5f, shoulderY); glVertex2f(-0.8f, 0.0f + sinf(sageArmAngle)*0.5f);
    glVertex2f(0.5f, shoulderY); glVertex2f(0.8f, 0.0f - sinf(sageArmAngle)*0.5f); glEnd();
    glColor3f(1.0f, 0.8f, 0.6f); glBegin(GL_LINES);
    glVertex2f(-0.3f, legStartY); glVertex2f(-0.3f, -2.0f); 
    glVertex2f(0.3f, legStartY); glVertex2f(0.3f, -2.0f); glEnd();
    glPopMatrix();
}

// User Snippet: Soldier
void drawSoldier(float x, float y) {
    // body
    glColor3f(0.2,0.3,0.8);
    glBegin(GL_QUADS); glVertex2f(x-0.3,y); glVertex2f(x+0.3,y); glVertex2f(x+0.3,y-1.5); glVertex2f(x-0.3,y-1.5); glEnd();
    // head
    glColor3f(1.0f, 0.8f, 0.6f); drawCircle(x,y+0.5,0.3, 20);
    // helmet
    glColor3f(0.5f,0.5f,0.5f); drawCircle(x,y+0.6,0.3, 20);
    // spear
    glColor3f(0.6,0.6,0.6); glLineWidth(3.0f); glBegin(GL_LINES); glVertex2f(x+0.5,y-2); glVertex2f(x+0.5,y+2); glEnd();
}

void drawMonkey(float x, float y) {
    glColor3f(0.4f, 0.2f, 0.0f); drawCircle(x, y, 0.4f, 15); drawCircle(x, y+0.4f, 0.3f, 15); 
    glLineWidth(4.0f); glBegin(GL_LINES); glVertex2f(x-0.4f, y); glVertex2f(x-0.8f, y+0.4f); glEnd();
    if(jewelBlink) { glColor3f(0,1,1); drawCircle(x-0.8f, y+0.4f, 0.2f, 8); } 
}

void drawSpeechBubble() {
    glColor3f(0.9f, 0.9f, 0.9f); glRectf(-3, 3, 3, 6);
    glBegin(GL_TRIANGLES); glVertex2f(1, 3); glVertex2f(2, 3); glVertex2f(0, 1); glEnd();
    glColor3f(0,0,0); glLineWidth(3.0f); glBegin(GL_LINES); glVertex2f(-2, 5); glVertex2f(2, 5); glVertex2f(-2, 4); glVertex2f(2, 4); glEnd();
}

void drawText(float x, float y, const char* str) {
    glColor3f(1.0f, 1.0f, 0.0f); 
    glRasterPos2f(x, y);
    while (*str) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str++);
}
void drawTitleText(float x, float y, const char* str) {
    glColor3f(1.0f, 0.8f, 0.0f); 
    glRasterPos2f(x, y);
    while (*str) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *str++);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT); 
    
    if (scene == 0) {
        // Title Screen
        glColor3f(0.0f, 0.0f, 0.0f); glRectf(-10.0f, -8.0f, 10.0f, 10.0f);
        drawTitleText(-3.5f, 0.0f, "VIKRAM AND BETAL");
        drawTitleText(-2.0f, -1.0f, "A Graphics Project");
    } else {
        // Flat 2D Backgrounds
        if (scene <= 3 || scene == 10) { 
            drawPalaceBackground(); 
        } else { 
            drawForestBG(); drawMoonAndClouds(); 
        }
        
        // Characters
        if (scene == 1) { 
            drawThrone(-5, -0.5f); drawKing(-5, -0.5, false, true, false); 
            drawSoldier(-8, -1.0f); drawSoldier(-2, -1.0f);
            drawSage(sageX, sageY);
        }
        else if (scene == 2) { 
            drawThrone(-5, -0.5f); drawKing(-5, -0.5, false, true, false); 
            drawSage(sageX, sageY); drawMonkey(monkeyX, monkeyY);
        }
        else if (scene == 3) { 
            drawThrone(-5, -0.5f); drawKing(-5, -0.5, false, true, false); 
            drawSage(sageX, sageY); drawSpeechBubble();
        }
        else if (scene == 4) { drawKing(vikramX, -2.0f, false, false, false); }
        else if (scene == 5) { drawTree(); drawVetala(4, 3, betalAngle, false); drawKing(vikramX, vikramY, false, false, false); }
        else if (scene == 6) { drawTree(); drawKing(vikramX, vikramY, true, false, false); drawVetala(vikramX, vikramY+2.0f, 0.0f, true); }
        else if (scene == 7) { 
            drawTree(); drawKing(vikramX, vikramY, true, false, false);
            if (elapsedTime > 285 && elapsedTime < 315) drawSpeechBubble();
            drawVetala(betalX, betalY, betalAngle, (elapsedTime <= 315.0f));
        }
        else if (scene == 8) { 
            drawTree(); drawKing(vikramX, vikramY, false, false, false); drawVetala(betalX, betalY, betalAngle, false);
            if (elapsedTime > 345 && elapsedTime < 375) { glColor3f(1,0,0); drawSpeechBubble(); } 
        }
        else if (scene == 9) { drawFire(-2, -4, 1.5f); drawSage(sageX, sageY, 45.0f); drawKing(vikramX, vikramY, false, false, true); }
        else if (scene == 10) { 
            drawThrone(-5, -0.5f); drawKing(-5, -0.5, false, true, false); 
            drawSoldier(-8, -1.0f); drawSoldier(-2, -1.0f);
        }
        
        // Subtitles Strip
        glColor3f(0.0f, 0.0f, 0.0f); glRectf(-10.0f, -8.0f, 10.0f, -5.5f);
        if (scene == 1) { 
            drawText(-9.5f, -6.0f, "Long ago, the brave and wise King Vikramaditya ruled");
            drawText(-9.5f, -6.6f, "his kingdom with justice. One day, a mysterious sage");
            drawText(-9.5f, -7.2f, "came to the royal court and offered the king a fruit.");
        }
        else if (scene == 2) { 
            drawText(-9.5f, -6.0f, "The sage visited the palace every day and gave a fruit.");
            drawText(-9.5f, -6.6f, "When a monkey broke one, a shining jewel fell out!");
            drawText(-9.5f, -7.2f, "The king realized every fruit contained a precious gem.");
        }
        else if (scene == 3) { 
            drawText(-9.5f, -6.0f, "The sage revealed his request: In a haunted forest there");
            drawText(-9.5f, -6.6f, "was a corpse hanging from a tree possessed by a spirit.");
            drawText(-9.5f, -7.2f, "He asked the king to bring it to complete a ritual.");
        }
        else if (scene == 4) { 
            drawText(-9.5f, -6.5f, "Late at night, King Vikramaditya set out on a");
            drawText(-9.5f, -7.1f, "dangerous journey to the haunted forest.");
        }
        else if (scene == 5) { 
            drawText(-9.5f, -6.0f, "In the middle of the cremation ground, the king found");
            drawText(-9.5f, -6.6f, "a body hanging upside down from a tree. When he lifted");
            drawText(-9.5f, -7.2f, "it, the body suddenly started laughing.");
        }
        else if (scene == 6) { 
            drawText(-9.5f, -6.0f, "The body was possessed by the spirit Vetala. The ghost");
            drawText(-9.5f, -6.6f, "agreed to go with the king but on one condition - he");
            drawText(-9.5f, -7.2f, "would tell a story and ask a riddle during the journey.");
        }
        else if (scene == 7) { 
            drawText(-9.5f, -6.0f, "Each time Vetala finished a story, he asked a difficult");
            drawText(-9.5f, -6.6f, "question. Whenever the king answered, the clever ghost");
            drawText(-9.5f, -7.2f, "escaped and flew back to the tree.");
        }
        else if (scene == 8) { 
            drawText(-9.5f, -6.0f, "Finally, Vetala revealed a secret: The sage who had sent");
            drawText(-9.5f, -6.6f, "the king actually planned to sacrifice him in a dark");
            drawText(-9.5f, -7.2f, "ritual to gain magical powers.");
        }
        else if (scene == 9) { 
            drawText(-9.5f, -6.0f, "When the king returned, he cleverly asked the sage to");
            drawText(-9.5f, -6.6f, "demonstrate the ritual bow. As the sage bent down,");
            drawText(-9.5f, -7.2f, "the king defeated him and stopped the evil plan.");
        }
        else if (scene == 10) { 
            drawText(-9.5f, -6.0f, "Impressed by the king's bravery and wisdom, Vetala");
            drawText(-9.5f, -6.6f, "praised him. King Vikramaditya returned safely to his");
            drawText(-9.5f, -7.2f, "kingdom and continued ruling with justice.");
        }
        
        glColor3f(0.5f, 0.5f, 0.5f); glRasterPos2f(2.0f, -7.5f);
        const char* help = "Tip: Press 'SPACE' to skip to the next scene";
        while (*help) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *help++);
    }
    glutSwapBuffers(); 
}

void timer(int value) {
    timerTicks++; elapsedTime = timerTicks * 0.04f;
    jewelBlink = (timerTicks % 10 < 5);

    if (elapsedTime < 5.0f) { scene = 0; }
    else if (elapsedTime < 50.0f) { scene = 1; if (sageX > 3.0f) sageX -= 0.05f; sageArmAngle += 0.1f; }
    else if (elapsedTime < 90.0f) { scene = 2; if (monkeyX > 1.0f) { monkeyX -= 0.05f; monkeyY = -1.0f + sinf(elapsedTime*5)*2; } else monkeyY = -2.0f; }
    else if (elapsedTime < 135.0f) { scene = 3; sageArmAngle += 0.2f; }
    else if (elapsedTime < 185.0f) { 
        scene = 4; if (elapsedTime < 136.0f) { vikramX = -10.0f; vikramY = -2.0f; } 
        if (vikramX < 10.0f) { vikramX += 0.03f; legAngle += legDirection*2; if(legAngle>40||legAngle<-40) legDirection*=-1; }
    }
    else if (elapsedTime < 230.0f) { 
        scene = 5; if (elapsedTime < 186.0f) { vikramX = -10.0f; betalX = 4; betalY = 3; betalAngle=180; }
        if (vikramX < 3.0f) { vikramX += 0.03f; legAngle += legDirection*2; if(legAngle>40||legAngle<-40) legDirection*=-1; }
        else if (vikramY < 1.0f) { vikramY += 0.02f; legAngle = 0; }
    }
    else if (elapsedTime < 280.0f) { scene = 6; vikramY = -2.0f; legAngle = 0.0f; betalAngle = 0.0f; betalX = vikramX; betalY = vikramY + 2.0f; }
    else if (elapsedTime < 340.0f) { 
        scene = 7;
        if (elapsedTime < 285.0f) { 
            if (vikramX > -6.0f) { vikramX -= 0.03f; legAngle += legDirection*2; if(legAngle>40||legAngle<-40) legDirection*=-1; }
            betalX = vikramX; betalY=vikramY + 2.0f; betalAngle = 0.0f;
        } else if (elapsedTime > 315.0f) { 
            betalX += (4.0 - betalX)*0.05f; betalY += (3.0 - betalY)*0.05f; betalAngle += (180 - betalAngle)*0.05f;
        } else { legAngle = 0; } 
    }
    else if (elapsedTime < 385.0f) { scene = 8; if (elapsedTime < 341.0f) { vikramX = 2.0f; betalX = 4.0; betalY = 3.0; betalAngle=180; } }
    else if (elapsedTime < 430.0f) { 
        scene = 9; if (elapsedTime < 386.0f) { sageX = 2.0f; sageY = -3.0f; vikramX = 5.0f; vikramY = -2.0f; legAngle=0; }
        if (vikramX > 2.5f) { vikramX -= 0.02f; legAngle += legDirection*2; if(legAngle>40||legAngle<-40) legDirection*=-1; } else legAngle = 0; 
    }
    else if (elapsedTime < 465.0f) { scene = 10; }
    else { timerTicks = 0; sageX = 10; monkeyX = 8; vikramX = -10; vikramY = -2; betalAngle=180; }
    
    glutPostRedisplay(); glutTimerFunc(30, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == ' ' || key == 'n' || key == 'N') {
        if (scene == 0)      { timerTicks = 5.0f / 0.04f; sageX = 3.0f; }
        else if (scene == 1) { timerTicks = 50.0f / 0.04f; monkeyX = 1.0f; monkeyY = -2.0f; }
        else if (scene == 2) { timerTicks = 90.0f / 0.04f; }
        else if (scene == 3) { timerTicks = 135.0f / 0.04f; vikramX = 10.0f; }
        else if (scene == 4) { timerTicks = 185.0f / 0.04f; vikramX = 3.0f; vikramY = 1.0f; legAngle = 0.0f; }
        else if (scene == 5) { timerTicks = 230.0f / 0.04f; vikramY = -2.0f; }
        else if (scene == 6) { timerTicks = 280.0f / 0.04f; betalX = 4.0f; betalY = 3.0f; betalAngle = 180.0f; vikramX = -6.0f; }
        else if (scene == 7) { timerTicks = 340.0f / 0.04f; }
        else if (scene == 8) { timerTicks = 385.0f / 0.04f; vikramX = 2.5f; legAngle = 0.0f; }
        else if (scene == 9) { timerTicks = 430.0f / 0.04f; }
        else if (scene == 10){ timerTicks = 465.0f / 0.04f; }
    }
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluOrtho2D(-10.0, 10.0, -8.0, 8.0); glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
    glutInitWindowSize(winWidth, winHeight); glutInitWindowPosition(100, 100);
    glutCreateWindow("Vikram and Betal Native 2D Epic");
    init(); glutDisplayFunc(display); glutTimerFunc(0, timer, 0); 
    glutKeyboardFunc(keyboard); glutMainLoop(); return 0;
}
