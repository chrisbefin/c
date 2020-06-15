#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ASCII_0 48
#define ASCII_9 57

void primes(int n) {
  int primes[n];
  primes[0] = 2; //start from the first prime number
  int curr = 0, temp, flag;
  printf("%d \n", primes[curr]);
  for(int i = 1; i < n; i++) { //find n prime numbers
    temp = primes[curr]; //start searching from last found prime
    while(1) {//keep looking until the next prime is found
      for(int j = 0; j <= curr; j++) {
        if(temp % primes[j] == 0) {
          flag = 0;
          break;//if temp is integer divisible by a previous prime, it is not a prime
        }
        flag = 1; // j could be a prime until it fails the test case above
      }
      if(flag == 1) {// if temp passes all tests, you have found a prime
        curr++;
        primes[curr] = temp;
        printf("%d \n", temp);
        break;
      }
      temp++;//check next number
    }
  }
  return;
}

void isocoles(int n) {
  for(int i = 0; i < n; i++) {
    for(int j = 0; j <= i; j++) {
      printf("*");
    }
    printf("\n");
  }
  return;
}

double pi(int n) {
  // pi = 4 * sum{k=1 to n} (  (-1)^(k-1) ) / ( 2k - 1 )
  double result = 0;
  int prints;
  if (n > 9) prints = 9;
  else prints = n;
  printf("   n   |   approximation\n");
  for(int i = 1; i <= n; i++) {
    result += 4 * (pow(-1.0, i-1) / (2*i - 1));
    if (i <= prints) {
      printf("   %d   |   %lf\n", i, result);
    }
  }
  return result;
}

int string_to_int(char* number) {
  // - is decimal 45 on ASCII table
  int length = strlen(number);
  int limit, place = 0;
  int is_neg, temp = 0;
  int result = 0;//stores value to be returned
  if (number[0] == 45) { //number is negative
    limit = 1; //do not look at first char since it is not a number
    is_neg = 1; //number is negative
  }
  else {
    limit = 0;
  }
  for(int i = length - 1; i >= limit; i--) {
    if(number[i] < ASCII_0 || number[i] > ASCII_9) {
      printf("invalid input \n");
      return -1;
    }
    else {
      temp = number[i] - ASCII_0; //normalize to decimal value
      result += temp * pow(10, place);//adjust number for significance
      place++;
    }
  }
  if (is_neg == 1) {
    result = result * -1;//flip number to reflect minus sign
  }
  printf("%d \n", result);
  return result;
}
int main (int argc, char *argv[]) {
  switch(atoi(argv[1])) {
    case 1:
      primes(atoi(argv[2]));
      break;
    case 2:
      isocoles(atoi(argv[2]));
      break;
    case 3:
      pi(atoi(argv[2]));
      break;
    case 4:
      string_to_int(argv[2]);
      break;
    default:
      printf("incorrect input \n");
  }
  return 0;
}
