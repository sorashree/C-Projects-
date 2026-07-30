#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define SIZE 4
#define WIN_TILE 2048

static int board[SIZE][SIZE];
static long long score = 0;
static int moved_flag = 0;

static HANDLE hConsole;
static void set_color(int color) {
    SetConsoleTextAttribute(hConsole, (WORD)color);
}
static void gotoxy(int x, int y) {
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    SetConsoleCursorPosition(hConsole, coord);
}
static void hide_cursor(void) {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &info);
}
static void clear_screen(void) {
    system("cls");
}
static int color_for(int value) {
    switch (value) {
        case 0:    return 8; 
        case 2:    return 15;  
        case 4:    return 14;  
        case 8:    return 12;  
        case 16:   return 10;  
        case 32:   return 11;  
        case 64:   return 13;  
        case 128:  return 9;   
        case 256:  return 14;
        case 512:  return 12;
        case 1024: return 10;
        case 2048: return 13;
        default:   return 15;
    }
}
static void spawn_tile(void) {
    int empty_r[SIZE * SIZE], empty_c[SIZE * SIZE];
    int count = 0;
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (board[r][c] == 0) {
                empty_r[count] = r;
                empty_c[count] = c;
                count++;
            }
        }
    }
    if (count == 0) return;
    int idx = rand() % count;
    board[empty_r[idx]][empty_c[idx]] = (rand() % 10 == 0) ? 4 : 2;
}
static void init_board(void) {
    memset(board, 0, sizeof(board));
    score = 0;
    spawn_tile();
    spawn_tile();
}
static void print_board(void) {
    gotoxy(0, 0);
    set_color(15);
    printf("=========  2048  =========\n");
    printf("Score: %-10lld\n\n", score);
    for (int r = 0; r < SIZE; r++) {
        set_color(15);
        printf("+------+------+------+------+\n");
        for (int c = 0; c < SIZE; c++) {
            printf("|");
            int val = board[r][c];
            set_color(color_for(val));
            if (val == 0) {
                printf("      ");
            } else {
                printf("%6d", val);
            }
            set_color(15);
        }
        printf("|\n");
    }
    set_color(15);
    printf("+------+------+------+------+\n\n");
    printf("Move: W/A/S/D or Arrow Keys | Q: Quit | R: Restart\n");
}
static int compress_merge_line(int line[SIZE]) {
    int changed = 0;
    int temp[SIZE];
    int idx = 0;

    for (int i = 0; i < SIZE; i++) {
        if (line[i] != 0) {
            temp[idx++] = line[i];
        }
    }
    for (int i = idx; i < SIZE; i++) temp[i] = 0;
    for (int i = 0; i < SIZE - 1; i++) {
        if (temp[i] != 0 && temp[i] == temp[i + 1]) {
            temp[i] *= 2;
            score += temp[i];
            temp[i + 1] = 0;
            i++; 
        }
    }
    int final[SIZE];
    idx = 0;
    for (int i = 0; i < SIZE; i++) {
        if (temp[i] != 0) final[idx++] = temp[i];
    }
    for (int i = idx; i < SIZE; i++) final[i] = 0;

    for (int i = 0; i < SIZE; i++) {
        if (final[i] != line[i]) changed = 1;
        line[i] = final[i];
    }
    return changed;
}

static void move_left(void) {
    moved_flag = 0;
    for (int r = 0; r < SIZE; r++) {
        int line[SIZE];
        for (int c = 0; c < SIZE; c++) line[c] = board[r][c];
        if (compress_merge_line(line)) moved_flag = 1;
        for (int c = 0; c < SIZE; c++) board[r][c] = line[c];
    }
}

static void reverse_row(int row[SIZE]) {
    for (int i = 0; i < SIZE / 2; i++) {
        int tmp = row[i];
        row[i] = row[SIZE - 1 - i];
        row[SIZE - 1 - i] = tmp;
    }
}

static void move_right(void) {
    moved_flag = 0;
    for (int r = 0; r < SIZE; r++) {
        int line[SIZE];
        for (int c = 0; c < SIZE; c++) line[c] = board[r][c];
        reverse_row(line);
        int changed = compress_merge_line(line);
        reverse_row(line);
        if (changed) moved_flag = 1;
        for (int c = 0; c < SIZE; c++) board[r][c] = line[c];
    }
}

static void transpose(void) {
    for (int r = 0; r < SIZE; r++) {
        for (int c = r + 1; c < SIZE; c++) {
            int tmp = board[r][c];
            board[r][c] = board[c][r];
            board[c][r] = tmp;
        }
    }
}
static void move_up(void) {
    transpose();
    move_left();
    transpose();
}
static void move_down(void) {
    transpose();
    move_right();
    transpose();
}
static int has_won(void) {
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            if (board[r][c] == WIN_TILE) return 1;
    return 0;
}
static int has_moves_left(void) {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (board[r][c] == 0) return 1;
            if (c < SIZE - 1 && board[r][c] == board[r][c + 1]) return 1;
            if (r < SIZE - 1 && board[r][c] == board[r + 1][c]) return 1;
        }
    }
    return 0;
}

int main(void) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    hide_cursor();
    srand((unsigned int)time(NULL));

    clear_screen();
    init_board();
    print_board();

    int won_message_shown = 0;

    while (1) {
        int key = _getch();
        int handled = 1;

        if (key == 0 || key == 224) {
            
            int arrow = _getch();
            switch (arrow) {
                case 72: move_up();    break; 
                case 80: move_down();  break;
                case 75: move_left();  break; 
                case 77: move_right(); break; 
                default: handled = 0; break;
            }
        } else {
            switch (key) {
                case 'w': case 'W': move_up();    break;
                case 's': case 'S': move_down();  break;
                case 'a': case 'A': move_left();  break;
                case 'd': case 'D': move_right(); break;
                case 'r': case 'R':
                    init_board();
                    won_message_shown = 0;
                    handled = 0; 
                    clear_screen();
                    print_board();
                    continue;
                case 'q': case 'Q':
                    clear_screen();
                    set_color(15);
                    printf("Thanks for playing! Final score: %lld\n", score);
                    return 0;
                default:
                    handled = 0;
                    break;
            }
        }

        if (!handled) continue;

        if (moved_flag) {
            spawn_tile();
        }

        clear_screen();
        print_board();

        if (has_won() && !won_message_shown) {
            set_color(14);
            printf("\n*** You reached 2048! Keep going for a higher score, ");
            printf("or press Q to quit. ***\n");
            set_color(15);
            won_message_shown = 1;
        }

        if (!has_moves_left()) {
            set_color(12);
            printf("\nGame Over! Final score: %lld\n", score);
            printf("Press R to restart or Q to quit.\n");
            set_color(15);
        }
    }

    return 0;
}
