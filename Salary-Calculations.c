#include <stdio.h> 

int main() { 
    float basic, da, hra, ta, gross, deduction, net; 
    float total, average, percentage; 
    char grade; 

    printf("Enter Basic Salary: "); 
    scanf("%f", &basic); 
    
    printf("Enter Dearness Allowance (DA): "); 
    scanf("%f", &da); 
    
    printf("Enter House Rent Allowance (HRA): "); 
    scanf("%f", &hra); 
    
    printf("Enter Transport Allowance (TA): "); 
    scanf("%f", &ta); 

    printf("Enter Deduction: "); 
    scanf("%f", &deduction); 

    gross = basic + da + hra + ta; 
    net = gross - deduction; 
    total = basic + da + hra; 
    average = total / 3.0; 
    percentage = (net / gross) * 100; 

    if (percentage >= 90) { 
        grade = 'A'; 
    } 
    else if (percentage >= 75) { 
        grade = 'B'; 
    } 
    else if (percentage >= 50) { 
        grade = 'C'; 
    } 
    else if (percentage >= 35) { 
        grade = 'D'; 
    } 
    else { 
        grade = 'F'; 
    } 

    printf("\n--- Salary Slip ---"); 
    printf("\nGross Salary: %.2f", gross); 
    printf("\nNet Salary: %.2f", net); 
    printf("\nTotal: %.2f", total); 
    printf("\nAverage: %.2f", average); 
    printf("\nPercentage: %.2f%%", percentage); 
    printf("\nGrade: %c\n", grade); 

    return 0; 
}
