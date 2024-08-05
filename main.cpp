
//
//  main.cpp
//  Tetris
//
//  Created by Gary Chu on 31/12/2017.
//  Copyright © 2017 Gary Chu. All rights reserved.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//#include "./actualgame.h"

#include <stddef.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#define NumberX 20
#define NumberY 20
#define Speed 3

int BoardX, BoardY;
int numberRotation;
int checkBoard[10][20];
int originX = 3;
int originY = 20;
int numnum;
bool downornot = true;
bool falling = false;
bool gameover = false;
int score = 0;
int leftmost, rightmost;
int primaryindex, secondaryindex;
int transactiontype = 1;
int count = 0;
int current, next = 0;
int prelevel, level = 1;

void draw_text(const char* text)
{
    size_t len = strlen(text);
    for (size_t i=0;i<len;i++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
}

void checkgameover() {
    if (originY == 20) {
        gameover = true;
        printf("game over\n");
        printf("your final score is %i\n", score);
    }
}

void draw3sqrhori() {
    glBegin(GL_POLYGON);
    glVertex2f(originX-1.0f, originY);
    glVertex2f(originX-1.0f, originY+1.0f);
    glVertex2f(originX+2.0f, originY+1.0f);
    glVertex2f(originX+2.0f, originY);
    glEnd();
}

void draw3sqrvert() {
    glBegin(GL_POLYGON);
    glVertex2f(originX, originY+2.0f);
    glVertex2f(originX+1.0f, originY+2.0f);
    glVertex2f(originX+1.0f, originY-1.0f);
    glVertex2f(originX, originY-1.0f);
    glEnd();
}

void drawtargetsqr(int targetX, int targetY) {
    glBegin(GL_POLYGON);
    glVertex2f(targetX, targetY);
    glVertex2f(targetX, targetY+1.0f);
    glVertex2f(targetX+1.0f, targetY+1.0f);
    glVertex2f(targetX+1.0f, targetY);
    glEnd();
}

void draw2sqrhori(int targetX, int targetY) {
    glBegin(GL_POLYGON);
    glVertex2f(targetX, targetY);
    glVertex2f(targetX, targetY+1.0f);
    glVertex2f(targetX+2.0f, targetY+1.0f);
    glVertex2f(targetX+2.0f, targetY);
    glEnd();
}

void draw2sqrvert(int targetX, int targetY) {
    glBegin(GL_POLYGON);
    glVertex2f(targetX, targetY);
    glVertex2f(targetX, targetY+2.0f);
    glVertex2f(targetX+1.0f, targetY+2.0f);
    glVertex2f(targetX+1.0f, targetY);
    glEnd();
}

void initialBoard(int x, int y) {
    BoardX = x;
    BoardY = y;
}

void paintSquare(int color, int x, int y) {
    switch (color) {
        case 1: glColor3f(0.5f, 0.0f, 0.5f); break;
        case 2: glColor3f(0.0f, 0.0f, 1.0f); break;
        case 3: glColor3f(0.0f, 1.0f, 0.0f); break;
        case 4: glColor3f(1.0f, 0.0f, 1.0f); break;
        case 5: glColor3f(0.0f, 1.0f, 1.0f); break;
        case 6: glColor3f(1.0f, 0.5f, 1.0f); break;
        case 7: glColor3f(1.0f, 1.0f, 0.0f); break;
        default: glColor3f(1.0f, 1.0f, 1.0f); break;
    }
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x, y+1);
    glVertex2f(x+1, y+1);
    glVertex2f(x+1, y);
    glEnd();
}

void setBoard() {
    for (int x = 0; x < 10; x++) {
        for(int y = 0; y < 20; y++) {
            paintSquare(checkBoard[x][y], x, y);
        }
    }
}

int randomNum() {
    srand(time(NULL));
    return rand() % (7 + 1 - 1) + 1;
    //return 7;
}

void drawRec(int color) {
    transactiontype = 2;
    glColor3f(0.5f, 0.0f, 0.5f);
    if (numberRotation == 0) {
        draw3sqrhori();
        drawtargetsqr(originX+2, originY);
        leftmost = originX-1;
        rightmost = originX+3;
        if (checkBoard[originX-1][originY-1] != 0 || checkBoard[originX][originY-1] != 0 || checkBoard[originX+1][originY-1] != 0 || checkBoard[originX+2][originY-1] != 0 || originY == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX-1][originY] = color;
            checkBoard[originX][originY] = color;
            checkBoard[originX+1][originY] = color;
            checkBoard[originX+2][originY] = color;
        }
    } else {
        draw3sqrvert();
        drawtargetsqr(originX, originY-2);
        leftmost = originX;
        rightmost= originX+1;
        if (checkBoard[originX][originY-3] != 0 || originY-2 == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY+1] = color;
            checkBoard[originX][originY] = color;
            checkBoard[originX][originY-1] = color;
            checkBoard[originX][originY-2] = color;
        }
    }
    
    glColor3f(0.5f, 0.5f, 0.5f);
    
    drawtargetsqr(originX, originY);
}

void drawSqr(int color) {
    transactiontype = 1;
    leftmost = originX;
    rightmost = originX+2;
    glColor3f(0.0f, 0.0f, 1.0f);
    draw2sqrhori(originX, originY);
    draw2sqrhori(originX, originY+1);
    
    glColor3f(0.5f, 0.5f, 0.5f);
    
    drawtargetsqr(originX, originY);
    
    if (checkBoard[originX][originY-1] != 0 || checkBoard[originX+1][originY-1] != 0 || originY == 0) {
        checkgameover();
        falling = false;
        checkBoard[originX][originY] = color;
        checkBoard[originX][originY+1] = color;
        checkBoard[originX+1][originY+1] = color;
        checkBoard[originX+1][originY] = color;
    }
}

void drawL(int color) {
    transactiontype = 4;
    glColor3f(0.0f, 1.0f, 0.0f);
    if (numberRotation == 0) {
        draw3sqrhori();
        
        drawtargetsqr(originX-1, originY+1);
        leftmost = originX-1;
        rightmost = originX+2;
        if (checkBoard[originX-1][originY-1] != 0 || checkBoard[originX][originY-1] != 0 || checkBoard[originX+1][originY-1] != 0 || originY == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX-1][originY+1] = color;
            checkBoard[originX-1][originY] = color;
            checkBoard[originX+1][originY] = color;
        }
    } else if (numberRotation == 3) {
        draw3sqrvert();
        drawtargetsqr(originX-1, originY-1);
        leftmost = originX-1;
        rightmost = originX+1;
        if (checkBoard[originX][originY-2] != 0 || checkBoard[originX-1][originY-2] != 0 || originY-1 == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX-1][originY-1] = color;
            checkBoard[originX][originY+1] = color;
            checkBoard[originX][originY-1] = color;
        }
    } else if (numberRotation == 2) {
        draw3sqrhori();
        drawtargetsqr(originX+1, originY-1);
        leftmost = originX-1;
        rightmost = originX+2;
        if (checkBoard[originX-1][originY-1] != 0 || checkBoard[originX][originY-1] != 0 || checkBoard[originX+1][originY-2] != 0 || originY-1 == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX-1][originY] = color;
            checkBoard[originX+1][originY] = color;
            checkBoard[originX+1][originY-1] = color;
        }
    } else {
        draw3sqrvert();
        drawtargetsqr(originX+1, originY+1);
        leftmost = originX;
        rightmost = originX+2;
        if (checkBoard[originX][originY-2] != 0 || checkBoard[originX+1][originY] != 0 || originY-1 == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX][originY-1] = color;
            checkBoard[originX][originY+1] = color;
            checkBoard[originX+1][originY+1] = color;
        }
    }
    
    glColor3f(0.5f, 0.5f, 0.5f);
    
    drawtargetsqr(originX, originY);
}

void drawLR(int color) {
    transactiontype = 4;
    glColor3f(1.0f, 0.0f, 1.0f);
    if (numberRotation == 0) {
        draw3sqrhori();
        
        drawtargetsqr(originX+1, originY+1);
        leftmost = originX-1;
        rightmost = originX+2;
        if (checkBoard[originX-1][originY-1] != 0 || checkBoard[originX][originY-1] != 0 || checkBoard[originX+1][originY-1] != 0 || originY == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX-1][originY] = color;
            checkBoard[originX+1][originY] = color;
            checkBoard[originX+1][originY+1] = color;
        }
    } else if (numberRotation == 3) {
        draw3sqrvert();
        
        drawtargetsqr(originX-1, originY+1);
        leftmost = originX-1;
        rightmost = originX+1;
        if (checkBoard[originX][originY-2] != 0 || checkBoard[originX-1][originY] != 0 || originY-1 == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX][originY+1] = color;
            checkBoard[originX][originY-1] = color;
            checkBoard[originX-1][originY+1] = color;
        }
    } else if (numberRotation == 2) {
        draw3sqrhori();
        
        drawtargetsqr(originX-1, originY-1);
        leftmost = originX-1;
        rightmost = originX+2;
        if (checkBoard[originX][originY-1] != 0 || checkBoard[originX+1][originY-1] != 0 || checkBoard[originX-1][originY-2] != 0 || originY-1 == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX-1][originY] = color;
            checkBoard[originX+1][originY] = color;
            checkBoard[originX-1][originY-1] = color;
        }
    } else {
        draw3sqrvert();
        
        drawtargetsqr(originX+1, originY-1);
        leftmost = originX;
        rightmost = originX+2;
        if (checkBoard[originX][originY-2] != 0 || checkBoard[originX+1][originY-2] != 0 || originY-1 == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX][originY-1] = color;
            checkBoard[originX][originY+1] = color;
            checkBoard[originX+1][originY-1] = color;
        }
    }
    
    glColor3f(0.5f, 0.5f, 0.5f);
    
    drawtargetsqr(originX, originY);
}

void drawZ(int color) {
    transactiontype = 2;
    glColor3f(0.0f, 1.0f, 1.0f);
    if (numberRotation == 0) {
        draw2sqrhori(originX-1, originY);
        draw2sqrhori(originX, originY-1);
        leftmost = originX-1;
        rightmost = originX+2;
        if (checkBoard[originX-1][originY-1] != 0 || checkBoard[originX][originY-2] != 0 || checkBoard[originX+1][originY-2] != 0 || originY-1 == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX-1][originY] = color;
            checkBoard[originX][originY-1] = color;
            checkBoard[originX+1][originY-1] = color;
        }
    } else {
        draw2sqrvert(originX, originY);
        draw2sqrvert(originX-1, originY-1);
        leftmost = originX-1;
        rightmost=  originX+1;
        if (checkBoard[originX][originY-1] != 0 || checkBoard[originX-1][originY-2] != 0 || originY-1 == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX][originY+1] = color;
            checkBoard[originX-1][originY] = color;
            checkBoard[originX-1][originY-1] = color;
        }
    }
    glColor3f(0.5f, 0.5f, 0.5f);
    drawtargetsqr(originX, originY);
}

void drawZR(int color) {
    transactiontype = 2;
    glColor3f(1.0f, 0.5f, 1.0f);
    if (numberRotation == 0) {
        draw2sqrhori(originX, originY);
        draw2sqrhori(originX-1, originY-1);
        leftmost = originX-1;
        rightmost = originX+2;
        if (checkBoard[originX-1][originY-2] != 0 || checkBoard[originX][originY-2] != 0 || checkBoard[originX+1][originY-1] != 0 || originY-1 == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX-1][originY-1] = color;
            checkBoard[originX][originY-1] = color;
            checkBoard[originX+1][originY] = color;
        }
    } else {
        draw2sqrvert(originX-1, originY);
        draw2sqrvert(originX, originY-1);
        leftmost = originX-1;
        rightmost=  originX+1;
        if (checkBoard[originX-1][originY-1] != 0 || checkBoard[originX][originY-2] != 0 || originY-1 == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX][originY-1] = color;
            checkBoard[originX-1][originY] = color;
            checkBoard[originX-1][originY+1] = color;
        }
    }
    glColor3f(0.5f, 0.5f, 0.5f);
    drawtargetsqr(originX, originY);
}

void drawT(int color) {
    transactiontype = 4;
    glColor3f(1.0f, 1.0f, 0.0f);
    if (numberRotation == 0) {
        draw3sqrhori();
        
        drawtargetsqr(originX, originY+1);
        leftmost = originX-1;
        rightmost = originX+2;
        if (checkBoard[originX-1][originY-1] != 0 || checkBoard[originX][originY-1] != 0 || checkBoard[originX+1][originY-1] != 0 || originY == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX-1][originY] = color;
            checkBoard[originX+1][originY] = color;
            checkBoard[originX][originY+1] = color;
        }
    } else if (numberRotation == 3) {
        draw3sqrvert();
        
        drawtargetsqr(originX-1, originY);
        leftmost = originX-1;
        rightmost = originX+1;
        if (checkBoard[originX-1][originY-1] != 0 || checkBoard[originX][originY-2] != 0 || originY-1 == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX-1][originY] = color;
            checkBoard[originX][originY+1] = color;
            checkBoard[originX][originY-1] = color;
        }
    } else if (numberRotation == 2) {
        draw3sqrhori();
        
        drawtargetsqr(originX, originY-1);
        leftmost = originX-1;
        rightmost = originX+2;
        if (checkBoard[originX-1][originY-1] != 0 || checkBoard[originX][originY-2] != 0 || checkBoard[originX+1][originY-1] != 0 || originY-1 == 0) {
            checkgameover();
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX][originY-1] = color;
            checkBoard[originX-1][originY] = color;
            checkBoard[originX+1][originY] = color;
        }
    } else {
        draw3sqrvert();
        
        drawtargetsqr(originX+1, originY);
        leftmost = originX;
        rightmost = originX+2;
        if (checkBoard[originX][originY-2] != 0 || checkBoard[originX+1][originY-1] != 0 || originY-1 == 0) {
            falling = false;
            checkBoard[originX][originY] = color;
            checkBoard[originX][originY-1] = color;
            checkBoard[originX][originY+1] = color;
            checkBoard[originX+1][originY] = color;
        }
    }
    
    glColor3f(0.5f, 0.5f, 0.5f);
    drawtargetsqr(originX, originY);
}

bool checkLine() {
    for (int y = 0; y < 20; y++) {
        bool linefull = false;
        for (int x = 0; x < 10; x++) {
            if (checkBoard[x][y] == 0) {
                break;
            } else if (checkBoard[x][y] != 0 && x == 9) {
                linefull = true;
            }
        }
        if (linefull) {
            for (int v = y; v < 20; v++) {
                for (int u = 0; u < 10; u++) {
                    if (v != 19) {
                        checkBoard[u][v] = checkBoard[u][v+1];
                    } else {
                        checkBoard[u][v] = 0;
                    }
                }
            }
            score++;
            setBoard();
            return true;
        } else {continue;}
    }
    return false;
}

void reset() {
    originX = 4;
    originY = 20;
    falling = true;
}

void display() {
    char buffer[20];
    char sbuffer[20];
    snprintf(buffer, sizeof(buffer), "%d", score);
    snprintf(sbuffer, sizeof(sbuffer), "%d", level);
    if (gameover == false) {
        prelevel = level*5;
        if (score > prelevel)
            level++;
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0.8f, 0.8f, 0.8f);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(12.0f, 10.0f, 0.0f); // this will work
        glScalef(0.01f, 0.01f, 0.01f); // this will work
        draw_text("Your Score");
        glPopMatrix();
        glPushMatrix();
        glTranslatef(15.0f, 8.0f, 0.0f); // this will work
        glScalef(0.01f, 0.01f, 0.01f); // this will work
        draw_text(buffer);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(11.0f, 5.0f, 0.0f); // this will work
        glScalef(0.01f, 0.01f, 0.01f); // this will work
        draw_text("Current Level");
        glPopMatrix();
        glPushMatrix();
        glTranslatef(15.0f, 3.0f, 0.0f); // this will work
        glScalef(0.01f, 0.01f, 0.01f); // this will work
        draw_text(sbuffer);
        glPopMatrix();
        setBoard();
        if (falling == false) {
            bool check = true;
            if (count == 0) {
                current = randomNum();
                next = randomNum();
                count = 1;
                glEnd();
            } else {
                current = next;
                next = randomNum();
            }
            numberRotation = 0;
            reset();
            while(check) {
                check = checkLine();
            }
        } else {
            switch (current) {
                case 1: drawRec(1); break;
                case 2: drawSqr(2); break;
                case 3: drawL(3); break;
                case 4: drawLR(4); break;
                case 5: drawZ(5); break;
                case 6: drawZR(6); break;
                case 7: drawT(7); break;
            }
            if (leftmost < 0) {
                originX-=leftmost;
            } else if (rightmost > 10) {
                originX-=(rightmost-10);
            }
            if (falling == true) {
                if (downornot == false) {
                    downornot = true;
                } else {
                    originY--;
                }
            } else {
                
            }
        }
        glColor3f(1.0f, 1.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(12.0f, 18.0f, 0.0f); // this will work
        glScalef(0.01f, 0.01f, 0.01f); // this will work
        draw_text("Next Piece");
        glPopMatrix();
        glBegin(GL_POLYGON);
        glBegin(GL_POLYGON);
        glVertex2f(13.0f, 13.0f);
        glVertex2f(13.0f, 17.0f);
        glVertex2f(17.0f, 17.0f);
        glVertex2f(17.0f, 13.0f);
        glEnd();
        switch (next) {
            case 1: glColor3f(0.5f, 0.0f, 0.5f); drawtargetsqr(13.0f, 14.75f); drawtargetsqr(14.0f, 14.75f); drawtargetsqr(15.0f, 14.75f); drawtargetsqr(16.0f, 14.75f); break;
            case 2: glColor3f(0.0f, 0.0f, 1.0f); drawtargetsqr(15.0f, 15.0f); drawtargetsqr(14.0f, 15.0f); drawtargetsqr(14.0f, 14.0f); drawtargetsqr(15.0f, 14.0f); break;
            case 3: glColor3f(0.0f, 1.0f, 0.0f); drawtargetsqr(13.75f, 15.0f); drawtargetsqr(13.75f, 14.0f); drawtargetsqr(14.75f, 14.0f); drawtargetsqr(15.75f, 14.0f); break;
            case 4: glColor3f(1.0f, 0.0f, 1.0f); drawtargetsqr(15.75f, 15.0f); drawtargetsqr(13.75f, 14.0f); drawtargetsqr(14.75f, 14.0f); drawtargetsqr(15.75f, 14.0f); break;
            case 5: glColor3f(0.0f, 1.0f, 1.0f); drawtargetsqr(13.75f, 15.0f); drawtargetsqr(14.75f, 15.0f); drawtargetsqr(14.75f, 14.0f); drawtargetsqr(15.75f, 14.0f); break;
            case 6: glColor3f(1.0f, 0.5f, 1.0f); drawtargetsqr(13.75f, 14.0f); drawtargetsqr(14.75f, 14.0f); drawtargetsqr(14.75f, 15.0f); drawtargetsqr(15.75f, 15.0f); break;
            case 7: glColor3f(1.0f, 1.0f, 0.0f); drawtargetsqr(14.75f, 15.0f); drawtargetsqr(13.75f, 14.0f); drawtargetsqr(14.75f, 14.0f); drawtargetsqr(15.75f, 14.0f); break;
        }
        glutSwapBuffers();
    } else {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0.8f, 0.8f, 0.8f);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(1.0f, 15.0f, 0.0f); // this will work
        glScalef(0.01f, 0.01f, 0.01f); // this will work
        draw_text("Game Over! Your Score is: ");
        glPopMatrix();
        glPushMatrix();
        glTranslatef(6.0f, 7.0f, 0.0f); // this will work
        glScalef(0.05f, 0.05f, 0.05f); // this will work
        draw_text(buffer);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(2.0f, 3.0f, 0.0f); // this will work
        glScalef(0.01f, 0.01f, 0.01f); // this will work
        draw_text("Please press Q to exit!");
        glPopMatrix();
        glutSwapBuffers();
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, NumberX, 0.0, NumberY, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int, int) {
    
    switch (key)
    {
        case 'q': exit(1); // quit!
    }
}

void special(int key, int, int) {
    switch (key)
    {
        case GLUT_KEY_LEFT:
            if (leftmost > 0) {
                switch (current) {
                    case 1: if (checkBoard[leftmost-1][originY+1]+checkBoard[leftmost-1][originY-1]+checkBoard[leftmost-1][originY-2] == 0) {originX--; glutPostRedisplay();} break;
                    case 2: if (checkBoard[leftmost-1][originY+1]+checkBoard[leftmost-1][originY] == 0) {originX--; glutPostRedisplay();} break;
                    case 3: switch (numberRotation) {
                        case 0: if (checkBoard[originX-2][originY+1]+checkBoard[leftmost-1][originY] == 0) {originX--; glutPostRedisplay();} break;
                        case 1: if (checkBoard[originX-1][originY]+checkBoard[originX-1][originY-1]+checkBoard[leftmost-1][originY+1] == 0) {originX--; glutPostRedisplay();} break;
                        case 2: if (checkBoard[originX][originY-1]+checkBoard[leftmost-1][originY] == 0) {originX--; glutPostRedisplay();} break;
                        case 3: if (checkBoard[originX-1][originY]+checkBoard[originX-1][originY+1]+checkBoard[leftmost-1][originY-1] == 0) {originX--; glutPostRedisplay();} break;
                    } break;
                    case 4: switch (numberRotation) {
                        case 0: if (checkBoard[originX][originY+1]+checkBoard[leftmost-1][originY] == 0) {originX--; glutPostRedisplay();} break;
                        case 1: if (checkBoard[originX-1][originY]+checkBoard[originX-1][originY+1]+checkBoard[leftmost-1][originY-1] == 0) {originX--; glutPostRedisplay();} break;
                        case 2: if (checkBoard[originX-2][originY-1]+checkBoard[leftmost-1][originY] == 0) {originX--; glutPostRedisplay();} break;
                        case 3: if (checkBoard[originX-1][originY]+checkBoard[originX-1][originY-1]+checkBoard[leftmost-1][originY+1] == 0) {originX--; glutPostRedisplay();} break;
                    } break;
                    case 5: switch (numberRotation) {
                        case 0: if (checkBoard[originX-1][originY-1]+checkBoard[leftmost-1][originY] == 0) {originX--; glutPostRedisplay();} break;
                        case 1: if (checkBoard[originX-1][originY+1]+checkBoard[leftmost-1][originY]+checkBoard[leftmost-1][originY-1] == 0) {originX--; glutPostRedisplay();} break;
                    } break;
                    case 6: switch (numberRotation) {
                        case 0: if (checkBoard[originX-1][originY]+checkBoard[leftmost-1][originY-1] == 0) {originX--; glutPostRedisplay();} break;
                        case 1: if (checkBoard[originX-1][originY-1]+checkBoard[leftmost-1][originY]+checkBoard[leftmost-1][originY+1] == 0) {originX--; glutPostRedisplay();} break;
                    } break;
                    case 7: switch (numberRotation) {
                        case 0: if (checkBoard[originX-1][originY+1]+checkBoard[leftmost-1][originY] == 0) {originX--; glutPostRedisplay();} break;
                        case 1: if (checkBoard[originX-1][originY+1]+checkBoard[originX-1][originY-1]+checkBoard[leftmost-1][originY] == 0) {originX--; glutPostRedisplay();} break;
                        case 2: if (checkBoard[originX-1][originY-1]+checkBoard[leftmost-1][originY] == 0) {originX--; glutPostRedisplay();} break;
                        case 3: if (checkBoard[originX-1][originY+1]+checkBoard[originX-1][originY-1]+checkBoard[leftmost-1][originY] == 0) {originX--; glutPostRedisplay();} break;
                    } break;
                }
            }
            downornot = false; break;
        case GLUT_KEY_RIGHT:
            if (rightmost < 10) {
                switch (current) {
                    case 1: if (checkBoard[rightmost][originY+1]+checkBoard[rightmost][originY-1]+checkBoard[rightmost][originY-2] == 0) {originX++; glutPostRedisplay();} break;
                    case 2: if (checkBoard[rightmost][originY+1]+checkBoard[rightmost][originY] == 0) {originX++; glutPostRedisplay();} break;
                    case 3: switch (numberRotation) {
                        case 0: if (checkBoard[originX][originY+1]+checkBoard[rightmost][originY] == 0) {originX++; glutPostRedisplay();} break;
                        case 1: if (checkBoard[originX+1][originY]+checkBoard[originX+1][originY-1]+checkBoard[rightmost][originY+1] == 0) {originX++; glutPostRedisplay();} break;
                        case 2: if (checkBoard[originX+2][originY-1]+checkBoard[rightmost][originY] == 0) {originX++; glutPostRedisplay();} break;
                        case 3: if (checkBoard[originX+1][originY]+checkBoard[originX+1][originY+1]+checkBoard[rightmost][originY-1] == 0) {originX++; glutPostRedisplay();} break;
                    } break;
                    case 4: switch (numberRotation) {
                        case 0: if (checkBoard[originX+2][originY+1]+checkBoard[rightmost][originY] == 0) {originX++; glutPostRedisplay();} break;
                        case 1: if (checkBoard[originX+1][originY]+checkBoard[originX+1][originY+1]+checkBoard[rightmost][originY-1] == 0) {originX++; glutPostRedisplay();} break;
                        case 2: if (checkBoard[originX][originY-1]+checkBoard[rightmost][originY] == 0) {originX++; glutPostRedisplay();} break;
                        case 3: if (checkBoard[originX+1][originY]+checkBoard[originX+1][originY-1]+checkBoard[rightmost][originY+1] == 0) {originX++; glutPostRedisplay();} break;
                    } break;
                    case 5: switch (numberRotation) {
                        case 0: if (checkBoard[originX+1][originY-1]+checkBoard[rightmost][originY-1] == 0) {originX++; glutPostRedisplay();} break;
                        case 1: if (checkBoard[originX][originY-1]+checkBoard[rightmost][originY]+checkBoard[rightmost][originY+1] == 0) {originX++; glutPostRedisplay();} break;
                    } break;
                    case 6: switch (numberRotation) {
                        case 0: if (checkBoard[originX+1][originY-1]+checkBoard[rightmost][originY] == 0) {originX++; glutPostRedisplay();} break;
                        case 1: if (checkBoard[originX][originY+1]+checkBoard[rightmost][originY]+checkBoard[rightmost][originY-1] == 0) {originX++; glutPostRedisplay();} break;
                    } break;
                    case 7: switch (numberRotation) {
                        case 0: if (checkBoard[originX+1][originY+1]+checkBoard[rightmost][originY] == 0) {originX++; glutPostRedisplay();} break;
                        case 1: if (checkBoard[originX+1][originY+1]+checkBoard[originX+1][originY-1]+checkBoard[rightmost][originY] == 0) {originX++; glutPostRedisplay();} break;
                        case 2: if (checkBoard[originX+1][originY-1]+checkBoard[rightmost][originY] == 0) {originX++; glutPostRedisplay();} break;
                        case 3: if (checkBoard[originX+1][originY+1]+checkBoard[originX+1][originY-1]+checkBoard[rightmost][originY] == 0) {originX++; glutPostRedisplay();} break;
                    } break;
                }
            }
            downornot = false; break;
        case GLUT_KEY_UP:
            if (originY < 20) {
                numberRotation = (numberRotation + 1) % transactiontype;
                downornot = false;
                glutPostRedisplay(); break;
            }
        case GLUT_KEY_DOWN: if (originY != 0) glutPostRedisplay(); break;
    }
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000/(Speed-1+level), timer, 0);
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    initialBoard(NumberX, NumberY);
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(500, 250);
    
    glutCreateWindow("Tetris");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutTimerFunc(0, timer, 0);
    
    init();
    
    glutMainLoop();
    
    return 0;
}
