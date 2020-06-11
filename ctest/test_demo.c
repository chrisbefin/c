

/* A brief demo of a basic C unit testing framework
 * To compile this example:
 *
 * > gcc test_demo.c ctest.c test_lib.c -o test_demo
 *
 */

#include <stdio.h>     // printf()
#include "ctest.h"     // Our unit testing library 
#include "test_lib.h"  // The library being tested

TEST_CASE( test_add )
{
    ASSERT_TRUE( add( 1,2 ) == 3 );
    ASSERT_TRUE( add( 0,2 ) == 2 );
    ASSERT_EQUAL( add( 2,0 ), 2 );
    ASSERT_EQUAL( add( -1,10 ), 9 );
}

TEST_CASE( test_subtract )
{
    ASSERT_TRUE( subtract( 2,2 ) == 0 );
    ASSERT_TRUE( subtract( 4,1 ) == 3 );
}

int main( int argc, char ** argv ) 
{
    INIT_TEST();

    ADD_TEST( test_add );
    ADD_TEST( test_subtract );

    TEST_RUN();

}
