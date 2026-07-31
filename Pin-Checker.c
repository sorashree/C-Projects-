#include <stdio.h>

int main() {
    int correct_pin = 1234;
    int entered_pin;
    int attempts = 0;

pin_prompt:
    if (attempts >= 3) {
        goto account_locked;
    }

    printf("Enter your 4 digit pin: ");
    scanf("%d", &entered_pin);

    if (entered_pin == correct_pin) {
        goto access_granted;
    }
    else {
        attempts++;
        printf("Incorrect Pin. Try Again\n");
        goto pin_prompt;
    }
access_granted:
    printf("Access Granted\n");
    return 0;

account_locked:
    printf("Account Locked\n");
    return 0;
}
