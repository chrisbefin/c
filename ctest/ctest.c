
#include <stdlib.h> // malloc(), free(), atoi()
#include <stdio.h>  // printf()
#include <string.h> // strlen()


#include "ctest.h"

#define INITIAL_TEST_CASES_SIZE 10


void init_test_case( struct TestCase * tc, const char * name, void (*func)( struct TestCase * ) )
{
    tc->name = name; 
    tc->test_func = func;
    tc->line = 0;
    tc->status = NOT_STARTED;
}

void init_test( struct Test * ts )
{
    ts->test_cases = malloc( INITIAL_TEST_CASES_SIZE * sizeof( struct TestCase ) );
    ts->test_cases_size = INITIAL_TEST_CASES_SIZE;
    int i=0;
    ts->n_tests = 0;
}

void add_test_case( struct Test * ts, char * name, void (*func)( struct TestCase * ) )
{
    if ( !ts ) {
        return;
    }
    // Expand list of test cases if necessary
    if ( ts->test_cases_size <= ts->n_tests ) {
        struct TestCase * temp = ts->test_cases;
        unsigned prevSize = ts->test_cases_size;
        unsigned newSize = prevSize * 2;

        ts->test_cases = malloc( newSize * sizeof( struct TestCase ) );
        ts->test_cases_size = newSize;
        int i=0;
        for(  ; i<ts->n_tests; ++i ) {
            ts->test_cases[i] = temp[i];
        }

    }
    init_test_case( &(ts->test_cases[ts->n_tests] ), name, func ); 
    ts->n_tests++;
}

void run_test( struct Test * test ) {
    if ( !test ) {
        printf( "NULL TEST\n" );
        return;
    }

    int i;
    int tests_passed = 0;
    const char * pass = "   PASS";
    const char * fail = "***FAIL";
    for ( i=0; i<test->n_tests; ++i ) {
        struct TestCase * testcase = &(test->test_cases[i]);
        testcase->status = PASS;
        (testcase->test_func)( testcase );
        const char * result;
        if ( testcase->status == PASS ) { 
            result = pass;
            tests_passed++;
        } else {
            result = fail;
        }
        printf("TEST %d:  %s  -- %s\n", i, testcase->name, result );
    }
    printf( "----------------------------\n" );
    printf( "%d of %d tests passed\n", tests_passed, test->n_tests );
    printf( "\n" );

    // Report failed test cases
    if ( tests_passed != test->n_tests ) {
        printf( "The following tests failed:\n" );
        for ( i=0; i<test->n_tests; ++i ) {
            struct TestCase * testcase = &(test->test_cases[i]);
            if ( testcase->status != PASS ) {
                printf( "  %d -- %s  failed at line %u\n", i, testcase->name, testcase->line );
            }
        }
        printf( "\n" );
    }
}

