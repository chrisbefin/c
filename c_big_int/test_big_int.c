
/* A unit test suite for the BigInt struct.
 */

#include <stdio.h>     // printf()
#include <string.h>    // strcmp()
#include "ctest.h"     // Our unit testing library 
#include "big_int.h"  // The library being tested

// Macro to print a message that includes the line number
// EXAMPLE:
// DEBUG( something is going wrong here )
#define DEBUG( x ) printf( "%u : %s\n", __LINE__, #x );



TEST_CASE( test_init )
// Verify that bi_init() leaves a BigInt in the correct state.
{
    struct big_int b;
    bi_init( &b );

    ASSERT_EQUAL( b.n, 1 );
    ASSERT_EQUAL( b.segments[0], 0 );

    bi_delete( &b );
}


TEST_CASE( test_delete )
// Verify that bi_delete() leaves a BigInt in the correct state.
{
    struct big_int b;
    bi_init( &b );

    bi_delete( &b );
    ASSERT_EQUAL( b.n, 0 );
    ASSERT_EQUAL( b.segments, 0 );
}

TEST_CASE( test_expand )
// Verify that internal function expand() functions properly.
{
    struct big_int b;
    bi_init( &b );

    // Expand from one to two segments
    bi_set_uint( &b, 12 );
    ASSERT_EQUAL( b.n, 1 );
    ASSERT_EQUAL( b.segments[0], 12 );
    bi_expand( &b );
    ASSERT_EQUAL( b.n, 2 );
    ASSERT_EQUAL( b.segments[0], 12 );
    ASSERT_EQUAL( b.segments[1], 0 );

    // Expand from two to four segments
    bi_expand( &b );
    ASSERT_EQUAL( b.n, 4 );
    ASSERT_EQUAL( b.segments[0], 12 );
    ASSERT_EQUAL( b.segments[1], 0 );
    ASSERT_EQUAL( b.segments[2], 0 );
    ASSERT_EQUAL( b.segments[3], 0 );

    // Expand from four to eight segments
    b.segments[3] = 99;
    bi_expand( &b );
    ASSERT_EQUAL( b.n, 8 );
    ASSERT_EQUAL( b.segments[0], 12 );
    ASSERT_EQUAL( b.segments[1], 0 );
    ASSERT_EQUAL( b.segments[2], 0 );
    ASSERT_EQUAL( b.segments[3], 99 );
    ASSERT_EQUAL( b.segments[4], 0 );
    ASSERT_EQUAL( b.segments[5], 0 );
    ASSERT_EQUAL( b.segments[6], 0 );
    ASSERT_EQUAL( b.segments[7], 0 );

    bi_delete( &b );
}

TEST_CASE( test_bi_set_uint)
{
    struct big_int b;
    bi_init( &b );
    int i;

    // Verify that the BigInt has been correctly initialized
    ASSERT_EQUAL( b.n, 1 );
    ASSERT_EQUAL( b.segments[0], 0 );

    // Value that fits within one segment
    bi_set_uint( &b, 12 );
    ASSERT_TRUE( b.n >= 1 );
    ASSERT_EQUAL( b.segments[0], 12 );
    for ( i = 1; i<b.n; ++i ) {
        ASSERT_EQUAL( b.segments[i], 0x00 );
    }
    bi_set_uint( &b, 255 );
    ASSERT_TRUE( b.n >= 1 );
    ASSERT_EQUAL( b.segments[0], 255 );
    for ( i = 1; i<b.n; ++i ) {
        ASSERT_EQUAL( b.segments[i], 0x00 );
    }

    // Value that requires two segments
    bi_set_uint( &b, 0x0100 );
    ASSERT_TRUE( b.n >= 2 );
    ASSERT_EQUAL( b.segments[0], 0x00 );
    ASSERT_EQUAL( b.segments[1], 0x01 );
    for ( i = 2; i<b.n; ++i ) {
        ASSERT_EQUAL( b.segments[i], 0x00 );
    }
    bi_set_uint( &b, 0x4E20 ); // 0b 0100 1110 0010 0000
    ASSERT_TRUE( b.n >= 2 );
    ASSERT_EQUAL( b.segments[0], 0x20 );
    ASSERT_EQUAL( b.segments[1], 0x4E );
    for ( i = 2; i<b.n; ++i ) {
        ASSERT_EQUAL( b.segments[i], 0x00 );
    }
    bi_set_uint( &b, 0xFFFF ); // 0b 1111 1111 1111 1111
    ASSERT_TRUE( b.n >= 2 );
    ASSERT_EQUAL( b.segments[0], 0xFF );
    ASSERT_EQUAL( b.segments[1], 0xFF );
    for ( i = 2; i<b.n; ++i ) {
        ASSERT_EQUAL( b.segments[i], 0x00 );
    }

    // One segment again
    bi_set_uint( &b, 0xC8 ); // 0b 1100 1000
    ASSERT_TRUE( b.n >= 1 );
    ASSERT_EQUAL( b.segments[0], 0xC8 );
    for ( i = 1; i<b.n; ++i ) {
        ASSERT_EQUAL( b.segments[i], 0x00 );
    }

    // Value that requires three segments
    bi_set_uint( &b, 0x00010000 );
    ASSERT_TRUE( b.n >= 3 );
    ASSERT_EQUAL( b.segments[0], 0x00 );
    ASSERT_EQUAL( b.segments[1], 0x00 );
    ASSERT_EQUAL( b.segments[2], 0x01 );
    for ( i = 3; i<b.n; ++i ) {
        ASSERT_EQUAL( b.segments[i], 0x00 );
    }

    // Max unsigned int  ( 4 bytes )
    bi_set_uint( &b, 0xFFFFFFFF );
    ASSERT_TRUE( b.n >= 4 );
    ASSERT_EQUAL( b.segments[0], 0xFF );
    ASSERT_EQUAL( b.segments[1], 0xFF );
    ASSERT_EQUAL( b.segments[2], 0xFF );
    ASSERT_EQUAL( b.segments[3], 0xFF );
    for ( i = 4; i<b.n; ++i ) {
        ASSERT_EQUAL( b.segments[i], 0x00 );
    }

    // Zero
    bi_set_uint( &b, 0 );
    ASSERT_TRUE( b.n >= 1 );
    ASSERT_EQUAL( b.segments[0], 0 );
    for ( i = 1; i<b.n; ++i ) {
        ASSERT_EQUAL( b.segments[i], 0x00 );
    }

    bi_delete( &b );
}


TEST_CASE( test_bi_copy_single_segment )
// Test copying a BigInt with a small value to a freshly initialized BigInt
{
    struct big_int a,b;
    bi_init( &a );
    bi_init( &b );

    // Set value on source BigInt
    bi_set_uint( &a, 37 );

    // Verify starting state of both BigInts
    ASSERT_EQUAL( a.n, 1 );
    ASSERT_EQUAL( a.segments[0], 37 );
    ASSERT_EQUAL( b.n, 1 );
    ASSERT_EQUAL( b.segments[0], 0 );

    // Perform copy
    bi_copy( &b, &a );
    ASSERT_EQUAL( a.n, 1 );
    ASSERT_EQUAL( a.segments[0], 37 );
    ASSERT_EQUAL( b.n, 1 );
    ASSERT_EQUAL( b.segments[0], 37 );

    // Cleanup
    bi_delete( &a );
    bi_delete( &b );

}

TEST_CASE( test_bi_copy )
// Verify that bi_copy() performs a deep copy
{
    struct big_int a,b;
    bi_init( &a );
    bi_init( &b );

    // Copy a BigInt
    bi_set_uint( &a, 37 );
    bi_copy( &b, &a );
    ASSERT_EQUAL( a.n, b.n );
    ASSERT_EQUAL( a.segments[0], b.segments[0] );

    // Can modify source without affecting copy
    bi_set_uint( &a, 1111 );
    ASSERT_EQUAL( a.n, 2 );
    ASSERT_EQUAL( a.segments[0], 87 );
    ASSERT_EQUAL( a.segments[1], 4 );
    ASSERT_EQUAL( b.n, 1 );
    ASSERT_EQUAL( b.segments[0], 37 );

    // Cleanup
    bi_delete( &a );
    bi_delete( &b );
}

TEST_CASE( test_bi_copy_multiple_segments_to_single_segment )
// Verify copy when source has multiple segments and destination has only one.
// - Verifies that destination expands as necessary.
{
    struct big_int a,b;
    bi_init( &a );
    bi_init( &b );

    // Copy BigInt with multiple segments to one with only one segment
    bi_set_uint( &a, 20000 ); // 0x 0100 1110 0010 0000
    ASSERT_EQUAL( a.n, 2 );
    ASSERT_EQUAL( a.segments[0], 0x20 );
    ASSERT_EQUAL( a.segments[1], 0x4E );
    ASSERT_EQUAL( b.n, 1 );

    bi_copy( &b, &a );
    ASSERT_EQUAL( b.n, 2 );
    ASSERT_EQUAL( b.segments[0], 0x20 );
    ASSERT_EQUAL( b.segments[1], 0x4E );

    // Cleanup
    bi_delete( &a );
    bi_delete( &b );
}


TEST_CASE( test_bi_copy_single_segment_to_multiple_segments )
// Verify copy when source has a single segment and destination has multiple.
// - Verifies that upper bytes of the destination are zeroed as necessary.
{
    struct big_int a,b;
    bi_init( &a );
    bi_init( &b );

    // Copy BigInt with one segment to one with multiple segments
    bi_set_uint( &a, 2 ); 
    ASSERT_EQUAL( a.n, 1 );
    ASSERT_EQUAL( a.segments[0], 2 );

    bi_set_uint( &b, 20000 );
    ASSERT_EQUAL( b.n, 2 );
    ASSERT_EQUAL( b.segments[0], 0x20 );
    ASSERT_EQUAL( b.segments[1], 0x4E );

    bi_copy( &b, &a );
    ASSERT_EQUAL( b.n, 2 );
    ASSERT_EQUAL( b.segments[0], 2 );
    ASSERT_EQUAL( b.segments[1], 0 );

    // Cleanup
    bi_delete( &a );
    bi_delete( &b );
}


TEST_CASE( test_bi_add )
//void bi_add( struct big_int * target, struct big_int * a, struct big_int * b ) {
{
    struct big_int a,b,c;
    bi_init( &a );
    bi_init( &b );
    bi_init( &c );

    // Sum is within range of two bytes
    bi_set_uint( &a, 200 );    // 0x      1100 1000
    bi_set_uint( &b, 1999 );   // 0x 0111 1100 1111
    bi_add( &c, &a, &b ); // 0x 1000 1001 0111
    ASSERT_EQUAL( c.n, 2 );
    ASSERT_EQUAL( c.segments[0], 0x97 );
    ASSERT_EQUAL( c.segments[1], 0x08 );

    bi_set_uint( &a, 87 );      // 0x           0101 0111
    bi_set_uint( &b, 65000 );   // 0x 1111 1101 1110 1000
    bi_add( &c, &a, &b );  // 0x 1111 1110 0011 1111
    ASSERT_EQUAL( c.n, 2 );
    ASSERT_EQUAL( c.segments[0], 0x3F );
    ASSERT_EQUAL( c.segments[1], 0xFE );

    // Sum is within range of four bytes
    bi_set_uint( &a, 65010 );    // 0x           1111 1101 1111 0010 
    bi_set_uint( &b, 2001003 );  // 0x 0001 1110 1000 1000 0110 1011
    bi_add( &c, &a, &b );   // 0x 0001 1111 1000 0110 0101 1101 
    ASSERT_EQUAL( c.n, 4 );
    ASSERT_EQUAL( c.segments[0], 0x5D );
    ASSERT_EQUAL( c.segments[1], 0x86 );
    ASSERT_EQUAL( c.segments[2], 0x1F );
    ASSERT_EQUAL( c.segments[3], 0x00 );

    // Sum requires many bytes
    bi_set_uint( &a, 4000000000 );  //           0x 1110 1110 0110 1011 0010 1000 0000 0000 
    bi_set_uint( &b, 3995432400 );  //           0x 1110 1110 0010 0101 0111 0101 1101 0000
    bi_add( &c, &a, &b );      // 0x 0000 0001 1101 1100 1001 0000 1001 1101 1101 0000
    ASSERT_EQUAL( c.n, 8 );
    ASSERT_EQUAL( c.segments[0], 0xD0 );
    ASSERT_EQUAL( c.segments[1], 0x9D );
    ASSERT_EQUAL( c.segments[2], 0x90 );
    ASSERT_EQUAL( c.segments[3], 0xDC );
    ASSERT_EQUAL( c.segments[4], 0x01 );
    ASSERT_EQUAL( c.segments[5], 0x00 );
    ASSERT_EQUAL( c.segments[6], 0x00 );
    ASSERT_EQUAL( c.segments[7], 0x00 );

    bi_delete( &a );
    bi_delete( &b );
    bi_delete( &c );
}

TEST_CASE( test_bi_add_sum_larger_than_addends )
// Test the case where the sum has more segments than either addend
{
    struct big_int a,b,c;
    bi_init( &a );
    bi_init( &b );
    bi_init( &c );

    // Sum is within range of two bytes
    bi_set_uint( &a, 0xFF );   // 0b           1111 1111   0x  ff
    bi_set_uint( &b, 0xFF );   // 0b           1111 1111   0x  ff
    bi_add( &c, &a, &b ); // 0b 0000 0001 1111 1110   0x01fe
    ASSERT_TRUE( c.n >= 2 );
    ASSERT_EQUAL( c.segments[0], 0xfe );
    ASSERT_EQUAL( c.segments[1], 0x01 );

    bi_delete( &a );
    bi_delete( &b );
    bi_delete( &c );
}

TEST_CASE( test_bi_to_string_initialized )
// Test bitIntToString() for newly initialized BigInt
{
    char buf[ 1000 ];
    struct big_int a;
    bi_init( &a );
   
    // Newly initialized BigInt --> "0x00"
    bi_to_string( &a, buf, sizeof( buf ) );
    ASSERT_EQUAL( strcmp( buf, "0x00" ), 0 );

    // Cleanup
    bi_delete( &a );
}


TEST_CASE( test_bi_to_string_one_byte )
// Test bi_to_string() for a value that requires one byte.
{
    char buf[ 1000 ];
    struct big_int a;
    bi_init( &a );
   
    // Small number
    bi_set_uint( &a, 200 ); 
    bi_to_string( &a, buf, sizeof( buf ) );
    ASSERT_EQUAL( strcmp( buf, "0xc8" ), 0 );

    // Cleanup
    bi_delete( &a );
}

   
TEST_CASE( test_bi_to_string_four_bytes )
// Test bi_to_string() for a value that requires four bytes.
{
    char buf[ 1000 ];
    struct big_int a;
    bi_init( &a );
   
    // A four byte value -- 2^32 - 1
    bi_set_uint( &a, 4294967295 );
    bi_to_string( &a, buf, sizeof( buf ) );
    ASSERT_EQUAL( strcmp( buf, "0xffffffff" ), 0 );

    // Cleanup
    bi_delete( &a );
}

TEST_CASE( test_bi_to_string_large_value )
// Test bi_to_string() for a value that requires four bytes.
{
    char buf[ 1000 ];
    struct big_int a;
    bi_init( &a );

    // Ensure the big number has enough segments
    int i;
    for ( i=0; i<10; i++ ) bi_expand( &a );
    ASSERT_TRUE( a.n >= 40 );

    // Set a large value
    unsigned ints[ 10 ] = { 0x01020304, 0x22334455, 0xaabbccdd, 0xffff0000, 0x09080706,
                             0x010408cc, 0x11223344, 0x99999999, 0xabcdef12, 0xffffffff };
    memcpy( a.segments, ints, 40 );

    bi_to_string( &a, buf, sizeof( buf ) );
    ASSERT_EQUAL( strcmp( buf, "0xffffffffabcdef129999999911223344010408cc09080706ffff0000aabbccdd2233445501020304"), 0 );

    // Cleanup
    bi_delete( &a );
}

 
TEST_CASE( test_bi_to_string_small_buffer )
// Test bi_to_string() when the string buffer is smaller than required
{
    char buf[ 20 ];
    struct big_int a;
    bi_init( &a );

    // Ensure the big number has enough segments
    int i;
    for ( i=0; i<10; i++ ) bi_expand( &a );
    ASSERT_TRUE( a.n >= 40 );

    // Set a large value
    unsigned ints[ 10 ] = { 0x01020304, 0x22334455, 0xaabbccdd, 0xffff0000, 0x09080706,
                             0x010408cc, 0x11223344, 0x99999999, 0xabcdef12, 0xffffffff };
    memcpy( a.segments, ints, 40 );

    bi_to_string( &a, buf, sizeof( buf ) );
    ASSERT_EQUAL( strcmp( buf, "0xffffffffabcdef..." ), 0 );

    // Cleanup
    bi_delete( &a );
}



TEST_CASE( test_fib_1001 )
// Calculate fib(1001) using BigInt struct
{
    unsigned char fib1001[] = {
            0x00,0x36,0xc3,0xee,0x02,0x14,0x8d,0x55,0x11,0x8f,0x24,0x5b,0xbe,0xe4,0x80,0xb6,0x39,0xda,0x51,0xdf,0x96,0xac,0x7d,0x0d,
            0x58,0xfa,0x12,0x09,0x35,0xaa,0x00,0x00,0x3c,0x2a,0xe9,0x58,0xea,0xe1,0xe6,0x03,0xa2,0x7f,0xa9,0x58,0x8d,0xfe,0x35,0x11,
            0x72,0xec,0xe9,0x5f,0xe7,0xb0,0xf7,0xca,0xa9,0x6f,0xac,0xea,0xa3,0x28,0x4f,0x2b,0x25,0x49,0xf1,0xca,0x6e,0xbf,0x4d,0x77,
            0xd8,0x5b,0x9b,0x41,0x9a,0xab,0xe5,0x9f,0x7e,0x01,0x18,0x52,0x4e,0x95,0x60,0xed 
            };
            
    const char * fib1001_str = "0x36c3ee02148d55118f245bbee480b639da51df96ac7d0d"
                               "58fa120935aa00003c2ae958eae1e603a27fa9588dfe3511"
                               "72ece95fe7b0f7caa96faceaa3284f2b2549f1ca6ebf4d77"
                               "d85b9b419aabe59f7e0118524e9560ed";

    struct big_int a,b,c;
    bi_init( &a );
    bi_init( &b );
    bi_init( &c );

    bi_set_uint( &a, 0 ); 
    bi_set_uint( &b, 1 ); 
    bi_set_uint( &c, 0 ); 

    struct big_int * nm2 = &c;
    struct big_int * nm1 = &a;
    struct big_int * fib = &b;
    int i;
    for ( i=1; i<1001; i++ ) {
        struct big_int * tmp = nm2;
        nm2 = nm1;
        nm1 = fib;
        fib = tmp;
        bi_add( fib, nm1, nm2 );
    }

    // Verify expected segment count is correct
    ASSERT_EQUAL( fib->n, 128 );

    // Verify segments are correct
    for ( i=0; i<sizeof( fib1001 ); ++i ) {
        ASSERT_EQUAL( fib->segments[i], fib1001[ sizeof( fib1001 ) - i - 1] );
    }

    // Verify string is correct
    char buf[1000];
    bi_to_string( fib, buf, sizeof( buf ) );

    ASSERT_EQUAL( strcmp( buf, fib1001_str ), 0 );

    bi_delete( &a );
    bi_delete( &b );
    bi_delete( &c );
}


int main( int argc, char ** argv ) 
{
    INIT_TEST();

    // expand() 
    ADD_TEST( test_expand );

    // bi_init()
    ADD_TEST( test_init );

    // bi_delete()
    ADD_TEST( test_delete );

    // bi_set_uint()
    ADD_TEST( test_bi_set_uint );
    
    // bi_copy()
    ADD_TEST( test_bi_copy_single_segment );
    ADD_TEST( test_bi_copy );
    ADD_TEST( test_bi_copy_multiple_segments_to_single_segment );
    ADD_TEST( test_bi_copy_single_segment_to_multiple_segments );

    // bi_add()
    ADD_TEST( test_bi_add );
    ADD_TEST( test_bi_add_sum_larger_than_addends );

    // bi_to_string()
    ADD_TEST( test_bi_to_string_initialized );
    ADD_TEST( test_bi_to_string_one_byte );
    ADD_TEST( test_bi_to_string_four_bytes );
    ADD_TEST( test_bi_to_string_large_value );
    ADD_TEST( test_bi_to_string_small_buffer );

    // integrated
    ADD_TEST( test_fib_1001 );


    TEST_RUN();

}
