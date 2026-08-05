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
        printf("Stack Overflow! Cannot push %d.\n", value);
        return;
    }
    stack->arr[++stack->top] = value;
    printf("%d pushed onto the stack.\n", value);
}

int pop(struct Stack *stack) {
    if (isEmpty(stack)) {
        printf("Stack Underflow! Stack is empty.\n");
        return -1;
    }

    return stack->arr[stack->top--];
}

int peek(struct Stack *stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty.\n");
        return -1;
    }

    return stack->arr[stack->top];
}

void display(struct Stack *stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty.\n");
        return;
    }

    printf("\nStack elements (Top to Bottom):\n");

    for (int i = stack->top; i >= 0; i--) {
        printf("%d\n", stack->arr[i]);
    }
}

int main() {
    struct Stack stack;
    int choice, value;

    initStack(&stack);

    while (1) {
        printf("\n===== STACK MENU =====\n");
        printf("1. Push\n");
        printf("2. Pop\n");
        printf("3. Peek\n");
        printf("4. Display\n");
        printf("5. Exit\n");
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
                printf("Popped element: %d\n", value);
            break;

        case 3:
            value = peek(&stack);
            if (value != -1)
                printf("Top element: %d\n", value);
            break;

        case 4:
            display(&stack);
            break;

        case 5:
            printf("Exiting program...\n");
            return 0;

        default:
            printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
