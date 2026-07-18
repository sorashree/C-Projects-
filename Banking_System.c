#include <stdio.h>
#include <string.h>

#define ACCOUNT_FILE "account.dat"

typedef struct {
    char name[50];
    int acc_no;
    float balance;
} Account;

void create_account();
void deposit_money();
void withdraw_money();
void check_balance();

int main() {
    int choice;

    while (1) {
        printf("\n========== BANK MANAGEMENT SYSTEM ==========\n");
        printf("1. Create Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                create_account();
                break;
            case 2:
                deposit_money();
                break;
            case 3:
                withdraw_money();
                break;
            case 4:
                check_balance();
                break;
            case 5:
                printf("Thank you for using our bank.\n");
                return 0;
            default:
                printf("Invalid Choice!\n");
        }
    }
}

void create_account() {
    FILE *file = fopen(ACCOUNT_FILE, "ab");

    if (file == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    Account acc;

    getchar(); // remove newline

    printf("Enter Name: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = '\0';

    printf("Enter Account Number: ");
    scanf("%d", &acc.acc_no);

    acc.balance = 0;

    fwrite(&acc, sizeof(Account), 1, file);

    fclose(file);

    printf("Account Created Successfully!\n");
}

void deposit_money() {
    FILE *file = fopen(ACCOUNT_FILE, "rb+");

    if (file == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    int acc_no;
    float amount;
    Account acc;

    printf("Enter Account Number: ");
    scanf("%d", &acc_no);

    printf("Enter Amount to Deposit: ");
    scanf("%f", &amount);

    while (fread(&acc, sizeof(Account), 1, file)) {
        if (acc.acc_no == acc_no) {
            acc.balance += amount;

            fseek(file, -sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, file);

            fclose(file);

            printf("Deposit Successful!\n");
            printf("Current Balance = Rs %.2f\n", acc.balance);
            return;
        }
    }

    fclose(file);
    printf("Account Not Found.\n");
}

void withdraw_money() {
    FILE *file = fopen(ACCOUNT_FILE, "rb+");

    if (file == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    int acc_no;
    float amount;
    Account acc;

    printf("Enter Account Number: ");
    scanf("%d", &acc_no);

    printf("Enter Amount to Withdraw: ");
    scanf("%f", &amount);

    while (fread(&acc, sizeof(Account), 1, file)) {
        if (acc.acc_no == acc_no) {

            if (amount > acc.balance) {
                printf("Insufficient Balance!\n");
                fclose(file);
                return;
            }

            acc.balance -= amount;

            fseek(file, -sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, file);

            fclose(file);

            printf("Withdrawal Successful!\n");
            printf("Current Balance = Rs %.2f\n", acc.balance);
            return;
        }
    }

    fclose(file);
    printf("Account Not Found.\n");
}

void check_balance() {
    FILE *file = fopen(ACCOUNT_FILE, "rb");

    if (file == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    int acc_no;
    Account acc;

    printf("Enter Account Number: ");
    scanf("%d", &acc_no);

    while (fread(&acc, sizeof(Account), 1, file)) {
        if (acc.acc_no == acc_no) {
            printf("\nAccount Holder : %s\n", acc.name);
            printf("Account Number : %d\n", acc.acc_no);
            printf("Balance        : Rs %.2f\n", acc.balance);

            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("Account Not Found.\n");
}
