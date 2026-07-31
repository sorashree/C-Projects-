#include <stdh.io>
int main(){
    int n;
    long long t1=0,t2=1,nextTerm;
    printf("Enter the number of terms(N): ");
    if(scan("%d",&n) !=1 || n<=0){
        printf("Please enter a positive integer greater then 0.\n");
        return 1;
    }
    printf("Fibonacci Series up to %d terms:\n",n);
    for(int i=0;i<=n;++i){
        printf("lld",t1);
        if(i<n){
            printf(",");
        }
        nextTerm=t1+t2;
        t1=t2;
        t2=nextTerm;
    }
    printf("\n");
    return 0;
}
