#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ---------------------------- Configuration --------------------------- */
#define MAX_MOVIES            50
#define MAX_BOOKINGS          200
#define MAX_SEATS_PER_BOOKING 10
#define TITLE_LEN             100
#define LANG_LEN              30
#define TIME_LEN              20
#define NAME_LEN              50
#define MAX_ROWS              6
#define MAX_COLS              10
#define MAX_SEATS             (MAX_ROWS * MAX_COLS)   /* 60 seats per show */

#define SEAT_AVAILABLE        'O'
#define SEAT_BOOKED           'X'

#define MOVIES_FILE           "movies.dat"
#define BOOKINGS_FILE         "bookings.dat"

/* ------------------------------- Types --------------------------------- */
typedef struct {
    int   id;
    char  title[TITLE_LEN];
    char  language[LANG_LEN];
    char  showtime[TIME_LEN];
    float price;
    int   rows;
    int   cols;
    char  seats[MAX_SEATS];   /* 'O' = available, 'X' = booked */
} Movie;

typedef struct {
    int   booking_id;
    int   movie_id;
    char  customer_name[NAME_LEN];
    int   seat_indices[MAX_SEATS_PER_BOOKING];
    int   seat_count;
    float total_amount;
    int   active;   /* 1 = active, 0 = cancelled */
} Booking;

/* ------------------------------ Globals --------------------------------- */
static Movie   g_movies[MAX_MOVIES];
static int     g_movie_count;
static Booking g_bookings[MAX_BOOKINGS];
static int     g_booking_count;

/* ------------------------- Input helper functions ------------------------ */
static void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}

static int read_int_in_range(const char *prompt, int min, int max) {
    int value, result;
    while (1) {
        printf("%s", prompt);
        result = scanf("%d", &value);
        clear_input_buffer();
        if (result != 1) { printf("Invalid input. Please enter a number.\n"); continue; }
        if (value < min || value > max) { printf("Please enter a value between %d and %d.\n", min, max); continue; }
        return value;
    }
}

static float read_positive_float(const char *prompt) {
    float value; int result;
    while (1) {
        printf("%s", prompt);
        result = scanf("%f", &value);
        clear_input_buffer();
        if (result != 1 || value <= 0) { printf("Invalid input. Please enter a positive number.\n"); continue; }
        return value;
    }
}

static void read_line(const char *prompt, char *buf, int len) {
    printf("%s", prompt);
    if (fgets(buf, len, stdin) != NULL) {
        size_t l = strlen(buf);
        if (l > 0 && buf[l - 1] == '\n') buf[l - 1] = '\0';
    } else {
        buf[0] = '\0';
    }
}

/* ------------------------------ Movie logic ------------------------------ */
static int movie_load_all(void) {
    FILE *fp = fopen(MOVIES_FILE, "rb");
    if (fp == NULL) return 0;
    int count = 0;
    while (count < MAX_MOVIES && fread(&g_movies[count], sizeof(Movie), 1, fp) == 1) count++;
    fclose(fp);
    return count;
}

static int movie_save_all(void) {
    FILE *fp = fopen(MOVIES_FILE, "wb");
    if (fp == NULL) { printf("Error: could not save movies.\n"); return -1; }
    size_t written = fwrite(g_movies, sizeof(Movie), (size_t)g_movie_count, fp);
    fclose(fp);
    return (written == (size_t)g_movie_count) ? 0 : -1;
}

static void movie_add(const char *title, const char *language, const char *showtime,
                       float price, int rows, int cols) {
    if (g_movie_count >= MAX_MOVIES) { printf("Cannot add more movies: catalogue full.\n"); return; }
    if (rows * cols > MAX_SEATS) { printf("Cannot add movie: too many seats requested.\n"); return; }

    Movie *m = &g_movies[g_movie_count];
    m->id = g_movie_count + 1;
    strncpy(m->title, title, TITLE_LEN - 1);       m->title[TITLE_LEN - 1] = '\0';
    strncpy(m->language, language, LANG_LEN - 1);  m->language[LANG_LEN - 1] = '\0';
    strncpy(m->showtime, showtime, TIME_LEN - 1);  m->showtime[TIME_LEN - 1] = '\0';
    m->price = price;
    m->rows = rows;
    m->cols = cols;
    memset(m->seats, SEAT_AVAILABLE, MAX_SEATS);

    g_movie_count++;
    movie_save_all();
}

static int movie_find_index_by_id(int id) {
    for (int i = 0; i < g_movie_count; i++)
        if (g_movies[i].id == id) return i;
    return -1;
}

static int movie_count_available_seats(const Movie *m) {
    int available = 0, total = m->rows * m->cols;
    for (int i = 0; i < total; i++)
        if (m->seats[i] == SEAT_AVAILABLE) available++;
    return available;
}

static void movie_display_list(void) {
    if (g_movie_count == 0) { printf("\nNo movies available right now.\n"); return; }
    printf("\n%-4s %-30s %-12s %-8s %-8s %s\n", "ID", "Title", "Language", "Time", "Price", "Seats Left");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < g_movie_count; i++) {
        printf("%-4d %-30s %-12s %-8s %-8.2f %d/%d\n",
               g_movies[i].id, g_movies[i].title, g_movies[i].language, g_movies[i].showtime,
               g_movies[i].price, movie_count_available_seats(&g_movies[i]),
               g_movies[i].rows * g_movies[i].cols);
    }
}

static void movie_display_seat_map(const Movie *m) {
    printf("\nSeat map for \"%s\" (%s):\n\n", m->title, m->showtime);
    int seat_num = 1;
    for (int r = 0; r < m->rows; r++) {
        printf("Row %c: ", 'A' + r);
        for (int c = 0; c < m->cols; c++) {
            int idx = r * m->cols + c;
            printf("[%2d:%c] ", seat_num, m->seats[idx]);
            seat_num++;
        }
        printf("\n");
    }
    printf("\nLegend: %c = available, %c = booked\n", SEAT_AVAILABLE, SEAT_BOOKED);
}

/* ----------------------------- Booking logic ------------------------------ */
static int booking_load_all(void) {
    FILE *fp = fopen(BOOKINGS_FILE, "rb");
    if (fp == NULL) return 0;
    int count = 0;
    while (count < MAX_BOOKINGS && fread(&g_bookings[count], sizeof(Booking), 1, fp) == 1) count++;
    fclose(fp);
    return count;
}

static int booking_save_all(void) {
    FILE *fp = fopen(BOOKINGS_FILE, "wb");
    if (fp == NULL) { printf("Error: could not save bookings.\n"); return -1; }
    size_t written = fwrite(g_bookings, sizeof(Booking), (size_t)g_booking_count, fp);
    fclose(fp);
    return (written == (size_t)g_booking_count) ? 0 : -1;
}

static int booking_next_id(void) {
    int max_id = 0;
    for (int i = 0; i < g_booking_count; i++)
        if (g_bookings[i].booking_id > max_id) max_id = g_bookings[i].booking_id;
    return max_id + 1;
}


static int booking_create(int movie_id, const char *customer_name,
                           const int *seat_indices, int seat_count) {
    int mi = movie_find_index_by_id(movie_id);
    if (mi == -1) { printf("Booking failed: movie not found.\n"); return -1; }
    if (g_booking_count >= MAX_BOOKINGS) { printf("Booking failed: system full.\n"); return -1; }

    Movie *m = &g_movies[mi];

    for (int i = 0; i < seat_count; i++) {
        int idx = seat_indices[i];
        if (idx < 0 || idx >= m->rows * m->cols) { printf("Booking failed: seat out of range.\n"); return -1; }
        if (m->seats[idx] != SEAT_AVAILABLE) { printf("Booking failed: seat %d already booked.\n", idx + 1); return -1; }
    }

    for (int i = 0; i < seat_count; i++) m->seats[seat_indices[i]] = SEAT_BOOKED;

    Booking *b = &g_bookings[g_booking_count];
    b->booking_id = booking_next_id();
    b->movie_id = movie_id;
    strncpy(b->customer_name, customer_name, NAME_LEN - 1); b->customer_name[NAME_LEN - 1] = '\0';
    b->seat_count = seat_count;
    for (int i = 0; i < seat_count; i++) b->seat_indices[i] = seat_indices[i];
    b->total_amount = m->price * seat_count;
    b->active = 1;
    g_booking_count++;

    movie_save_all();
    booking_save_all();
    return b->booking_id;
}

static int booking_cancel(int booking_id) {
    for (int i = 0; i < g_booking_count; i++) {
        if (g_bookings[i].booking_id == booking_id && g_bookings[i].active) {
            int mi = movie_find_index_by_id(g_bookings[i].movie_id);
            if (mi != -1) {
                Movie *m = &g_movies[mi];
                for (int s = 0; s < g_bookings[i].seat_count; s++)
                    m->seats[g_bookings[i].seat_indices[s]] = SEAT_AVAILABLE;
            }
            g_bookings[i].active = 0;
            movie_save_all();
            booking_save_all();
            return 1;
        }
    }
    return 0;
}

static void booking_print_ticket(const Booking *b, const Movie *m) {
    printf("\n==================== TICKET ====================\n");
    printf(" Booking ID   : %d\n", b->booking_id);
    printf(" Movie        : %s\n", m->title);
    printf(" Showtime     : %s\n", m->showtime);
    printf(" Customer     : %s\n", b->customer_name);
    printf(" Seats booked : ");
    for (int i = 0; i < b->seat_count; i++) {
        printf("%d", b->seat_indices[i] + 1);
        if (i < b->seat_count - 1) printf(", ");
    }
    printf("\n Total amount : %.2f\n", b->total_amount);
    printf("==================================================\n");
}

static void booking_display_all(void) {
    if (g_booking_count == 0) { printf("\nNo bookings found.\n"); return; }
    printf("\n%-6s %-20s %-25s %-8s %-8s\n", "ID", "Customer", "Movie", "Seats", "Status");
    printf("------------------------------------------------------------------\n");
    for (int i = 0; i < g_booking_count; i++) {
        int mi = movie_find_index_by_id(g_bookings[i].movie_id);
        const char *title = (mi != -1) ? g_movies[mi].title : "(deleted)";
        printf("%-6d %-20s %-25s %-8d %-8s\n",
               g_bookings[i].booking_id, g_bookings[i].customer_name, title,
               g_bookings[i].seat_count, g_bookings[i].active ? "ACTIVE" : "CANCELLED");
    }
}

/* -------------------------------- Flows ---------------------------------- */
static void seed_demo_movies_if_empty(void) {
    if (g_movie_count > 0) return;
    movie_add("Inception", "English", "18:30", 250.0f, 5, 10);
    movie_add("Interstellar", "English", "21:00", 280.0f, 5, 10);
    movie_add("Baahubali", "Telugu", "15:00", 220.0f, 6, 10);
}

static void flow_book_ticket(void) {
    movie_display_list();
    if (g_movie_count == 0) return;

    int movie_id = read_int_in_range("\nEnter movie ID to book: ", 1, g_movie_count);
    int mi = movie_find_index_by_id(movie_id);
    if (mi == -1) { printf("Movie not found.\n"); return; }

    movie_display_seat_map(&g_movies[mi]);
    if (movie_count_available_seats(&g_movies[mi]) == 0) { printf("Sorry, this show is fully booked.\n"); return; }

    char name[NAME_LEN];
    read_line("Your name: ", name, NAME_LEN);

    int num_seats = read_int_in_range("How many seats do you want? ", 1, MAX_SEATS_PER_BOOKING);
    int seat_indices[MAX_SEATS_PER_BOOKING];
    int max_seat_num = g_movies[mi].rows * g_movies[mi].cols;

    for (int i = 0; i < num_seats; i++) {
        char prompt[64];
        snprintf(prompt, sizeof(prompt), "Seat number %d of %d: ", i + 1, num_seats);
        int seat_num = read_int_in_range(prompt, 1, max_seat_num);
        seat_indices[i] = seat_num - 1;
    }

    int booking_id = booking_create(movie_id, name, seat_indices, num_seats);
    if (booking_id != -1) {
        for (int i = 0; i < g_booking_count; i++) {
            if (g_bookings[i].booking_id == booking_id) {
                booking_print_ticket(&g_bookings[i], &g_movies[mi]);
                break;
            }
        }
    }
}

static void flow_cancel_ticket(void) {
    booking_display_all();
    if (g_booking_count == 0) return;

    int id = read_int_in_range("\nEnter booking ID to cancel: ", 1, 100000);
    if (booking_cancel(id)) printf("Booking %d cancelled and seats released.\n", id);
    else printf("No active booking found with that ID.\n");
}

static void user_menu(void) {
    int choice;
    do {
        printf("\n----- Customer Menu -----\n");
        printf("1. View Movies\n2. Book a Ticket\n3. Cancel a Booking\n4. View All Bookings\n5. Back\n");
        choice = read_int_in_range("Choose an option: ", 1, 5);
        switch (choice) {
            case 1: movie_display_list(); break;
            case 2: flow_book_ticket(); break;
            case 3: flow_cancel_ticket(); break;
            case 4: booking_display_all(); break;
        }
    } while (choice != 5);
}

static void admin_menu(void) {
    int choice;
    do {
        printf("\n----- Admin Menu -----\n");
        printf("1. View Movies\n2. Add a Movie\n3. View All Bookings\n4. Back\n");
        choice = read_int_in_range("Choose an option: ", 1, 4);

        if (choice == 1) {
            movie_display_list();
        } else if (choice == 2) {
            char title[TITLE_LEN], language[LANG_LEN], showtime[TIME_LEN];
            read_line("Title: ", title, TITLE_LEN);
            read_line("Language: ", language, LANG_LEN);
            read_line("Showtime (e.g. 19:00): ", showtime, TIME_LEN);
            float price = read_positive_float("Ticket price: ");
            int rows = read_int_in_range("Number of rows (1-6): ", 1, MAX_ROWS);
            int cols = read_int_in_range("Seats per row (1-10): ", 1, MAX_COLS);
            int before = g_movie_count;
            movie_add(title, language, showtime, price, rows, cols);
            if (g_movie_count > before) printf("Movie added successfully.\n");
        } else if (choice == 3) {
            booking_display_all();
        }
    } while (choice != 4);
}

/* --------------------------------- main ----------------------------------- */
int main(void) {
    g_movie_count   = movie_load_all();
    g_booking_count = booking_load_all();
    seed_demo_movies_if_empty();

    int choice;
    do {
        printf("\n===== MOVIE TICKET BOOKING SYSTEM =====\n");
        printf("1. Customer Menu\n2. Admin Menu\n3. Exit\n");
        choice = read_int_in_range("Choose an option: ", 1, 3);
        switch (choice) {
            case 1: user_menu();  break;
            case 2: admin_menu(); break;
            case 3: printf("Goodbye!\n"); break;
        }
    } while (choice != 3);

    return 0;
}
