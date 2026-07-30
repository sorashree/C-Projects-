#include <stdio.h>
#include <string.h>

#define MAX_CONTACTS 50

struct Contact {
    char name[50];
    char phone[15];
};

int main() {
    struct Contact phonebook[MAX_CONTACTS];
    int contact_count = 0;
    int choice;

    while (1) {
        printf("\n--- Contact Book Mini-System ---\n");
        printf("1. Add Contact\n");
        printf("2. Display All Contacts\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); 

        if (choice == 1) {
            if (contact_count < MAX_CONTACTS) {
                printf("Enter name: ");
                fgets(phonebook[contact_count].name, sizeof(phonebook[contact_count].name), stdin);
                phonebook[contact_count].name[strcspn(phonebook[contact_count].name, "\n")] = 0; 

                printf("Enter phone number: ");
                fgets(phonebook[contact_count].phone, sizeof(phonebook[contact_count].phone), stdin);
                phonebook[contact_count].phone[strcspn(phonebook[contact_count].phone, "\n")] = 0; 

                contact_count++;
                printf("Contact saved successfully!\n");
            } else {
                printf("Phonebook is full!\n");
            }
        } 
        else if (choice == 2) {
            if (contact_count == 0) {
                printf("No contacts found.\n");
            } else {
                printf("\n--- Contact List ---\n");
                for (int i = 0; i < contact_count; i++) {
                    printf("%d. Name: %s | Phone: %s\n", i + 1, phonebook[i].name, phonebook[i].phone);
                }
            }
        } 
        else if (choice == 3) {
            printf("Exiting program. Goodbye!\n");
            break;
        } 
        else {
            printf("Invalid selection. Try again.\n");
        }
    }

    return 0;
}
