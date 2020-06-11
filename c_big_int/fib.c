#include <stdio.h>  // printf()
#include <stdlib.h> // atoi()
#include <string.h> // strcmp()

#include "fib.h"
// List of fibonacci numbers:
// http://www.maths.surrey.ac.uk/hosted-sites/R.Knott/Fibonacci/fibtable.html

#define ASCII_0 48
#define ASCII_9 57
#define BUFFER_SIZE 210

void usage() {
    printf( "USAGE:\n" );
    printf( "   fib [n]\n" );
    printf( "\n" );
    printf( "INPUTS:\n" );
    printf( "   n       A nonnegative integer. Program computes the nth Fibonacci number.\n" );
    printf( "           If n is not provided, program computes and displays first 100 Fibonacci numbers.\n" );
}

int isPositiveInteger( char * str ) {
    if ( !str ) return 0;
    // Verify that every character is numeric
    int i;
    for ( i=0 ; i<strlen( str ); i++ ) {
        char c = str[ i ];
        if ( c < ASCII_0 || c > ASCII_9 ) {
            return 0;
        }
    }
    return 1;
}

// long unsigned fib( unsigned n )
// Returns the nth Fibonacci number.
// EXAMPLE: fib( 0 ) returns 0
//          fib( 1 ) returns 1
//          fib( 8 ) returns 21
unsigned long long fib( unsigned n ) { //this function works until 64 bit unsigned ints overflow i.e. fib(93)
  if ( n == 0 ) return 0;
  if ( n == 1 ) return 1;
  unsigned long long previous[2] = {0,1};
  unsigned long long result;
  for (int i = 2; i <= n; i++) {
    result = previous[0] + previous[1];
    previous[0] = previous[1];
    previous[1] = result;
  }
  return result;
}

char* fibonacci(unsigned n) { //this works for all fib numbers 210 digits or less
  char* number; //current number
  char* n__1; //previous fib number
  char* n__2; //previous previous fib number
  number = malloc(BUFFER_SIZE *sizeof(char));//allocate memory for strings
  n__1 = malloc(BUFFER_SIZE *sizeof(char));
  n__2 = malloc(BUFFER_SIZE *sizeof(char));

  for (int i = 0; i < BUFFER_SIZE; i++) { //initialize arrays to all zeroes
    n__1[i] = '0';
    n__2[i] = '0';
    number[i] = '0';
  }
  n__1[BUFFER_SIZE-1] = '1';// fib(1) = 1, so this will be our starting point

  if (n == 0) {//0th fib number is zero
    strcpy(number, n__2);//copy result
    free(n__1);//free memory
    free(n__2);
    return number;
  }
  if (n == 1) { //1st fib number is 1
    strcpy(number, n__1);//copy result
    free(n__1);//free memory
    free(n__2);
    return number;
  }
  int temp;//stores arithmetic result
  int carry = 0;
  for (int num = 2; num <= n; num++) {//keep building up until we arrive at desired fib number
    for (int idx = BUFFER_SIZE-1; idx >= 0; idx--) {//for every digit, from least to most significant
      temp = (n__1[idx] - ASCII_0) + (n__2[idx] - ASCII_0) + carry;//n__1 + n__2 with de-ASCIIed values
      if (temp > 9) {//overflow results in a carry operation
        carry = 1;
        temp = temp - 10;
      }
      else {//no overflow -> no carry
        carry = 0;
      }
      number[idx] = temp + ASCII_0;//convert the int back to an ASCII value
    }
    strcpy(n__2, n__1); //n__1 becomes new n__2
    strcpy(n__1, number);//number becomes new n__1
  }
  free(n__1);//free memory
  free(n__2);
  return number;//return final result as char array
}

// int main(int argc, char ** argv) {
//   //Handle too many arguments
//   if ( argc > 2 ) {
//     printf( "ERROR: Too many arguments.\n\n" );
//     usage();
//     return 1;
//   // Handle one argument
//   }
//   else if ( argc == 2 ) {
//     // Display usage if requested
//     if ( strcmp( "-h", argv[ 1 ] ) == 0 ) {
//         usage();
//         return 0;
//     }
//     // Verify numeric input
//     if ( !isPositiveInteger( argv[ 1 ] ) ) {
//         printf( "ERROR: Input must be a positive integer.\n\n" );
//         usage();
//         return 1;
//     }
//     // Display requested fibonacci number
//     unsigned n = atoi(argv[1]);
//     char* f = fibonacci(n);
//     int i;
//     for (i = 0; i < BUFFER_SIZE; i++) {
//       if (f[i] != '0') break;//strip leading zeroes
//     }
//     printf("Fibonacci number %d is %s\n", n, f+i);
//   // Display first 100 fibonacci numbers
//   }
//   else {
//     int i;
//     char* result;
//     for (int num = 0; num < 100; num++) {
//       result = fibonacci(num);
//       for (i = 0; i < BUFFER_SIZE; i++) {
//         if (result[i] != '0') break;//strip leading zeroes
//       }
//       printf("%d: %s \n", num, result+i);
//       free(result);
//     }
//   }
// }
