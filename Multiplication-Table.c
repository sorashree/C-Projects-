#include <stdio.h>
int main(){
    int num,i;

    printf("Enter an integer: ");
    scanf("%d", &num);

    printf("\nMultiplication Table for %d:\n", num);

    for(i=0;i<=10;++i){
        printf("%d * %d= %d\n",num,i,num*i);
    }
    return 0;
}
