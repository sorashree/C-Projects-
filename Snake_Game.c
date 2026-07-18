#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define HEIGHT 20
#define WIDTH 60

int score = 0;
int fruit_x = 30, fruit_y = 10;
int head_x = 15, head_y = 8;

struct termios old_props;

void clear_screen();
void draw();
void set_terminal_attributes();
void reset_terminal_attributes();

int main() {
    set_terminal_attributes();

    draw();

    getchar();

    reset_terminal_attributes();
    return 0;
}

void draw() {
    clear_screen();

    printf("\tWelcome to the Snake Game\n\n");

    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");

    for (int i = 0; i < HEIGHT; i++) {

        printf("\n#");

        for (int j = 0; j < WIDTH; j++) {

            if (i == fruit_y && j == fruit_x)
                printf("F");

            else if (i == head_y && j == head_x)
                printf("O");

            else
                printf(" ");
        }

        printf("#");
    }

    printf("\n");

    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");

    printf("\nScore: %d\n", score);
}

void set_terminal_attributes() {
    struct termios new_props;

    tcgetattr(STDIN_FILENO, &old_props);

    new_props = old_props;
    new_props.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &new_props);
}

void reset_terminal_attributes() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_props);
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
