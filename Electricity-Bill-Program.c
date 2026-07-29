#include <stdio.h>

int main() {
    int units;
    float base_amount = 0.0, surcharge = 0.0, total_bill = 0.0;
    const float MIN_CHARGE = 100.0; 
  
    printf("Enter total electricity units consumed: ");
    if (scanf("%d", &units) != 1 || units < 0) {
        printf("Error: Please enter a valid non-negative number of units.\n");
        return 1;
    }

    if (units <= 50) {
        base_amount = units * 0.50;
    } 
    else if (units <= 150) {
        base_amount = (50 * 0.50) + ((units - 50) * 0.75);
    } 
    else if (units <= 250) {
        base_amount = (50 * 0.50) + (100 * 0.75) + ((units - 150) * 1.20);
    } 
    else {
        base_amount = (50 * 0.50) + (100 * 0.75) + (100 * 1.20) + ((units - 250) * 1.50);
    }

  
    if (base_amount > 400.0) {
        surcharge = base_amount * 0.20;
    }
  
    total_bill = base_amount + surcharge + MIN_CHARGE;
  
    printf("\n----------- ELECTRICITY BILL -----------\n");
    printf("Units Consumed:     %d\n", units);
    printf("Base Charges:       $%.2f\n", base_amount);
    printf("Surcharge (20%%):    $%.2f\n", surcharge);
    printf("Meter Fixed Charge: $%.2f\n", MIN_CHARGE);
    printf("----------------------------------------\n");
    printf("Total Amount Due:   $%.2f\n", total_bill);
    printf("----------------------------------------\n");

    return 0;
}
