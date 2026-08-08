#include <stdio.h>

#define MONTHS 12

int main() {
    double sales[MONTHS];
    double total_sales = 0.0;
    const char *month_names[] = {
        "January", "February", "March", "April", 
        "May", "June", "July", "August", 
        "September", "October", "November", "December"
    };

    printf("Monthly Sales Calculator\n");

    for (int i = 0; i < MONTHS; i++) {
        printf("Enter sales for %s: $", month_names[i]);
       
        while (scanf("%lf", &sales[i]) != 1 || sales[i] < 0) {
            printf("Invalid input. Please enter a positive number: $");
            while (getchar() != '\n'); 
        }
        total_sales += sales[i];
    }
    printf("Total Annual Sales: $%.2f\n", total_sales);
    printf("Average Monthly Sales: $%.2f\n", total_sales / MONTHS);

    return 0;
}
