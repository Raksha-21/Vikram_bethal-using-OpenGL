#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>

// --- Global Variables ---
// 'scene' controls which background and characters are currently being drawn.
int scene = 0; 
int timerTicks = 0;
// 'elapsedTime' is used to script the entire sequence of animations and scenes.
float elapsedTime = 0.0f; 

// Character positions and animation states
// We lowered the Y coordinates (e.g., -2.5f instead of -2.0f) so characters walk on the grass, not the sky.
float vikramX = -10.0f, vikramY = -2.5f, legAngle = 0.0f, legDirection = 1.0f;   
float betalX = 4.0f, betalY = 3.0f, betalAngle = 180.0f, swingDirection = 1.0f;
float sageX = 10.0f, sageY = -2.5f, sageArmAngle = 0.0f;
float monkeyX = 8.0f, monkeyY = 8.0f;

// Toggles the color of the jewel so it appears to be blinking.
bool jewelBlink = false;
int winWidth = 800, winHeight = 600;

// --- Basic 2D Drawing Helpers ---

// Function to draw a solid circle using polygons
void drawCircle(float cx, float cy, float r, int num_segments) {
    // glBegin(GL_POLYGON) starts drawing a filled polygon.
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++) {
        // Calculate the angle for each vertex around the circle
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        // glVertex2f specifies a point in 2D space. The math converts polar to cartesian coordinates.
        glVertex2f(r * cosf(theta) + cx, r * sinf(theta) + cy);
    }
    // glEnd() finishes the current drawing shape.
    glEnd();
}

// Function to draw basic black eyes
void drawEyes(float cx, float cy) {
    // glColor3f sets the current drawing color (Red, Green, Blue) from 0.0 to 1.0. (0,0,0) is Black.
    glColor3f(0.0f, 0.0f, 0.0f); 
    // glPointSize sets the size of the points.
    glPointSize(4.0f); 
    // GL_POINTS tells OpenGL to render individual dots.
    glBegin(GL_POINTS);
    glVertex2f(cx - 0.1f, cy + 0.1f); 
    glVertex2f(cx + 0.1f, cy + 0.1f); 
    glEnd();
}

// Function to draw the moon and clouds for the forest background
void drawMoonAndClouds() { 
    // Draw the Moon (light yellow)
    glColor3f(0.9f, 0.9f, 0.8f); 
    drawCircle(-5.0f, 5.0f, 1.2f, 50); 
    
    // Draw Clouds using overlapping circles
    glColor3f(0.3f, 0.3f, 0.4f); 
    drawCircle(-2.0f, 6.0f, 0.8f, 20); 
    drawCircle(-1.0f, 6.5f, 1.0f, 20); 
    drawCircle(0.0f, 6.0f, 0.8f, 20);
    drawCircle(4.0f, 5.0f, 0.6f, 20); 
    drawCircle(5.0f, 5.2f, 0.8f, 20); 
    drawCircle(6.0f, 5.0f, 0.6f, 20);
}

// Function to draw the night forest background
void drawForestBG() {
    // Sky color (dark blue). glRectf draws a filled rectangle.
    glColor3f(0.05f, 0.05f, 0.15f); 
    glRectf(-10.0f, -4.0f, 10.0f, 10.0f);
    
    // Grass color (dark green).
    glColor3f(0.1f, 0.3f, 0.1f); 
    glRectf(-10.0f, -8.0f, 10.0f, -4.0f);
}

// Function to draw the palace background using quads and lines
void drawPalaceBackground() {
    // Wall (light beige)
    glColor3f(0.9f, 0.8f, 0.6f);
    // GL_QUADS tells OpenGL to connect every 4 vertices into a rectangle (quadrilateral)
    glBegin(GL_QUADS); 
    glVertex2f(-10.0f, -7.0f); 
    glVertex2f(10.0f, -7.0f); 
    glVertex2f(10.0f, 7.0f); 
    glVertex2f(-10.0f, 7.0f); 
    glEnd();
    
    // Carpet on the floor (red)
    glColor3f(0.7f, 0.1f, 0.1f);
    glBegin(GL_QUADS); 
    glVertex2f(-10.0f, -7.0f); 
    glVertex2f(10.0f, -7.0f); 
    glVertex2f(10.0f, -3.0f); 
    glVertex2f(-10.0f, -3.0f); 
    glEnd();
    
    // Carpet gold borders
    glColor3f(1.0f, 0.8f, 0.2f);
    glLineWidth(8.0f); // Sets thickness of lines
    glBegin(GL_LINES); // Draws a line segment between every pairs of vertices
    glVertex2f(-5.0f, -7.0f); 
    glVertex2f(-5.0f, -3.0f); 
    glVertex2f(5.0f, -7.0f);  
    glVertex2f(5.0f, -3.0f); 
    glEnd();

    // Pillars (gold)
    glColor3f(0.8f, 0.6f, 0.2f);
    for (int i = -8; i <= 8; i += 8) {
        glBegin(GL_QUADS); 
        glVertex2f(i, -7.0f); 
        glVertex2f(i + 1, -7.0f); 
        glVertex2f(i + 1, 5.0f); 
        glVertex2f(i, 5.0f); 
        glEnd();
    }
}

// Function to draw the tree where Vetala hangs
void drawTree() {
    // Trunk (brown). Note: The base is changed to -5.5f so it firmly plants into the grass visually
    glColor3f(0.4f, 0.2f, 0.0f); 
    glRectf(3.5f, -5.5f, 4.5f, 2.0f);
    
    // Branches
    glLineWidth(8.0f); 
    glBegin(GL_LINES); 
    glVertex2f(4.0f, 1.0f); 
    glVertex2f(2.0f, 3.0f); 
    glVertex2f(4.0f, 1.5f); 
    glVertex2f(6.0f, 3.5f); 
    glEnd();
    
    // Leaves (dark green overlapping circles)
    glColor3f(0.0f, 0.5f, 0.0f); 
    drawCircle(4.0f, 3.0f, 2.0f, 20); 
    drawCircle(2.5f, 3.0f, 1.5f, 20); 
    drawCircle(5.5f, 3.0f, 1.5f, 20); 
    drawCircle(4.0f, 4.5f, 1.8f, 20);
}

// Function to draw the royal throne
void drawThrone(float x, float y) {
    // Wood base
    glColor3f(0.7f, 0.5f, 0.1f);
    glBegin(GL_QUADS); 
    glVertex2f(x - 1.0f, y - 2.0f); 
    glVertex2f(x + 1.0f, y - 2.0f); 
    glVertex2f(x + 1.0f, y); 
    glVertex2f(x - 1.0f, y); 
    glEnd();
    
    // Red cushion
    glColor3f(0.8f, 0.1f, 0.1f);
    glBegin(GL_QUADS); 
    glVertex2f(x - 0.8f, y - 2.0f); 
    glVertex2f(x + 0.8f, y - 2.0f); 
    glVertex2f(x + 0.8f, y - 1.5f); 
    glVertex2f(x - 0.8f, y - 1.5f); 
    glEnd();
}

// Function to draw the ritual fire using triangles
void drawFire(float x, float y, float scale) {
    // Outer flame (orange)
    glColor3f(1.0f, 0.3f, 0.0f); 
    // GL_TRIANGLES connects every 3 vertices to make a solid triangle
    glBegin(GL_TRIANGLES); 
    glVertex2f(x - scale, y); 
    glVertex2f(x + scale, y); 
    glVertex2f(x, y + scale * 2.5f); 
    glEnd();
    
    // Inner flame (yellow)
    glColor3f(1.0f, 0.8f, 0.0f); 
    glBegin(GL_TRIANGLES); 
    glVertex2f(x - scale / 2.0f, y); 
    glVertex2f(x + scale / 2.0f, y); 
    glVertex2f(x, y + scale * 1.5f); 
    glEnd();
}

// Function to draw King Vikramaditya
// Uses boolean flags to change his stance depending on the scene.
void drawKing(float x, float y, bool carryingBetal, bool onThrone, bool strike) {
    // glPushMatrix saves the current coordinate system matrix.
    glPushMatrix(); 
    // glTranslatef moves the origin to (x, y). All drawing below is relative to this new point.
    glTranslatef(x, y, 0.0f); 
    
    float legStartY = -0.8f;
    // King's body
    glColor3f(0.8f, 0.2f, 0.2f); 
    glRectf(-0.5f, legStartY, 0.5f, 1.2f); 
    
    // Face Details
    glColor3f(1.0f, 0.8f, 0.6f); 
    drawCircle(0.0f, 1.5f, 0.4f, 20); 
    drawEyes(0.0f, 1.6f); 
    
    // Moustache
    glColor3f(0.0f, 0.0f, 0.0f); 
    glLineWidth(2.0f); 
    glBegin(GL_LINES); 
    glVertex2f(-0.15f, 1.45f); 
    glVertex2f(0.15f, 1.45f); 
    glEnd();

    // Crown
    glColor3f(1.0f, 0.8f, 0.0f); 
    glBegin(GL_TRIANGLES); 
    glVertex2f(-0.4f, 1.8f); 
    glVertex2f(0.4f, 1.8f); 
    glVertex2f(0.0f, 2.6f); 
    glEnd(); 
    
    // Sword
    if (!carryingBetal && !onThrone) {
        glColor3f(0.8f, 0.8f, 0.8f); 
        glLineWidth(4.0f); 
        glBegin(GL_LINES);
        if (strike) { 
            // Swung down position
            glVertex2f(0.8f, 1.0f); 
            glVertex2f(2.0f, 0.0f); 
        } else { 
            // Sheathed or held straight position
            glVertex2f(0.8f, 0.0f); 
            glVertex2f(2.0f, 0.5f); 
        }
        glEnd();
    }
    
    // Arms
    glColor3f(1.0f, 0.8f, 0.6f); 
    glLineWidth(6.0f); 
    glBegin(GL_LINES);
    // Left arm
    glVertex2f(-0.5f, 1.0f); 
    glVertex2f(-0.8f, carryingBetal ? 1.5f : 0.0f);
    // Right arm
    glVertex2f(0.5f, 1.0f);  
    glVertex2f(0.8f, carryingBetal || strike ? 1.5f : 0.0f);
    glEnd();

    // Legs
    glColor3f(1.0f, 0.8f, 0.6f); 
    glBegin(GL_LINES);
    if (onThrone) {
        // Sitting pose
        glVertex2f(-0.3f, legStartY); glVertex2f(-0.3f, -1.5f); 
        glVertex2f(-0.3f, -1.5f); glVertex2f(-0.8f, -2.5f);
        glVertex2f(0.3f, legStartY); glVertex2f(0.3f, -1.5f); 
        glVertex2f(0.3f, -1.5f); glVertex2f(0.8f, -2.5f);
    } else {
        // Walking pose calculated using legAngle
        glVertex2f(-0.3f, legStartY); 
        glVertex2f(-0.3f - sinf(legAngle * 3.14f / 180.0f) * 0.8f, -2.0f); 
        
        glVertex2f(0.3f, legStartY); 
        glVertex2f(0.3f + sinf(legAngle * 3.14f / 180.0f) * 0.8f, -2.0f);
    }
    glEnd(); 
    
    // glPopMatrix restores the coordinate system that was saved before drawing the King.
    glPopMatrix();
}

// Function to draw Vetala
void drawVetala(float x, float y, float angle, bool carried) {
    glPushMatrix(); 
    glTranslatef(x, y, 0.0f); 
    // glRotatef rotates the coordinate system. Used when Vetala swings or hangs upside down.
    glRotatef(angle, 0.0f, 0.0f, 1.0f); 
    
    // Ghost Hair
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLES); 
    glVertex2f(0.0f, 1.3f); 
    glVertex2f(-0.6f, -0.6f); 
    glVertex2f(0.6f, -0.6f); 
    glEnd();

    // Body
    glColor3f(0.7f, 0.8f, 0.9f); 
    if (carried) {
        // Lifted body to avoiding blocking the King's face!
        glRectf(-0.3f, 0.1f, 0.3f, 0.6f); 
    } else {
        glRectf(-0.3f, -0.6f, 0.3f, 0.6f); 
    }

    // Head
    glColor3f(0.8f, 0.9f, 1.0f); 
    drawCircle(0.0f, 1.0f, 0.45f, 20); 
    drawEyes(0.0f, 1.0f); 

    // Arms
    glColor3f(0.7f, 0.8f, 0.9f); 
    glLineWidth(5.0f); 
    glBegin(GL_LINES);
    if (carried) {
        glVertex2f(-0.3f, 0.4f); glVertex2f(-0.5f, 0.0f);
        glVertex2f(0.3f, 0.4f);  glVertex2f(0.5f, 0.0f);
    } else {
        glVertex2f(-0.3f, 0.4f); glVertex2f(-0.8f, -0.2f); 
        glVertex2f(0.3f, 0.4f);  glVertex2f(0.8f, -0.2f);
    }
    glEnd();

    // Legs
    glBegin(GL_LINES);
    if (carried) {
        // Legs draping wide over the King's shoulders
        glVertex2f(-0.3f, 0.1f); glVertex2f(-0.6f, -1.8f);
        glVertex2f(0.3f, 0.1f);  glVertex2f(0.6f, -1.8f);
    } else {
        glVertex2f(-0.2f, -0.6f); glVertex2f(-0.4f, -1.5f); 
        glVertex2f(0.2f, -0.6f);  glVertex2f(0.4f, -1.5f);
    }
    glEnd(); 
    
    glPopMatrix();
}

// Function to draw the evil Sage
void drawSage(float x, float y, float angle = 0.0f) {
    glPushMatrix(); 
    glTranslatef(x, y, 0.0f); 
    glRotatef(angle, 0.0f, 0.0f, 1.0f); // Allows the sage to tilt/fall when attacked
    
    float shoulderY = 1.0f, legStartY = -0.8f;
    // Robe
    glColor3f(1.0f, 0.5f, 0.0f); 
    glRectf(-0.5f, legStartY, 0.5f, 1.2f); 
    
    // Head and eyes
    glColor3f(1.0f, 0.8f, 0.6f); 
    drawCircle(0.0f, 1.5f, 0.4f, 20); 
    drawEyes(0.0f, 1.5f); 
    
    // Beard
    glColor3f(0.8f, 0.8f, 0.8f); 
    drawCircle(0.0f, 1.1f, 0.35f, 15); 
    
    // Arms (animated using sageArmAngle)
    glColor3f(1.0f, 0.8f, 0.6f); 
    glLineWidth(6.0f); 
    glBegin(GL_LINES);
    glVertex2f(-0.5f, shoulderY); 
    glVertex2f(-0.8f, 0.0f + sinf(sageArmAngle) * 0.5f);
    glVertex2f(0.5f, shoulderY);  
    glVertex2f(0.8f, 0.0f - sinf(sageArmAngle) * 0.5f); 
    glEnd();
    
    // Legs
    glColor3f(1.0f, 0.8f, 0.6f); 
    glBegin(GL_LINES);
    glVertex2f(-0.3f, legStartY); 
    glVertex2f(-0.3f, -2.0f); 
    glVertex2f(0.3f, legStartY);  
    glVertex2f(0.3f, -2.0f); 
    glEnd();
    
    glPopMatrix();
}

// Function to draw Palace Soldiers
void drawSoldier(float x, float y) {
    // Body Armor
    glColor3f(0.2f, 0.3f, 0.8f);
    glBegin(GL_QUADS); 
    glVertex2f(x - 0.3f, y); 
    glVertex2f(x + 0.3f, y); 
    glVertex2f(x + 0.3f, y - 1.5f); 
    glVertex2f(x - 0.3f, y - 1.5f); 
    glEnd();
    
    // Head
    glColor3f(1.0f, 0.8f, 0.6f); 
    drawCircle(x, y + 0.5f, 0.3f, 20);
    
    // Helmet
    glColor3f(0.5f, 0.5f, 0.5f); 
    drawCircle(x, y + 0.6f, 0.3f, 20);
    
    // Long Spear Tool
    glColor3f(0.6f, 0.6f, 0.6f); 
    glLineWidth(3.0f); 
    glBegin(GL_LINES); 
    glVertex2f(x + 0.5f, y - 2.0f); 
    glVertex2f(x + 0.5f, y + 2.0f); 
    glEnd();
}

// Function to draw the Monkey
void drawMonkey(float x, float y) {
    // Body & Head
    glColor3f(0.4f, 0.2f, 0.0f); 
    drawCircle(x, y, 0.4f, 15); 
    drawCircle(x, y + 0.4f, 0.3f, 15); 
    
    // Tail / arm outline
    glLineWidth(4.0f); 
    glBegin(GL_LINES); 
    glVertex2f(x - 0.4f, y); 
    glVertex2f(x - 0.8f, y + 0.4f); 
    glEnd();
    
    // The glowing magical Jewel held by the monkey
    if (jewelBlink) { 
        glColor3f(0.0f, 1.0f, 1.0f); 
        drawCircle(x - 0.8f, y + 0.4f, 0.2f, 8); 
    } 
}

// Function to draw a speech bubble over a character
void drawSpeechBubble() {
    // Bubble box
    glColor3f(0.9f, 0.9f, 0.9f); 
    glRectf(-3.0f, 3.0f, 3.0f, 6.0f);
    
    // Triangle pointing down toward the speaker
    glBegin(GL_TRIANGLES); 
    glVertex2f(1.0f, 3.0f); 
    glVertex2f(2.0f, 3.0f); 
    glVertex2f(0.0f, 1.0f); 
    glEnd();
    
    // Decorative lines inside the bubble representing text
    glColor3f(0.0f, 0.0f, 0.0f); 
    glLineWidth(3.0f); 
    glBegin(GL_LINES); 
    glVertex2f(-2.0f, 5.0f); 
    glVertex2f(2.0f, 5.0f); 
    glVertex2f(-2.0f, 4.0f); 
    glVertex2f(2.0f, 4.0f); 
    glEnd();
}

// Helper to draw small on-screen text (like subtitles)
void drawText(float x, float y, const char* str) {
    glColor3f(1.0f, 1.0f, 0.0f); 
    // glRasterPos2f sets the starting cursor position for drawing text
    glRasterPos2f(x, y);
    // glutBitmapCharacter renders a single character. We loop through the string to write sentences.
    while (*str) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str++);
    }
}

// Helper to draw large text for the title screen
void drawTitleText(float x, float y, const char* str) {
    glColor3f(1.0f, 0.8f, 0.0f); 
    glRasterPos2f(x, y);
    while (*str) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *str++);
    }
}

// --- Main Display Function ---
// This function is automatically called by OpenGL to render every frame.
void display() {
    // Clear the pixel buffer to a blank slate for drawing
    glClear(GL_COLOR_BUFFER_BIT); 
    
    // Switch between scenes
    if (scene == 0) {
        // --- Scene 0: Title Screen ---
        glColor3f(0.0f, 0.0f, 0.0f); // Back background
        glRectf(-10.0f, -8.0f, 10.0f, 10.0f);
        drawTitleText(-3.5f, 0.0f, "VIKRAM AND BETAL");
        drawTitleText(-2.0f, -1.0f, "A Graphics Project");
    } else {
        // --- Render Backgrounds ---
        // Scenes 1-3, and 10 are in the palace. The rest are in the haunted forest.
        if (scene <= 3 || scene == 10) { 
            drawPalaceBackground(); 
        } else { 
            drawForestBG(); 
            drawMoonAndClouds(); 
        }
        
        // --- Render Characters and Props Per Scene ---
        if (scene == 1) { 
            drawThrone(-5.0f, -0.5f); 
            drawKing(-5.0f, -0.5f, false, true, false); 
            drawSoldier(-8.0f, -1.0f); 
            drawSoldier(-2.0f, -1.0f);
            drawSage(sageX, sageY);
        }
        else if (scene == 2) { 
            drawThrone(-5.0f, -0.5f); 
            drawKing(-5.0f, -0.5f, false, true, false); 
            drawSage(sageX, sageY); 
            drawMonkey(monkeyX, monkeyY);
        }
        else if (scene == 3) { 
            drawThrone(-5.0f, -0.5f); 
            drawKing(-5.0f, -0.5f, false, true, false); 
            drawSage(sageX, sageY); 
            drawSpeechBubble();
        }
        else if (scene == 4) { 
            // King sets out into the forest
            drawKing(vikramX, vikramY, false, false, false); 
        }
        else if (scene == 5) { 
            drawTree(); 
            drawVetala(4.0f, 3.0f, betalAngle, false); 
            drawKing(vikramX, vikramY, false, false, false); 
        }
        else if (scene == 6) { 
            drawTree(); 
            drawKing(vikramX, vikramY, true, false, false); 
            drawVetala(vikramX, vikramY + 2.0f, 0.0f, true); 
        }
        else if (scene == 7) { 
            drawTree(); 
            drawKing(vikramX, vikramY, true, false, false);
            // Show a speech bubble when Vetala is speaking
            if (elapsedTime > 285 && elapsedTime < 315) {
                drawSpeechBubble();
            }
            drawVetala(betalX, betalY, betalAngle, (elapsedTime <= 315.0f));
        }
        else if (scene == 8) { 
            drawTree(); 
            drawKing(vikramX, vikramY, false, false, false); 
            drawVetala(betalX, betalY, betalAngle, false);
            if (elapsedTime > 345 && elapsedTime < 375) { 
                glColor3f(1.0f, 0.0f, 0.0f); 
                drawSpeechBubble(); 
            } 
        }
        else if (scene == 9) { 
            drawFire(-2.0f, -4.0f, 1.5f); 
            drawSage(sageX, sageY, 45.0f); 
            drawKing(vikramX, vikramY, false, false, true); 
        }
        else if (scene == 10) { 
            drawThrone(-5.0f, -0.5f); 
            drawKing(-5.0f, -0.5f, false, true, false); 
            drawSoldier(-8.0f, -1.0f); 
            drawSoldier(-2.0f, -1.0f);
        }
        
        // --- Subtitles Strip at the bottom of the screen ---
        glColor3f(0.0f, 0.0f, 0.0f); 
        glRectf(-10.0f, -8.0f, 10.0f, -5.5f); // Black bar for text clarity
        
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
        
        // Show skip helper text at bottom right
        glColor3f(0.5f, 0.5f, 0.5f); 
        glRasterPos2f(2.0f, -7.5f);
        const char* help = "Tip: Press 'SPACE' to skip to the next scene";
        while (*help) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *help++);
        }
    }
    
    // Swap front and back buffers to display the rendered image (Double Buffering smooths animations)
    glutSwapBuffers(); 
}

// --- Animation Update Logic (Called 30 times a second) ---
void timer(int value) {
    timerTicks++; 
    elapsedTime = timerTicks * 0.04f;
    
    // Toggle the jewel blink state
    jewelBlink = (timerTicks % 10 < 5);

    // Scene progression logic (hardcoded timeline based on elapsedTime)
    if (elapsedTime < 5.0f) { 
        scene = 0; 
    }
    else if (elapsedTime < 50.0f) { 
        scene = 1; 
        if (sageX > 3.0f) sageX -= 0.05f; // Sage walks in
        sageArmAngle += 0.1f; // Wave hand
    }
    else if (elapsedTime < 90.0f) { 
        scene = 2; 
        if (monkeyX > 1.0f) { 
            monkeyX -= 0.05f; 
            monkeyY = -1.0f + sinf(elapsedTime * 5.0f) * 2.0f; // Monkey jumping math
        } else {
            monkeyY = -2.0f; 
        } 
    }
    else if (elapsedTime < 135.0f) { 
        scene = 3; 
        sageArmAngle += 0.2f; 
    }
    else if (elapsedTime < 185.0f) { 
        scene = 4; 
        // Reset position at the start of scene 4
        if (elapsedTime < 136.0f) { vikramX = -10.0f; vikramY = -2.5f; } 
        // Walk right
        if (vikramX < 10.0f) { 
            vikramX += 0.03f; 
            legAngle += legDirection * 2.0f; 
            if(legAngle > 40.0f || legAngle < -40.0f) legDirection *= -1; 
        }
    }
    else if (elapsedTime < 230.0f) { 
        scene = 5; 
        if (elapsedTime < 186.0f) { 
            vikramX = -10.0f; vikramY = -2.5f; betalX = 4.0f; betalY = 3.0f; betalAngle = 180.0f; 
        }
        
        // Logic for Walking and then Climbing the tree
        if (vikramX < 3.8f) { 
            // Still walking to tree
            vikramX += 0.03f; 
            legAngle += legDirection * 2.0f; 
            if(legAngle > 40.0f || legAngle < -40.0f) legDirection *= -1; 
        }
        else if (vikramY < 1.0f) { 
            // Starts climbing the trunk
            vikramY += 0.02f; 
            legAngle += legDirection * 3.0f; 
            if(legAngle > 35.0f || legAngle < -35.0f) legDirection *= -1; 
        }
        else { 
            // Reached branch
            legAngle = 0.0f; 
        }
    }
    else if (elapsedTime < 280.0f) { 
        scene = 6; 
        vikramY = -2.5f; legAngle = 0.0f; betalAngle = 0.0f; 
        betalX = vikramX; betalY = vikramY + 2.0f; 
    }
    else if (elapsedTime < 340.0f) { 
        scene = 7;
        if (elapsedTime < 285.0f) { 
            // Vikram carrying Betal starts walking back
            if (vikramX > -6.0f) { 
                vikramX -= 0.03f; 
                legAngle += legDirection * 2.0f; 
                if(legAngle > 40.0f || legAngle < -40.0f) legDirection *= -1; 
            }
            betalX = vikramX; 
            betalY = vikramY + 2.0f; 
            betalAngle = 0.0f;
        } else if (elapsedTime > 315.0f) { 
            // Betal flies back to tree
            betalX += (4.0f - betalX) * 0.05f; 
            betalY += (3.0f - betalY) * 0.05f; 
            betalAngle += (180.0f - betalAngle) * 0.05f;
        } else { 
            // Stood still while answering riddle
            legAngle = 0.0f; 
        } 
    }
    else if (elapsedTime < 385.0f) { 
        scene = 8; 
        if (elapsedTime < 341.0f) { 
            vikramX = 2.0f; betalX = 4.0f; betalY = 3.0f; betalAngle = 180.0f; 
        } 
    }
    else if (elapsedTime < 430.0f) { 
        scene = 9; 
        if (elapsedTime < 386.0f) { 
            sageX = 2.0f; sageY = -3.0f; vikramX = 5.0f; vikramY = -2.5f; legAngle = 0.0f; 
        }
        // Vikram sneaks up
        if (vikramX > 2.5f) { 
            vikramX -= 0.02f; 
            legAngle += legDirection * 2.0f; 
            if(legAngle > 40.0f || legAngle < -40.0f) legDirection *= -1; 
        } else {
            legAngle = 0.0f; 
        }
    }
    else if (elapsedTime < 465.0f) { 
        scene = 10; 
    }
    else { 
        // Restart loop
        timerTicks = 0; sageX = 10.0f; monkeyX = 8.0f; vikramX = -10.0f; vikramY = -2.5f; betalAngle = 180.0f; 
    }
    
    // glutPostRedisplay tells OpenGL that scene data changed, so it needs to draw again immediately.
    glutPostRedisplay(); 
    // Wait ~30 milliseconds, then run the timer function again for the next frame.
    glutTimerFunc(30, timer, 0);
}

// Function to handle keyboard input (Spaceskip mechanism)
void keyboard(unsigned char key, int x, int y) {
    if (key == ' ' || key == 'n' || key == 'N') {
        // Skips animations / skips to the end of scenes
        if (scene == 0)      { timerTicks = 5.0f / 0.04f; sageX = 3.0f; }
        else if (scene == 1) { timerTicks = 50.0f / 0.04f; monkeyX = 1.0f; monkeyY = -2.0f; }
        else if (scene == 2) { timerTicks = 90.0f / 0.04f; }
        else if (scene == 3) { timerTicks = 135.0f / 0.04f; vikramX = 10.0f; }
        else if (scene == 4) { timerTicks = 185.0f / 0.04f; vikramX = 3.8f; vikramY = 1.0f; legAngle = 0.0f; }
        else if (scene == 5) { timerTicks = 230.0f / 0.04f; vikramX = 3.8f; vikramY = -2.5f; }
        else if (scene == 6) { timerTicks = 280.0f / 0.04f; betalX = 4.0f; betalY = 3.0f; betalAngle = 180.0f; vikramX = -6.0f; }
        else if (scene == 7) { timerTicks = 340.0f / 0.04f; }
        else if (scene == 8) { timerTicks = 385.0f / 0.04f; vikramX = 2.5f; legAngle = 0.0f; }
        else if (scene == 9) { timerTicks = 430.0f / 0.04f; }
        else if (scene == 10){ timerTicks = 465.0f / 0.04f; }
    }
}

// Function to initialize OpenGL configuration
void init() {
    // Set default background color to pitch black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    // GL_PROJECTION sets up our camera / viewing volume details
    glMatrixMode(GL_PROJECTION); 
    // glLoadIdentity resets the matrix 
    glLoadIdentity();
    
    // gluOrtho2D defines our 2D coordinate space mapping!
    // X goes from -10 to +10, Y goes from -8 to +8. Screen resolution maps to these boundaries.
    gluOrtho2D(-10.0, 10.0, -8.0, 8.0); 
    
    // GL_MODELVIEW switches back to the mode for transforming and drawing objects (shapes)
    glMatrixMode(GL_MODELVIEW);
}

// Application Entry Point
int main(int argc, char** argv) {
    // Initialize the GLUT (Graphics Library Utility Toolkit)
    glutInit(&argc, argv); 
    // Set display mode. DOUBLE buffering eliminates screen tearing during animation.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
    
    // Set window dimensions and starting screen position
    glutInitWindowSize(winWidth, winHeight); 
    glutInitWindowPosition(100, 100);
    // Creates the desktop window and title
    glutCreateWindow("Vikram and Betal Native 2D Epic");
    
    // Call our initialization logic
    init(); 
    
    // Attach our callback functions
    glutDisplayFunc(display); // Tells glut to use our 'display' func to draw graphics
    glutTimerFunc(0, timer, 0); // Start our animation timer looping
    glutKeyboardFunc(keyboard); // Catch keystrokes for skipping scenes
    
    // Wait and process events infinitely until the user closes the window.
    glutMainLoop(); 
    return 0;
}
