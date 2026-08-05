#include <stdio.h>
#include <stdbool.h>

bool hasTwoSum(int arr[], int size, int target) {
  int left = 0;
  int right = size - 1;

  while (left < right) {
    int current_sum = arr[left] + arr[right];

    if (current_sum == target) {
      printf("Pair found: %d+%d=%d\n", arr[left], arr[right], target);
      return true;
    }
    else if (current_sum < target) {
      left++;
    }
    else {
      right--;
    }
  }
  return false;
}

int main() {
  int arr[] = {2, 5, 8, 12, 16, 23, 38};
  int target = 20;

  int size = sizeof(arr) / sizeof(arr[0]);

  if (!hasTwoSum(arr, size, target)) {
    printf("No pair exists that adds up to %d\n", target);
  }
  return 0;
}
