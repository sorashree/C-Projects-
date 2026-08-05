#include <stdio.h>
#define MAX 100

struct Stack {
    int arr[MAX];
    int top;
};

void initStack(struct Stack *stack) {
    stack->top = -1;
}

int isEmpty(struct Stack *stack) {
    return stack->top == -1;
}

int isFull(struct Stack *stack) {
    return stack->top == MAX - 1;
}

void push(struct Stack *stack, int value) {
    if (isFull(stack)) {
        printf("\nStack Overflow! Cannot insert %d.\n", value);
        return;
    }

    stack->arr[++stack->top] = value;
    printf("\n%d inserted successfully.\n", value);
}

int pop(struct Stack *stack) {
    if (isEmpty(stack)) {
        printf("\nStack Underflow! Stack is empty.\n");
        return -1;
    }

    return stack->arr[stack->top--];
}
int peek(struct Stack *stack) {
    if (isEmpty(stack)) {
        printf("\nStack is empty.\n");
        return -1;
    }

    return stack->arr[stack->top];
}

void display(struct Stack *stack) {

    if (isEmpty(stack)) {
        printf("\nStack is empty.\n");
        return;
    }

    printf("\nStack Elements (Top to Bottom)\n");
    printf("------------------------------\n");

    for (int i = stack->top; i >= 0; i--) {
        printf("| %d |\n", stack->arr[i]);
    }

    printf("------------------------------\n");
}


int main() {
    struct Stack stack;
    int choice;
    int value;
    initStack(&stack);

    do {
        printf("       STACK USING ARRAY\n");
        printf("1. Push\n");
        printf("2. Pop\n");
        printf("3. Peek\n");
        printf("4. Display\n");
        printf("5. Check if Empty\n");
        printf("6. Check if Full\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter value to push: ");
            scanf("%d", &value);
            push(&stack, value);
            break;

        case 2:
            value = pop(&stack);
            if (value != -1)
                printf("\nPopped Element: %d\n", value);
            break;

        case 3:
            value = peek(&stack);
            if (value != -1)
                printf("\nTop Element: %d\n", value);
            break;

        case 4:
            display(&stack);
            break;

        case 5:
            if (isEmpty(&stack))
                printf("\nStack is Empty.\n");
            else
                printf("\nStack is NOT Empty.\n");
            break;

        case 6:
            if (isFull(&stack))
                printf("\nStack is Full.\n");
            else
                printf("\nStack is NOT Full.\n");
            break;

        case 7:
            printf("\nThank you for using the program!\n");
            break;

        default:
            printf("\nInvalid Choice! Please try again.\n");
        }

    } while (choice != 7);

    return 0;
}
