// Sample C program for testing
// This file contains various elements to test all programs

#include <stdio.h>
#include <stdlib.h>

/* Multi-line comment
   for testing comment counting */

// Global variables
int global_count = 0;
float pi = 3.14;
char letter = 'A';
double precision = 0.0001;

int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Function to check if number is prime
int is_prime(int num) {
    if (num <= 1) return 0;
    
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

// Function to print array
void print_array(int arr[], int size) {
    printf("Array elements: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int numbers[5] = {1, 2, 3, 4, 5};
    int count = 5;
    char name[50];
    float average = 0.0;
    double sum = 0.0;
    
    printf("Enter your name: ");
    scanf("%s", name);
    
    if (count > 0) {
        printf("Array has elements\n");
    } else {
        printf("Array is empty\n");
    }
    
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
    }
    average = sum / count;
    
    int i = 0;
    while (i < count) {
        if (is_prime(numbers[i])) {
            printf("%d is prime\n", numbers[i]);
        }
        i++;
    }
    
    int j = 0;
    do {
        global_count++;
        j++;
    } while (j < 3);
    
    switch (count) {
        case 0:
            printf("Empty\n");
            break;
        case 5:
            printf("Full array\n");
            break;
        default:
            printf("Partial array\n");
    }
    
    print_array(numbers, count);
    int fact = factorial(5);
    printf("Factorial of 5 is %d\n", fact);
    
    return 0;
}