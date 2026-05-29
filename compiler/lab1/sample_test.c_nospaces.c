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

// Function to calculate factorial
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

// Main function
int main() {
 // Local variables
 int numbers[5] = {1, 2, 3, 4, 5};
 int count = 5;
 char name[50];
 float average = 0.0;
 double sum = 0.0;
 
 printf("Enter your name: ");
 scanf("%s", name);
 
 // Using if-else
 if (count > 0) {
 printf("Array has elements\n");
 } else {
 printf("Array is empty\n");
 }
 
 // Using for loop
 for (int i = 0; i < count; i++) {
 sum += numbers[i];
 }
 average = sum / count;
 
 // Using while loop
 int i = 0;
 while (i < count) {
 if (is_prime(numbers[i])) {
 printf("%d is prime\n", numbers[i]);
 }
 i++;
 }
 
 // Using do-while loop
 int j = 0;
 do {
 global_count++;
 j++;
 } while (j < 3);
 
 // Using switch statement
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
 
 // Calling functions
 print_array(numbers, count);
 int fact = factorial(5);
 printf("Factorial of 5 is %d\n", fact);
 
 return 0;
}

// Another function for additional testing
void helper_function() {
 int temp = 10;
 char ch = 'X';
 float value = 2.5;
 
 for (int k = 0; k < temp; k++) {
 if (k % 2 == 0) {
 printf("Even: %d\n", k);
 }
 }
}
