 #include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
 
#define WIDTH        80
#define GROUND_ROW   20
#define DINO_X       5      
#define DINO_BASE_ROW (GROUND_ROW - 1)   
#define OBS_HEIGHT   2      
#define JUMP_STRENGTH 5      
 
void gotoxy(int x, int y) {
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
 
void hideCursor(void) {
    CONSOLE_CURSOR_INFO ci = { 1, FALSE };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}
 
void eraseDinoBox(int topRow) {
    for (int r = 0; r < 3; r++) {
        gotoxy(DINO_X, topRow + r);
        printf("   ");
    }
}
 
void drawDino(int topRow) {
    gotoxy(DINO_X, topRow);     printf(" O ");
    gotoxy(DINO_X, topRow + 1); printf("/|\\");
    gotoxy(DINO_X, topRow + 2); printf("/ \\");
}

void eraseObstacle(int col) {
    gotoxy(col, GROUND_ROW - 1); printf("  ");
    gotoxy(col, GROUND_ROW);     printf("  ");
}
 
void drawObstacle(int col) {
    gotoxy(col, GROUND_ROW - 1); printf("||");
    gotoxy(col, GROUND_ROW);     printf("||");
}
 
void drawStaticUI(int score) {
    system("cls");
    gotoxy(2, 1);  printf("SPACE = Jump    X = Exit");
    gotoxy(60, 1); printf("Score: %d", score);
    for (int i = 0; i < WIDTH; i++) {
        gotoxy(i, GROUND_ROW + 1);
        printf("_");
    }
}
 
void updateScore(int score) {
    gotoxy(67, 1);
    printf("%d   ", score);
}
 

 
typedef struct {
    int jumpHeight;   
    int velocity;    
    int isJumping;
} Dino;
 
int main(void) {
    Dino dino = {0, 0, 0};
    int obsX = WIDTH - 5;
    int score = 0;
    int speed = 60;         
    int prevDinoTop, newDinoTop;
    int prevObsX;
    char ch;
 
    srand((unsigned)time(NULL));
    system("mode con: cols=80 lines=30");
    hideCursor();
    drawStaticUI(score);
    drawDino(DINO_BASE_ROW - 2);
    drawObstacle(obsX);
 
    while (1) {
        if (kbhit()) {
            ch = getch();
            if ((ch == ' ') && !dino.isJumping) {
                dino.isJumping = 1;
                dino.velocity = JUMP_STRENGTH;
            } else if (ch == 'x' || ch == 'X') {
                break;
            }
        }
 
        prevDinoTop = DINO_BASE_ROW - 2 - dino.jumpHeight;
        if (dino.isJumping) {
            dino.jumpHeight += dino.velocity;
            dino.velocity -= 1;              
            if (dino.jumpHeight <= 0) {
                dino.jumpHeight = 0;
                dino.isJumping = 0;
                dino.velocity = 0;
            }
        }
        newDinoTop = DINO_BASE_ROW - 2 - dino.jumpHeight;
 
        if (newDinoTop != prevDinoTop) {
            eraseDinoBox(prevDinoTop);
            drawDino(newDinoTop);
        }
 
       
        prevObsX = obsX;
        obsX--;
        if (obsX < 0) {
            obsX = WIDTH - 5 + (rand() % 10); 
            score++;
            updateScore(score);
            if (speed > 20) speed--;
        }
        eraseObstacle(prevObsX);
        drawObstacle(obsX);
 
        int obsLeft = obsX, obsRight = obsX + 1;
        int dinoLeft = DINO_X, dinoRight = DINO_X + 2;
        int columnsOverlap = (obsRight >= dinoLeft) && (obsLeft <= dinoRight);
        int notClearedVertically = (dino.jumpHeight < OBS_HEIGHT + 2);
 
        if (columnsOverlap && notClearedVertically) {
            gotoxy(30, 10);
            printf("GAME OVER!");
            gotoxy(28, 11);
            printf("Score: %d", score);
            gotoxy(20, 12);
            printf("Press R to Restart or X to Exit");
 
            while (1) {
                ch = getch();
                if (ch == 'r' || ch == 'R') {
                    dino.jumpHeight = 0;
                    dino.velocity = 0;
                    dino.isJumping = 0;
                    obsX = WIDTH - 5;
                    score = 0;
                    speed = 60;
                    drawStaticUI(score);
                    drawDino(DINO_BASE_ROW - 2);
                    drawObstacle(obsX);
                    break;
                }
                if (ch == 'x' || ch == 'X') return 0;
            }
            continue;
        }
        Sleep(speed);
    }
    return 0;
}
