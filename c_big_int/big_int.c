

#include "big_int.h"

#include <stdlib.h>
#include <stdio.h>


// ------------- PRIVATE FUNCTIONS

void bi_expand( struct big_int * b )
/*
    Allocates more memory to a big_int.
    The value must not change, but the number of allocated bytes should grow.

    Some potential strategies:
    - Expand by one byte
    - Expand by a large fixed number of bytes
    - Double the size
    - Multiply size by another fixed number

    Whenever you identify that a big_int must expand,
    you probably want to do something like:

        while b is not big enough:
            bi_expand( &b );
*/
{
  b->n = b->n + 1;//add one segment
  b->segments = realloc(b->segments, b->n * sizeof(char));
  b->segments[(b->n)-1] = 0;
  return;
    // TODO: Implement me
}

// ------------- PUBLIC FUNCTIONS

void bi_init( struct big_int * b )
/*
    Initializes a big_int to zero.
*/
{
  b->segments = malloc(1 *sizeof(char));//start with 1 segment
  b->segments[0] = 0;
  b->n = 1;
  return;
    // TODO: Implement me
}


void bi_delete( struct big_int * b )
/*
    Render a big_int NULL.
    This will require freeing any dynamically allocated memory, at least.
*/
{
  b->n = 0;
  free(b->segments);
  b->segments = 0;
  return;
    // TODO: Implement me
}


void bi_set_uint( struct big_int * b, unsigned u )
/*
    Sets the numeric value of a big_int equal to an unsigned int.
*/
{
  int remainder = 1;//initialize to any non-zero value
  int seg = 0;//current segment being written to
  while (u != 0) {
    if (seg == b->n) {
      bi_expand(b);//add a new segment if necessary
    }
    remainder = u % 256;
    u = u / 256;
    b->segments[seg] = remainder;//insert result into the proper segment
    seg++;//move on the the next segment
  }
  if (u == 0 || seg < b->n) {
    for(int i = seg; i < b->n; i++) { //zero out remaining segments
      b->segments[i] = 0;
    }
  }
    // TODO: Implement me
}


void bi_copy( struct big_int * destination, struct big_int * source )
/*
    Perform a deep copy of a big_int.
    Equivalent to   destination = source;
*/
{
  int size;
  if (destination->n > source->n) {//data will be copied up to the shortest struct's size
    size = source->n;
  }
  else {
    size = destination->n;
  }
  for(int i = 0; i < size; i++) {
    destination->segments[i] = source->segments[i];
  }
  return;
    // TODO: Implement me
}


int bi_is_equal( struct big_int * a, struct big_int * b )
/*
    Equivalent to ( a == b )

    Two big_ints are equal if their numeric value is identical,
    regardless how much memory is allocated to their segments.

    RETURNS: 0 if a is not equal to b
             1 if a is equal to b
*/
{
  int size;
  if (a->n > b->n) {//data will be copied up to the shortest struct's size
    size = b->n;
  }
  else {
    size = a->n;
  }
  for(int i = 0; i < size; i++) {
    if (a->segments[i] != b->segments[i]) return 0;
  }
  for(int i = size; i < a->n; i++) {
    if (a->segments[size] != 0) return 0;
  }
  for(int i = size; i < b->n; i++) {
    if (b->segments[size] != 0) return 0;
  }
  return 1;
    // TODO: Implement me
}


void bi_plus_equals( struct big_int * a, struct big_int * b   )
/*
     Equivalent to   a += b;
*/
{
  int size;
  if (a->n > b->n) {//find size of smaller big number
    size = b->n;
  }
  else {
    size = a->n;
  }
  int carry = 0;
  int temp;
  for(int i = 0; i < size; i++) {
    temp = a->segments[i] + b->segments[i] + carry;
    if (temp > 255) {
      carry = 1;
      temp = temp - 256;
    }
    else {
      carry = 0;
    }
    a->segments[i] = temp;
  }
  return;
    // TODO: Implement me
}

void bi_add( struct big_int * target, struct big_int * a, struct big_int * b   )
/*
 * Add two big_ints.
 * Performs:  target = a + b
 * a and b must not be modified.
 * target, a, and b must be separate initialized big_int structs.
 * NOTE: This function does not support a = a + b
 *       but does support target = a + a
 */
{
  int size;
  if (a->n > b->n) {//find size of smaller big number
    size = b->n;
  }
  else {
    size = a->n;
  }
  int carry = 0;
  int temp;
  for(int i = 0; i < size; i++) {
    temp = a->segments[i] + b->segments[i] + carry;
    if (temp > 255) {
      carry = 1;
      temp = temp - 256;
    }
    else {
      carry = 0;
    }
    target->segments[i] = temp;
  }
  return;
    // TODO: Implement me
}


void bi_to_string( struct big_int * b, char * buf, unsigned max_chars  )
/*
 *  Generates a hexidecimal string representation of a big_int struct.
 *
 *  ARGS:
 *    b         A pointer to the BigInt struct to convert
 *    buf       A character buffer to write the string into.
 *    max_chars The max number of chars to write into buf.
 *
 *  NOTE: If a buffer larger than max_chars is required,
 *        then the generated string will be a truncated version of the true
 *        value, indicated by a trailing "...".
 *  NOTE: Writes an empty string if b is invalid for any reason,
 *        such as uninitialized.
 *  NOTE: If max_chars is less than 10 bytes, always generates "---"
 */

{
    // TODO: Implement me
}


void bi_print( struct big_int * b )
/*
    Prints the value of a big number to stdout.
*/
{
    // TODO: Implement me
}
