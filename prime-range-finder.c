#include <stdio.h>

int isPrime(int num) {
    if (num <= 1) return 0;
    if (num == 2) return 1; 
    if (num % 2 == 0) return 0; 

    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int lower, upper;

    printf("Enter lower bound of the range: ");
    if (scanf("%d", &lower) != 1) return 1;
    printf("Enter upper bound of the range: ");
    if (scanf("%d", &upper) != 1) return 1;

    if (lower > upper) {
        int temp = lower;
        lower = upper;
        upper = temp;
    }
    
    printf("Prime numbers between %d and %d are:\n", lower, upper);

    int foundPrime = 0;
    for (int i = lower; i <= upper; i++) {
        if (isPrime(i)) {
            printf("%d ", i); 
            foundPrime = 1;
        }
    }
    
    if (!foundPrime) {
        printf("None");
    }
    printf("\n");
    
    return 0;
}
