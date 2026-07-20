#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define WIDTH 20
#define HEIGHT 10

int gameOver, score, totalDots, won;
int pacX, pacY;
int ghostX, ghostY;

char maze[HEIGHT][WIDTH + 1] = {
    "####################",
    "#........#.........#",
    "#.####...#...####..#",
    "#..................#",
    "#.###.#####.###....#",
    "#........#.........#",
    "#.####...#...####..#",
    "#..................#",
    "#........#.........#",
    "####################"
};

void setup() {
    gameOver = 0;
    won = 0;
    score = 0;
    pacX = 1;
    pacY = 1;
    ghostX = WIDTH - 2;
    ghostY = HEIGHT - 2;

    /* Count dots so we can detect a "win" when all are eaten */
    totalDots = 0;
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            if (maze[i][j] == '.')
                totalDots++;
}

void clearScreen() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD topLeft = {0, 0};
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO screen;

    GetConsoleScreenBufferInfo(hOut, &screen);
    FillConsoleOutputCharacter(
        hOut, ' ',
        screen.dwSize.X * screen.dwSize.Y,
        topLeft, &written
    );
    SetConsoleCursorPosition(hOut, topLeft);
}

void draw() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    SetConsoleCursorPosition(hOut, coord);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == pacY && j == pacX)
                printf("C");
            else if (i == ghostY && j == ghostX)
                printf("G");
            else
                printf("%c", maze[i][j]);
        }
        printf("\n");
    }
    printf("\nScore: %d / %d\n", score, totalDots * 10);
    printf("Controls: W A S D | X to Exit\n");
}

void input() {
    int newX = pacX;
    int newY = pacY;

    if (GetAsyncKeyState('A') & 0x8000) newX--;
    if (GetAsyncKeyState('D') & 0x8000) newX++;
    if (GetAsyncKeyState('W') & 0x8000) newY--;
    if (GetAsyncKeyState('S') & 0x8000) newY++;
    if (GetAsyncKeyState('X') & 0x8000) gameOver = 1;

    if (newX >= 0 && newX < WIDTH &&
        newY >= 0 && newY < HEIGHT &&
        maze[newY][newX] != '#') {
        pacX = newX;
        pacY = newY;
    }
}

void logic() {
    if (maze[pacY][pacX] == '.') {
        score += 10;
        maze[pacY][pacX] = ' ';
        if (score >= totalDots * 10) {
            won = 1;
            gameOver = 1;
            return;
        }
    }

    int newGX = ghostX;
    int newGY = ghostY;

    if (pacX < ghostX) newGX--;
    else if (pacX > ghostX) newGX++;
    else if (pacY < ghostY) newGY--;
    else if (pacY > ghostY) newGY++;

    if (newGX >= 0 && newGX < WIDTH &&
        newGY >= 0 && newGY < HEIGHT &&
        maze[newGY][newGX] != '#') {
        ghostX = newGX;
        ghostY = newGY;
    }

    if (pacX == ghostX && pacY == ghostY) {
        gameOver = 1;
    }
}

int main() {
    setup();

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);
    system("cls");

    while (!gameOver) {
        clearScreen();
        draw();
        input();
        logic();
        Sleep(100);
    }

    clearScreen();
    if (won)
        printf("You Win! Final Score: %d\n", score);
    else
        printf("Game Over! Final Score: %d\n", score);

    /* Restore the cursor so it doesn't stay hidden in the terminal
       after the program exits */
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    return 0;
}
