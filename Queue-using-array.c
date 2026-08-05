#include <stdio.h>
#define MAX 100

struct Queue {
    int arr[MAX];
    int front;
    int rear;
};

void initQueue(struct Queue *q) {
    q->front = -1;
    q->rear = -1;
}

int isEmpty(struct Queue *q) {
    return (q->front == -1 || q->front > q->rear);
}
int isFull(struct Queue *q) {
    return q->rear == MAX - 1;
}
void enqueue(struct Queue *q, int value) {

    if (isFull(q)) {
        printf("\nQueue Overflow! Cannot insert %d.\n", value);
        return;
    }

    if (q->front == -1)
        q->front = 0;

    q->arr[++q->rear] = value;

    printf("\n%d inserted successfully.\n", value);
}

int dequeue(struct Queue *q) {

    if (isEmpty(q)) {
        printf("\nQueue Underflow! Queue is empty.\n");
        return -1;
    }

    return q->arr[q->front++];
}

int peek(struct Queue *q) {

    if (isEmpty(q)) {
        printf("\nQueue is empty.\n");
        return -1;
    }

    return q->arr[q->front];
}

void display(struct Queue *q) {

    if (isEmpty(q)) {
        printf("\nQueue is empty.\n");
        return;
    }

    printf("\nQueue Elements:\n");
    printf("-----------------------\n");

    for (int i = q->front; i <= q->rear; i++) {
        printf("%d ", q->arr[i]);
    }

    printf("\n-----------------------\n");
}

int main() {

    struct Queue queue;

    int choice;
    int value;

    initQueue(&queue);

    do {
        printf("QUEUE USING ARRAY\n");
        printf("1. Enqueue\n");
        printf("2. Dequeue\n");
        printf("3. Peek\n");
        printf("4. Display\n");
        printf("5. Check if Empty\n");
        printf("6. Check if Full\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {

        case 1:
            printf("Enter value to enqueue: ");
            scanf("%d", &value);
            enqueue(&queue, value);
            break;

        case 2:
            value = dequeue(&queue);
            if (value != -1)
                printf("\nDequeued Element: %d\n", value);
            break;

        case 3:
            value = peek(&queue);
            if (value != -1)
                printf("\nFront Element: %d\n", value);
            break;

        case 4:
            display(&queue);
            break;

        case 5:
            if (isEmpty(&queue))
                printf("\nQueue is Empty.\n");
            else
                printf("\nQueue is NOT Empty.\n");
            break;

        case 6:
            if (isFull(&queue))
                printf("\nQueue is Full.\n");
            else
                printf("\nQueue is NOT Full.\n");
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
