#include <stdio.h>

int main() {
    int sum = 0;
    for (int i = 1; i <= 100; i++) {
        // Skip multiples of 7
        if (i % 7 == 0) {
            continue;
        }
        sum += i;
    }
    printf("The sum of numbers from 1 to 100 excluding multiples of 7 is: %d\n", sum);

    return 0;
}
