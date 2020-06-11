
#ifndef CTEST_LIB
#define CTEST_LIB


#define ASSERT_TRUE( VALUE ) \
    if ( !(VALUE) ) { \
        testcase->status = FAIL; \
        testcase->line = __LINE__; \
        return; \
    } 


#define ASSERT_EQUAL( X, Y ) \
    if ( X != Y ) { \
        testcase->status = FAIL; \
        testcase->line = __LINE__; \
        return; \
    }

#define TEST_CASE( NAME ) \
    void NAME( struct TestCase * testcase )

#define INIT_TEST() \
    struct Test test; \
    init_test( &test ); \

#define ADD_TEST( NAME ) \
    add_test_case( &test, #NAME, NAME );

#define TEST_RUN() \
    run_test( &test );

enum TestStatus {
    NOT_STARTED,
    PASS,
    FAIL
};

struct TestCase {
    const char * name;
    enum TestStatus status;
    unsigned int line;
    void (*test_func)( struct TestCase * );
};

struct Test {
    struct TestCase * test_cases;
    unsigned test_cases_size;
    unsigned n_tests;
};

void init_test( struct Test * );
void add_test_case( struct Test *, char * name, void (*func)( struct TestCase * ) );
void run_test( struct Test * test );


#endif
