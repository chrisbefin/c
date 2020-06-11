#ifndef C_BIG_INT_H
#define C_BIG_INT_H


/* This header defines the big_int struct 
 * which provides an unsigned integer type of infinite size.
 *
 * A big_int represents its numeric value as a dynamically allocated array of bytes (segments).
 * segment[0] is the least significant byte of the integer.
 * 
 * Member "n" will always contain the number of segments allocated for member array "segments".
 *
 * A big_int representing 0x01020304 will look like the following:
 *   n >= 4
 *   segments[0] == 0x04
 *   segments[1] == 0x03
 *   segments[2] == 0x02
 *   segments[3] == 0x01
 *
 * The number of segments increases as necessary.
 * The number of segments never decreases.
 *
 * An initialized big_int has a single segment with value 0.
 */

struct big_int
{
    // Array of segments containing the big integer
    unsigned char * segments;

    // The number of segments currently allocated
    unsigned n;
};

// Initialize a big_int
void bi_init( struct big_int * );

// Delete a big_int
void bi_delete( struct big_int * );

// Set the value of an initialized big_int struct according to an unsigned integer.
void bi_set_uint( struct big_int *, unsigned );

// Perform a deep copy of a BigInt struct.
void bi_copy( struct big_int * a, struct big_int * b );

// Determine whether two big_ints are equivalent
int bi_is_equal( struct big_int * a, struct big_int * b );

// Perform a += b
void bi_plus_equals( struct big_int * a, struct big_int * b );

// Perform target = a + b
void bi_add( struct big_int * target, struct big_int * a, struct big_int * b );

// Generate a text representation of a big_int, no more than max_chars characters.
void bi_to_string( struct big_int *, char * buf, unsigned max_chars );

// Write a text representation of a big_int to stdout
void bi_print( struct big_int * );


#endif
