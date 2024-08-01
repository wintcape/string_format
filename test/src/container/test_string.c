/**
 * @file container/test_string.c
 * @brief Implementation of the container/test_string header.
 * (see container/test_string.h for additional details)
 */
#include "container/test_string.h"

#include "test/expect.h"

#include "platform/memory.h"

u8
test_string_allocate_and_free
( void )
{
    // u64 global_amount_allocated;
    // u64 string_amount_allocated;
    // u64 global_allocation_count;

    // u64 global_amount_allocated_;
    // u64 string_amount_allocated_;
    // u64 global_allocation_count_;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // string_amount_allocated = memory_amount_allocated ( MEMORY_TAG_STRING );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    const char* hello = "Hello world!";

    ////////////////////////////////////////////////////////////////////////////
    // Start test.
    
    // TEST 1: string_allocate_from.

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated_ = memory_amount_allocated ( MEMORY_TAG_ALL );
    // string_amount_allocated_ = memory_amount_allocated ( MEMORY_TAG_STRING );
    // global_allocation_count_ = MEMORY_ALLOCATION_COUNT;

    char* string = string_allocate_from ( hello );

    // // TEST 1.1: string_allocate_from performed **one** memory allocation.
    // EXPECT_EQ ( global_allocation_count_ + 1 , MEMORY_ALLOCATION_COUNT );

    // // TEST 1.2: string_allocate_from allocated the correct number of bytes with the correct memory tag (length of string + terminator + u64 (used internally to store string length to free)).
    // EXPECT_EQ ( global_amount_allocated_ + _string_length ( hello ) + sizeof ( char ) + sizeof ( u64 ) , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( string_amount_allocated_ + _string_length ( hello ) + sizeof ( char ) + sizeof ( u64 ) , memory_amount_allocated ( MEMORY_TAG_STRING ) );

    // TEST 1.3: String created via string_allocate_from has identical length to the string it was created from.
    EXPECT_EQ ( _string_length ( hello ) , _string_length ( string ) );

    // TEST 1.4: String created via string_allocate_from has identical characters to the string it was created from.
    EXPECT ( memory_equal ( string , hello , _string_length ( string ) ) );

    // TEST 2: string_free.

    // // TEST 2.1: string_free restores the global allocator state.
    string_free ( string );
    // EXPECT_EQ ( global_amount_allocated_ , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( string_amount_allocated_ , memory_amount_allocated ( MEMORY_TAG_STRING ) );
    // EXPECT_EQ ( global_allocation_count_ , MEMORY_ALLOCATION_COUNT );

    // // TEST 2.2: string_free does not modify the global allocator state if no string is provided.
    string_free ( 0 );
    // EXPECT_EQ ( global_amount_allocated_ , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( string_amount_allocated_ , memory_amount_allocated ( MEMORY_TAG_STRING ) );
    // EXPECT_EQ ( global_allocation_count_ , MEMORY_ALLOCATION_COUNT );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( string_amount_allocated , memory_amount_allocated ( MEMORY_TAG_STRING ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_string_create_and_destroy
( void )
{
    // u64 global_amount_allocated;
    // u64 array_amount_allocated;
    // u64 global_allocation_count;

    // u64 global_amount_allocated_;
    // u64 array_amount_allocated_;
    // u64 global_allocation_count_;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    const char* hello = "Hello world!";

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: string_create.

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated_ = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated_ = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // global_allocation_count_ = MEMORY_ALLOCATION_COUNT;

    char* string = string_create ();

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string );

    // // TEST 1.1: string_create performed **one** memory allocation.
    // EXPECT_EQ ( global_allocation_count_ + 1 , MEMORY_ALLOCATION_COUNT );

    // // TEST 1.2: string_create allocated the correct number of bytes with the correct memory tag (array is used internally to represent a resizable string).
    // EXPECT_EQ ( global_amount_allocated_ + array_size ( string ) , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated_ + array_size ( string ) , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );

    // TEST 1.3: String created via string_create has 0 length.
    EXPECT_EQ ( 0 , string_length ( string ) );

    // TEST 1.4: String created via string_create has a null terminator.
    EXPECT_EQ ( 0 , *string );

    // // TEST 1.5: string_destroy restores the global allocator state.
    string_destroy ( string );
    // EXPECT_EQ ( global_amount_allocated_ , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated_ , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( global_allocation_count_ , MEMORY_ALLOCATION_COUNT );

    // TEST 2: string_create_from.

    string = string_create_from ( hello );

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string );

    // // TEST 2.1: string_create_from performed **one** memory allocation.
    // EXPECT_EQ ( global_allocation_count_ + 1 , MEMORY_ALLOCATION_COUNT );

    // // TEST 2.2: string_create_from allocated the correct number of bytes with the correct memory tag (array is used internally to represent a resizable string).
    // EXPECT_EQ ( array_amount_allocated_ + array_size ( string ) , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );

    // TEST 2.3: String created via string_create_from has identical length to the string it was created from.
    EXPECT_EQ ( _string_length ( hello ) , string_length ( string ) );
    
    // TEST 2.4: String created via string_create_from has identical characters to the string it was created from.
    EXPECT ( memory_equal ( string , hello , string_length ( string ) + 1 ) );

    // TEST 3: string_copy.

    char* copy = _string_copy ( string );

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , copy );

    // // TEST 3.1: string_copy performed **one** memory allocation.
    // EXPECT_EQ ( global_allocation_count + 1 + 1 , MEMORY_ALLOCATION_COUNT );

    // // TEST 3.2: string_copy allocated the correct number of bytes with the correct memory tag (array is used internally to represent a resizable string).
    // EXPECT_EQ ( array_amount_allocated + array_size ( string ) + array_size ( string_copy ) , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    
    // TEST 3.3: String created via string_copy has identical length to the string it was created from.
    EXPECT_EQ ( string_length ( copy ) , string_length ( string ) );
    
    // TEST 3.4: String created via string_copy has identical characters to the string it was created from.
    EXPECT ( memory_equal ( string , copy , string_length ( string ) + 1 ) );

    // // TEST 3.5: string_destroy restores the global allocator state.
    string_destroy ( copy );
    string_destroy ( string );
    // EXPECT_EQ ( global_amount_allocated_ , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated_ , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( global_allocation_count_ , MEMORY_ALLOCATION_COUNT );

    // TEST 4: string_create handles invalid argument.

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated_ = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated_ = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // global_allocation_count_ = MEMORY_ALLOCATION_COUNT;

    // TEST 4.1: string_create logs an error and fails if provided capacity is invalid.
    LOGWARN ( "The following error is intentionally triggered by a test:" );
    EXPECT_EQ ( 0 , _string_create ( 0 ) );

    // // TEST 4.2: string_create does not allocate memory on failure.
    // EXPECT_EQ ( global_amount_allocated_ , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated_ , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( global_allocation_count_ , MEMORY_ALLOCATION_COUNT );

    // // TEST 5: string_destroy handles invalid argument.

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated_ = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated_ = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // global_allocation_count_ = MEMORY_ALLOCATION_COUNT;

    // // TEST 5.1: string_destroy does not modify the global allocator state if no string is provided.
    string_destroy ( 0 );
    // EXPECT_EQ ( global_amount_allocated_ , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated_ , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( global_allocation_count_ , MEMORY_ALLOCATION_COUNT );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_string_append
( void )
{
    // u64 global_amount_allocated;
    // u64 array_amount_allocated;
    // u64 string_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // string_amount_allocated = memory_amount_allocated ( MEMORY_TAG_STRING );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    const char* to_push = "push";
    u64 op_count = 100000;
    char* string = string_create ();

    // // Avoid allocating too much memory.
    // while ( op_count && op_count * _string_length ( to_push ) + 1 > memory_amount_free () / 2 - KiB ( 1 ) )
    // {
    //     op_count >>= 1;
    // }

    char* old_string = string_allocate ( op_count * _string_length ( to_push ) + 1 );

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string );
    EXPECT_NEQ ( 0 , old_string );

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    for ( u64 i = 0; i < op_count; ++i )
    {
        // Copy string state prior to performing the operation being tested.
        const u64 old_length = string_length ( string );
        memory_copy ( old_string , string , old_length );

        _string_append ( string , to_push );

        // Verify there was no memory error prior to the test.
        EXPECT_NEQ ( 0 , string );

        // TEST 1: string_append increases the length of the string by the length of the string being appended.
        EXPECT_EQ ( old_length , string_length ( string ) - _string_length ( to_push ) );
        
        // TEST 2: string_append appends the correct characters to the end of the string.
        EXPECT ( memory_equal ( to_push , &string[ old_length ] , _string_length ( to_push ) ) );

        // TEST 3: string_append leaves the remainder of the string unmodified.
        EXPECT ( memory_equal ( string , old_string , old_length ) );
    }
    
    // End test.
    ////////////////////////////////////////////////////////////////////////////

    string_free ( old_string );
    string_destroy ( string );

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( string_amount_allocated , memory_amount_allocated ( MEMORY_TAG_STRING ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_string_insert_and_remove
( void )
{
    // u64 global_amount_allocated;
    // u64 array_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    const char* to_insert[] = { "He" , "llo " , "world" , "!" };
    const char* insert1     =          "llo "                  ;
    const char* insert2     =   "He"   "llo "                  ;
    const char* insert3     =   "He"   "llo "             "!"  ;
    const char* full        =   "He"   "llo "   "world"   "!"  ;
    const char* remove1     =   "He"   "llo "             "!"  ;
    const char* remove2     =   "He"                      "!"  ;
    const char* remove3     =                             "!"  ;
    char* string1 = string_create ();
    char* string2 = string_create ();
    char* string;

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string1 );
    EXPECT_NEQ ( 0 , string2 );

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: string_insert handles invalid arguments.

    // TEST 1.1: string_insert logs an error if the index is invalid.
    LOGWARN ( "The following error is intentionally triggered by a test:" );
    string = string1;
    _string_insert ( string1 , string_length ( string1 ) + 1 , to_insert[ 0 ] );

    // TEST 1.2: string_insert does not perform memory allocation if the index is invalid.
    EXPECT_EQ ( string , string1 );

    // TEST 1.3: string_insert does not modify string length if the index is invalid.
    EXPECT_EQ ( 0 , string_length ( string1 ) );

    // TEST 2: string_insert.

    _string_insert ( string1 , 0 , to_insert[ 1 ] );

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string1 );

    // TEST 2.1: string_insert increases the length of the string by the length of the string being inserted.
    EXPECT_EQ ( _string_length ( insert1 ) , string_length ( string1 ) );

    // TEST 2.2: string_insert inserts the correct number of characters to the string at the correct index, leaving the remainder of the string unmodified.
    EXPECT ( memory_equal ( string1 , insert1 , string_length ( string1 ) ) );
    
    _string_insert ( string1 , 0 , to_insert[ 0 ] );

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string1 );

    // TEST 2.3: string_insert increases the length of the string by the length of the string being inserted.
    EXPECT_EQ ( _string_length ( insert2 ) , string_length ( string1 ) );

    // TEST 2.4: string_insert inserts the correct number of characters to the string at the correct index, leaving the remainder of the string unmodified.
    EXPECT ( memory_equal ( string1 , insert2 , string_length ( string1 ) ) );

    _string_insert ( string1 , string_length ( string1 ) , to_insert[ 3 ] );

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string1 );

    // TEST 2.5: string_insert increases the length of the string by the length of the string being inserted.
    EXPECT_EQ ( _string_length ( insert3 ) , string_length ( string1 ) );

    // TEST 2.6: string_insert inserts the correct number of characters to the string at the correct index, leaving the remainder of the string unmodified.
    EXPECT ( memory_equal ( string1 , insert3 , string_length ( string1 ) ) );

    _string_insert ( string1 , _string_length ( to_insert[ 0 ] ) + _string_length ( to_insert[ 1 ] ) , to_insert[ 2 ] );

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string1 );

   // TEST 2.7: string_insert increases the length of the string by the length of the string being inserted.
    EXPECT_EQ ( _string_length ( full ) , string_length ( string1 ) );

    // TEST 2.8: string_insert inserts the correct number of characters to the string at the correct index, leaving the remainder of the string unmodified.
    EXPECT ( memory_equal ( string1 , full , string_length ( string1 ) ) );

    // TEST 2.9: string_insert yields the same results as string_append when inserting at the end of the array.
    _string_append ( string2 , to_insert[ 0 ] );
    _string_append ( string2 , to_insert[ 1 ] );
    _string_append ( string2 , to_insert[ 2 ] );
    _string_append ( string2 , to_insert[ 3 ] );
    EXPECT_EQ ( string_length ( string2 ) , string_length ( string1 ) );
    EXPECT ( memory_equal ( string1 , string2 , string_length ( string1 ) + 1 ) );

    // TEST 3: string_remove handles invalid arguments.

    LOGWARN ( "The following errors are intentionally triggered by a test:" );

    // TEST 3.1: string_remove logs an error if the index is invalid.
    string = string1;
    string_remove ( string1 , string_length ( string1 ) , 1 );

    // TEST 3.2: string_remove does not perform memory allocation if the index is invalid* (current implementation doesn't allocate in general either, but I don't need to test for that).
    EXPECT_EQ ( string , string1 );

    // TEST 3.3: string_remove does not modify string length if the index is invalid.
    EXPECT_EQ ( string_length ( string2 ) , string_length ( string1 ) );

    // TEST 3.4: string_remove does not modify string characters if the index is invalid.
    EXPECT ( memory_equal ( string1 , string2 , string_length ( string1 ) ) );

    // TEST 3.5: string_remove logs an error if the index is invalid.
    string = string1;
    string_remove ( string1 , string_length ( string1 ) + 1 , 0 );

    // TEST 3.6: string_remove does not perform memory allocation if the index is invalid* (current implementation doesn't allocate in general either, but I don't need to test for that).
    EXPECT_EQ ( string , string1 );

    // TEST 3.7: string_remove does not modify string length if the index is invalid.
    EXPECT_EQ ( string_length ( string2 ) , string_length ( string1 ) );

    // TEST 3.8: string_remove does not modify string characters if the index is invalid.
    EXPECT ( memory_equal ( string1 , string2 , string_length ( string1 ) ) );

    // TEST 3.9: string_remove logs an error if the index is invalid.
    string = string1;
    string_remove ( string1 , string_length ( string1 ) - 5 , 6 );

    // TEST 3.10: string_remove does not perform memory allocation if the index is invalid* (current implementation doesn't allocate in general either, but I don't need to test for that).
    EXPECT_EQ ( string , string1 );

    // TEST 3.11: string_remove does not modify string length if the index is invalid.
    EXPECT_EQ ( string_length ( string2 ) , string_length ( string1 ) );

    // TEST 3.12: string_remove does not modify string characters if the index is invalid.
    EXPECT ( memory_equal ( string1 , string2 , string_length ( string1 ) ) );

    // TEST 3.13: string_remove does not fail when count is 0.
    string = string1;
    string_remove ( string1 , 0 , 0 );

    // TEST 3.14: string_remove does not perform memory allocation if the count is 0* (current implementation doesn't allocate in general either, but I don't need to test for that).
    EXPECT_EQ ( string , string1 );

    // TEST 3.15: string_remove does not modify string length if the count is 0.
    EXPECT_EQ ( string_length ( string2 ) , string_length ( string1 ) );

    // TEST 3.16: string_remove does not modify string characters if the count is 0.
    EXPECT ( memory_equal ( string1 , string2 , string_length ( string1 ) ) );

    // TEST 4: string_remove.

    string_remove ( string1 , _string_length ( to_insert[ 0 ] ) + _string_length ( to_insert[ 1 ] ) , _string_length ( to_insert[ 2 ] ) );
    
    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string1 );

    // TEST 4.1: string_remove decreases the length of the string by the number of characters being removed.
    EXPECT_EQ ( _string_length ( remove1 )  , string_length ( string1 ) );
    
    // TEST 4.2: string_remove removes the correct number of characters from the string at the correct index, leaving the rest of the string unmodified.
    EXPECT ( memory_equal ( string1 , remove1 , string_length ( string1 ) ) );

    string_remove ( string1 , _string_length ( to_insert[ 0 ] ) , _string_length ( to_insert[ 1 ] ) );
    
    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string1 );

    // TEST 4.3: string_remove decreases the length of the string by the number of characters being removed.
    EXPECT_EQ ( _string_length ( remove2 ) , string_length ( string1 ) );
    
    // TEST 4.4: string_remove removes the correct number of characters from the string at the correct index, leaving the rest of the string unmodified.
    EXPECT ( memory_equal ( string1 , remove2 , string_length ( string1 ) ) );
    
    string_remove ( string1 , 0 , _string_length ( to_insert[ 0 ] ) );
    
    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string1 );

    // TEST 4.5: string_remove decreases the length of the string by the number of characters being removed.
    EXPECT_EQ ( _string_length ( remove3 )  , string_length ( string1 )  );
    
    // TEST 4.6: string_remove removes the correct number of characters from the string at the correct index, leaving the rest of the string unmodified.
    EXPECT ( memory_equal ( string1 , remove3 , string_length ( string1 ) ) );
    
    string_remove ( string1 , 0 , string_length ( string1 ) );
    
    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string1 );

    // TEST 4.7: string_remove decreases the length of the string by the number of characters being removed.
    EXPECT_EQ ( 0 , string_length ( string1 ) );

    // TEST 4.8: string_remove removes the correct number of characters from the string at the correct index, leaving the rest of the string unmodified.
    EXPECT_EQ ( 0 , *string1 );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    string_destroy ( string1 );
    string_destroy ( string2 );

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_string_insert_and_remove_random
( void )
{
    // u64 global_amount_allocated;
    // u64 array_amount_allocated;
    // u64 string_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // string_amount_allocated = memory_amount_allocated ( MEMORY_TAG_STRING );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    u64 op_count = 100000;
    char* string = string_create ();

    // // Avoid allocating too much memory.
    // while ( op_count && op_count + 1 > memory_amount_free () / 2 - KiB ( 1 ) )
    // {
    //     op_count >>= 1;
    // }

    char* old_string = string_allocate ( op_count + 1 );

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string );
    EXPECT_NEQ ( 0 , old_string );

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    for ( u64 i = 0; i < op_count; ++i )
    {
        // Copy the string state prior to performing the operation being tested.
        const u64 old_length = string_length ( string );
        memory_copy ( old_string , string , old_length );

        // Insert a random non-zero character at a random index within the string.
        const char to_insert[] = { random2 ( 1 , 255 ) , 0 };
        const u64 insert_index = random2 ( 0 , old_length );
        _string_insert ( string , insert_index , to_insert );

        // Verify there was no memory error prior to the test.
        EXPECT_NEQ ( 0 , string );

        // TEST 1: string_insert increases the length of the string by the length of the string being inserted.
        EXPECT_EQ ( old_length + 1 , string_length ( string ) );

        // TEST 2: string_insert inserts the correct character into the string at the correct index.
        EXPECT_EQ ( *to_insert , string[ insert_index ] );

        // TEST 3: string_insert leaves the remainder of the string unmodified.
        if ( !insert_index )
        {
            EXPECT ( memory_equal ( &string[ 1 ] , &old_string[ 0 ] , old_length ) );
        }
        else if ( insert_index == old_length )
        {
            EXPECT ( memory_equal ( &string[ 0 ] , &old_string[ 0 ] , old_length ) );
        }
        else
        {
            EXPECT ( memory_equal ( &string[ 0 ] , &old_string[ 0 ] , ( insert_index - 1 ) ) );
            EXPECT ( memory_equal ( &string[ insert_index + 1 ] , &old_string[ insert_index ] , ( old_length - insert_index ) ) );
        }
    }

    while ( string_length ( string ) )
    {
        // Copy the string state prior to performing the operation being tested.
        const u64 old_length = string_length ( string );
        memory_copy ( old_string , string , old_length );

        // Remove a random number of characters from a random index within the string.
        const u64 remove_cap = ( old_length > 1000 ) ? old_length / 100 : old_length;
        const u64 remove_count = random2 ( 1 , remove_cap );
        const u64 remove_index = random2 ( 0 , old_length - remove_cap );
        string_remove ( string , remove_index , remove_count );

        // Verify there was no memory error prior to the test.
        EXPECT_NEQ ( 0 , string );

        // TEST 4: string_insert decreases the length of the string by the number of characters being removed.
        EXPECT_EQ ( old_length - remove_count , string_length ( string ) );

        // TEST 5: string_remove removes the correct number of characters from the string at the correct index, leaving the rest of the string unmodified.
        if ( !remove_index )
        {
            EXPECT ( memory_equal ( &string[ 0 ] , &old_string[ remove_count ] , string_length ( string ) ) );
        }
        else if ( remove_index == old_length - 1 )
        {
            EXPECT ( memory_equal ( &string[ 0 ] , &old_string[ 0 ] , string_length ( string ) ) );
        }
        else
        {
            EXPECT ( memory_equal ( &string[ 0 ] , &old_string[ 0 ] , remove_index - 1 ) );
            EXPECT ( memory_equal ( &string[ remove_index ] , &old_string[ remove_index + remove_count ] , string_length ( string ) - remove_index ) );
        }
    }

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    string_free ( old_string );
    string_destroy ( string );

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( string_amount_allocated , memory_amount_allocated ( MEMORY_TAG_STRING ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_string_empty
( void )
{
    // u64 global_amount_allocated;
    // u64 array_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    char* string = string_create ();

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string );

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: string_empty succeeds on empty null-terminated string.
    EXPECT ( string_empty ( "" ) );

    // TEST 2: string_empty fails on non-empty null-terminated string.
    EXPECT_NOT ( string_empty ( "a" ) );

    // TEST 3: string_empty succeeds on empty string created with the _string_create class of functions.
    EXPECT_EQ ( 0 , string_length ( string ) ); // Verify empty prior to testing.
    EXPECT ( string_empty ( string ) );

    // TEST 4: string_empty fails on non-empty string created with the _string_create class of functions.
    string_append ( string , "a" , _string_length ( "a" ) );
    EXPECT_NEQ ( 0 , string_length ( string ) ); // Verify non-empty prior to testing.
    EXPECT_NOT ( string_empty ( string ) );

    // TEST 5: string_empty fails if the provided string is null.
    EXPECT_NOT ( string_empty ( 0 ) );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    string_destroy ( string );

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_string_truncate
( void )
{
    // u64 global_amount_allocated;
    // u64 array_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    string_t* string = string_create ();

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string );

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: string_truncate succeeds on empty string.
    string_clear ( string );
    string_truncate ( string , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "" , string_length ( string ) ) );

    // TEST 2: string_truncate does not modify the string if the provided length is greater than (or equal to) than the length of the string.
    string_clear ( string );
    _string_append ( string , "Hello world!" );
    string_truncate ( string , string_length ( string ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello world!" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello world!" , string_length ( string ) ) );
    // ( x2 )
    string_clear ( string );
    _string_append ( string , "Hello world!" );
    string_truncate ( string , string_length ( string ) + 1 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello world!" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello world!" , string_length ( string ) ) );

    // TEST 3: string_truncate truncates the string if the provided length is less than the length of the string.
    string_truncate ( string , _string_length ( "Hello" ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello" , string_length ( string ) ) );

    // TEST 4: string_truncate clears the string if the provided index is zero.
    string_truncate ( string , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "" , string_length ( string ) ) );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    string_destroy ( string );

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_string_trim
( void )
{
    // u64 global_amount_allocated;
    // u64 array_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    char* string = string_create ();

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string );

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: __string_trim (resizable string).

    // TEST 1.1: __string_trim does not fail on empty string.
    _string_append ( string , "" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_trim ( string );
    EXPECT ( memory_equal ( string , "" , _string_length ( "" ) ) );
    string_clear ( string );

    // TEST 1.2: __string_trim reduces a string of only whitespace to empty.
    _string_append ( string , "       \t\n\r        " );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_trim ( string );
    EXPECT ( memory_equal ( string , "" , _string_length ( "" ) ) );
    string_clear ( string );

    // TEST 1.3: __string_trim trims a string with leading whitespace.
    _string_append ( string , "\n\t\t\t  <-- Trim this off -->" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_trim ( string );
    EXPECT ( memory_equal ( string , "<-- Trim this off -->" , _string_length ( "<-- Trim this off -->" ) ) );
    string_clear ( string );

    // TEST 1.4: __string_trim trims a string with trailing whitespace.
    _string_append ( string , "<-- Trim this off -->  \t\t\t\n" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_trim ( string );
    EXPECT ( memory_equal ( string , "<-- Trim this off -->" , _string_length ( "<-- Trim this off -->" ) ) );
    string_clear ( string );

    // TEST 1.5: __string_trim trims a string with both leading and trailing whitespace.
    _string_append ( string , "\n\t\t\t  <-- Trim this off -->  \t\t\t\n" );
    __string_trim ( string );
    EXPECT ( memory_equal ( string , "<-- Trim this off -->" , _string_length ( "<-- Trim this off -->" ) ) );
    string_clear ( string );

    // TEST 2: string_trim (fixed-length string).

    // TEST 2.1: string_trim does not fail on empty string.
    _string_append ( string , "" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_trim ( string , string );
    EXPECT ( memory_equal ( string , "" , _string_length ( "" ) ) );
    string_clear ( string );

    // TEST 2.2: string_trim reduces a string of only whitespace to empty.
    _string_append ( string , "       \t\n\r        " );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_trim ( string , string );
    EXPECT ( memory_equal ( string , "" , _string_length ( "" ) ) );
    string_clear ( string );

    // TEST 2.3: string_trim trims a string with leading whitespace.
    _string_append ( string , "\n\t\t\t  <-- Trim this off -->" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_trim ( string , string );
    EXPECT ( memory_equal ( string , "<-- Trim this off -->" , _string_length ( "<-- Trim this off -->" ) ) );
    string_clear ( string );

    // TEST 2.4: string_trim trims a string with trailing whitespace.
    _string_append ( string , "<-- Trim this off -->  \t\t\t\n" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_trim ( string , string );
    EXPECT ( memory_equal ( string , "<-- Trim this off -->" , _string_length ( "<-- Trim this off -->" ) ) );
    string_clear ( string );

    // TEST 2.5: string_trim trims a string with both leading and trailing whitespace.
    _string_append ( string , "\n\t\t\t  <-- Trim this off -->  \t\t\t\n" );
    _string_trim ( string , string );
    EXPECT ( memory_equal ( string , "<-- Trim this off -->" , _string_length ( "<-- Trim this off -->" ) ) );
    string_clear ( string );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    string_destroy ( string );

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_string_contains
( void )
{
    const char* search1 = "This is the string to search.";
    const char search2[] = { 'H' , 'e' , 'l' , 'l' , 'o' , 0 , 'w' , 'o' , 'r' , 'l' , 'd' };
    const u64 search1_length = _string_length ( search1 );
    const u64 search2_length = sizeof ( search2 );
    u64 index;

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: string_contains succeeds on first index when length of substring to find is 0.
    EXPECT ( string_contains ( search1 , search1_length , "" , _string_length ( "" ) , false , &index ) );
    EXPECT_EQ ( 0 , index );

    // TEST 2: string_contains (reverse) succeeds on final index when length of substring to find is 0.
    EXPECT ( string_contains ( search1 , search1_length , "" , _string_length ( "" ) , true , &index ) );
    EXPECT_EQ ( search1_length - 1 , index );

    // TEST 3: string_contains succeeds on first index when length of substring to find is 0 and strings are not null-terminated.
    EXPECT ( string_contains ( search2 , search2_length , "" , _string_length ( "" ) , false , &index ) );
    EXPECT_EQ ( 0 , index );

    // TEST 4: string_contains (reverse) succeeds on final index when length of substring to find is 0 and strings are not null-terminated.
    EXPECT ( string_contains ( search2 , search2_length , "" , _string_length ( "" ) , true , &index ) );
    EXPECT_EQ ( search2_length - 1 , index );

    // TEST 5: string_contains succeeds on first index when substring to find is the string to search itself.
    EXPECT ( string_contains ( search1 , search1_length , search1 , search1_length , false , &index ) );
    EXPECT_EQ ( 0 , index );
    
    // TEST 6: string_contains (reverse) succeeds on first index when substring to find is the string to search itself.
    EXPECT ( string_contains ( search1 , search1_length , search1 , search1_length , true , &index ) );
    EXPECT_EQ ( 0 , index );

    // TEST 7: string_contains succeeds on first index when substring to find is the string to search itself and strings are not null-terminated.
    EXPECT ( string_contains ( search2 , search2_length , search2 , search2_length , false , &index ) );
    EXPECT_EQ ( 0 , index );
    
    // TEST 8: string_contains (reverse) succeeds on first index when substring to find is the string to search itself and strings are not null-terminated.
    EXPECT ( string_contains ( search2 , search2_length , search2 , search2_length , true , &index ) );
    EXPECT_EQ ( 0 , index );

    // TEST 9: string_contains fails if length of substring to find exceeds length of string to search.
    EXPECT_NOT ( string_contains ( search1 , search1_length , "This cannot be a substring because it is longer than both search1 and search2." , _string_length ( "This cannot be a substring because it is longer than both search1 and search2." ) , false , &index ) );
    EXPECT_NOT ( string_contains ( search1 , search1_length , "This cannot be a substring because it is longer than both search1 and search2." , _string_length ( "This cannot be a substring because it is longer than both search1 and search2." ) , true , &index ) );
    EXPECT_NOT ( string_contains ( search2 , search2_length , "This cannot be a substring because it is longer than both search1 and search2." , _string_length ( "This cannot be a substring because it is longer than both search1 and search2." ) , false , &index ) );
    EXPECT_NOT ( string_contains ( search2 , search2_length , "This cannot be a substring because it is longer than both search1 and search2." , _string_length ( "This cannot be a substring because it is longer than both search1 and search2." ) , true , &index ) );
    
    // TEST 10: string_contains succeeds in locating the first character in the string to search.
    EXPECT ( string_contains ( search1 , search1_length , "T" , _string_length ( "T" ) , false , &index ) );
    EXPECT_EQ ( 0 , index );

    // TEST 11: string_contains (reverse) succeeds in locating the first character in the string to search.
    EXPECT ( string_contains ( search1 , search1_length , "T" , _string_length ( "T" ) , true , &index ) );
    EXPECT_EQ ( 0 , index );

    // TEST 12: string_contains succeeds in locating the first character in the string to search and strings are not null-terminated.
    EXPECT ( string_contains ( search2 , search2_length , "H" , _string_length ( "H" ) , false , &index ) );
    EXPECT_EQ ( 0 , index );

    // TEST 13: string_contains (reverse) succeeds in locating the first character in the string to search and strings are not null-terminated.
    EXPECT ( string_contains ( search2 , search2_length , "H" , _string_length ( "H" ) , true , &index ) );
    EXPECT_EQ ( 0 , index );

    // TEST 14: string_contains succeeds in locating the final character in the string to search.
    EXPECT ( string_contains ( search1 , search1_length , "." , _string_length ( "." ) , false , &index ) );
    EXPECT_EQ ( search1_length - 1 , index );

    // TEST 15: string_contains (reverse) succeeds in locating the final character in the string to search.
    EXPECT ( string_contains ( search1 , search1_length , "." , _string_length ( "." ) , true , &index ) );
    EXPECT_EQ ( search1_length - 1 , index );

    // TEST 16: string_contains succeeds in locating the final character in the string to search and strings are not null-terminated.
    EXPECT ( string_contains ( search2 , search2_length , "d" , _string_length ( "d" ) , false , &index ) );
    EXPECT_EQ ( search2_length - 1 , index );

    // TEST 17: string_contains (reverse) succeeds in locating the final character in the string to search and strings are not null-terminated.
    EXPECT ( string_contains ( search2 , search2_length , "d" , _string_length ( "d" ) , true , &index ) );
    EXPECT_EQ ( search2_length - 1 , index );

    // TEST 18: string_contains fails if the substring to find does not occur within the string to search.
    EXPECT_NOT ( string_contains ( search1 , search1_length , "search. " , _string_length ( "search. " ) , false , &index ) );
    EXPECT_NOT ( string_contains ( search1 , search1_length , "search. " , _string_length ( "search. " ) , true , &index ) );
    EXPECT_NOT ( string_contains ( search2 , search2_length , string_char ( 'r' , 'l' , 'd' ) , sizeof ( string_char ( 'r' , 'l' , 'd' ) ) , false , &index ) );
    EXPECT_NOT ( string_contains ( search2 , search2_length , string_char ( 'r' , 'l' , 'd' ) , sizeof ( string_char ( 'r' , 'l' , 'd' ) ) , true , &index ) );

    // TEST 19: string_contains succeeds in locating a substring which occurs once in the string to search.
    EXPECT ( string_contains ( search1 , search1_length , "string" , _string_length ( "string" ) , false , &index ) );
    EXPECT_EQ ( 12 , index );

    // TEST 20: string_contains (reverse) succeeds in locating a substring which occurs once in the string to search.
    EXPECT ( string_contains ( search1 , search1_length , "string" , _string_length ( "string" ) , false , &index ) );
    EXPECT_EQ ( 12 , index );

    // TEST 21: string_contains succeeds in locating a substring which occurs once in the string to search and strings are not null-terminated.
    EXPECT ( string_contains ( search2 , search2_length , "wor" , _string_length ( "wor" ) , false , &index ) );
    EXPECT_EQ ( 6 , index );

    // TEST 22: string_contains (reverse) succeeds in locating a substring which occurs once in the string to search and strings are not null-terminated.
    EXPECT ( string_contains ( search2 , search2_length , "wor" , _string_length ( "wor" ) , true , &index ) );
    EXPECT_EQ ( 6 , index );

    // TEST 23: string_contains succeeds in locating the first occurrence of a substring which occurs more than once in the string to search.
    EXPECT ( string_contains ( search1 , search1_length , "s" , _string_length ( "s" ) , false , &index ) );
    EXPECT_EQ ( 3 , index );

    // TEST 24: string_contains (reverse) succeeds in locating the final occurrence of a substring which occurs more than once in the string to search.
    EXPECT ( string_contains ( search1 , search1_length , "s" , _string_length ( "s" ) , true , &index ) );
    EXPECT_EQ ( 22 , index );

    // TEST 25: string_contains succeeds in locating the first occurrence of a substring which occurs more than once in the string to search and strings are not null-terminated.
    EXPECT ( string_contains ( search2 , search2_length , "l" , _string_length ( "l" ) , false , &index ) );
    EXPECT_EQ ( 2 , index );

    // TEST 26: string_contains (reverse) succeeds in locating the final occurrence of a substring which occurs more than once in the string to search and strings are not null-terminated.
    EXPECT ( string_contains ( search2 , search2_length , "l" , _string_length ( "l" ) , true , &index ) );
    EXPECT_EQ ( 9 , index );

    // TEST 27: string_contains correctly handles zero-bytes when dealing with strings that are not null-terminated.
    EXPECT ( string_contains ( search2 , search2_length , ( char[] ){ 'o' , 0 , 'w' } , sizeof ( ( char[] ){ 'o' , 0 , 'w' } ) , false , &index ) );
    EXPECT_EQ ( 4 , index );

    // TEST 28: string_contains (reverse) correctly handles zero-bytes when dealing with strings that are not null-terminated.
    EXPECT ( string_contains ( search2 , search2_length ,  ( char[] ){ 'o' , 0 , 'w' } , sizeof ( ( char[] ){ 'o' , 0 , 'w' } ) , true , &index ) );
    EXPECT_EQ ( 4 , index );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    return true;
}

u8
test_string_reverse
( void )
{
    char string[ 11 ];

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: string_reverse does not fail on an empty string.
    memory_copy ( string , "" , _string_length ( "" ) + 1 );
    _string_reverse ( string );
    EXPECT_EQ ( _string_length ( "" ) , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "" , _string_length ( string ) ) );

    // TEST 2: string_reverse does not fail on a single-character string.
    memory_copy ( string , "$" , _string_length ( "$" ) + 1 );
    _string_reverse ( string );
    EXPECT_EQ ( _string_length ( "$" ) , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "$" , _string_length ( string ) ) );

    // TEST 3: string_reverse correctly reverses an input string with more than one character.
    memory_copy ( string , "0123456789" , _string_length ( "0123456789" ) + 1 );
    _string_reverse ( string );
    EXPECT_EQ ( _string_length ( "9876543210" ) , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "9876543210" , _string_length ( string ) ) );
    // (reverse it again)
    _string_reverse ( string );
    EXPECT_EQ ( _string_length ( "0123456789" ) , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "0123456789" , _string_length ( string ) ) );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    return true;
}

u8
test_string_replace
( void )
{
    // u64 global_amount_allocated;
    // u64 array_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    const char* to_replace = "\r\n";
    char* string = string_create ();

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string );

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: string_replace does not fail on an empty string.
    EXPECT_EQ ( _string_length ( "" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "" , string_length ( string ) + 1 ) );
    _string_replace ( string , "\r\n" , "    " );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "" , string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 2: string_replace does not modify the string if the substring to remove and replacement substring are identical.
    _string_append ( string , "Replace\r\nall\r\nnewlines\r\nwith\r\n4\r\nspaces.\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n" );
    _string_replace ( string , to_replace , to_replace );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Replace\r\nall\r\nnewlines\r\nwith\r\n4\r\nspaces.\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Replace\r\nall\r\nnewlines\r\nwith\r\n4\r\nspaces.\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n" , string_length ( string ) + 1 ) );

    // TEST 3: string_replace does not modify the string if the substring to remove and replacement substring have identical size and characters.
    _string_replace ( string , "\r\n" , "\r\n" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Replace\r\nall\r\nnewlines\r\nwith\r\n4\r\nspaces.\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Replace\r\nall\r\nnewlines\r\nwith\r\n4\r\nspaces.\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n" , string_length ( string ) + 1 ) );

    // TEST 4: string_replace does not modify the string if the substring to remove does not occur within it.
    _string_replace ( string , "Not found." , "    " );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Replace\r\nall\r\nnewlines\r\nwith\r\n4\r\nspaces.\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Replace\r\nall\r\nnewlines\r\nwith\r\n4\r\nspaces.\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n" , string_length ( string ) + 1 ) );

    // TEST 5: string_replace replaces all instances of the substring to remove within the string with the replacement substring.
    _string_replace ( string , "\r\n" , "    " );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Replace    all    newlines    with    4    spaces.                                            " ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Replace    all    newlines    with    4    spaces.                                            " , string_length ( string ) + 1 ) );
    // (undo the replacement by performing it backwards)
    _string_replace ( string , "    " , "\r\n" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Replace\r\nall\r\nnewlines\r\nwith\r\n4\r\nspaces.\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Replace\r\nall\r\nnewlines\r\nwith\r\n4\r\nspaces.\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n" , string_length ( string ) + 1 ) );

    // TEST 6: string_replace removes all instances of the substring to remove within the string if the length of the replacement substring is 0.
    _string_replace ( string , "\r\n" , "" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Replaceallnewlineswith4spaces." ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Replaceallnewlineswith4spaces." , string_length ( string ) + 1 ) );

    // TEST 7: string_replace replaces every character of the string with the replacement substring if the length of the subtring to remove is 0.
    _string_replace ( string , "" , "    " );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "                                                                                                                        " ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "                                                                                                                        " , string_length ( string ) + 1 ) );

    // Restore the original string prior to testing.
    string_clear ( string );
    _string_append ( string , "Replace\r\nall\r\nnewlines\r\nwith\r\n4\r\nspaces.\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n" );

    // TEST 9: string_replace does not fail if string to replace and replacement substring are the same length and share leading characters.
    _string_replace ( string , "\r\n" , "\r\t" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Replace\r\tall\r\tnewlines\r\twith\r\t4\r\tspaces.\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Replace\r\tall\r\tnewlines\r\twith\r\t4\r\tspaces.\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t" , string_length ( string ) + 1 ) );

    // TEST 10: string_replace does not fail if string to replace and replacement substring are different lengths and share leading characters.
    _string_replace ( string , "\r\t" , "\r\t\r\t" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Replace\r\t\r\tall\r\t\r\tnewlines\r\t\r\twith\r\t\r\t4\r\t\r\tspaces.\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Replace\r\t\r\tall\r\t\r\tnewlines\r\t\r\twith\r\t\r\t4\r\t\r\tspaces.\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t\r\t" , string_length ( string ) + 1 ) );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    string_destroy ( string );

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_string_strip_escape
( void )
{
    // u64 global_amount_allocated;
    // u64 array_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    char* string = string_create ();

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string );

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: __string_strip_escape (resizable string).

    // TEST 1.1: __string_strip_escape does not fail on an empty string.
    _string_append ( string , "" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_strip_escape ( string , "|" , _string_length ( "|" ) );
    EXPECT_EQ ( 0 , string_length ( string ) );
    EXPECT ( memory_equal ( string , "" , string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 1.2: __string_strip_escape removes a single character escape sequence from the string.
    _string_append ( string , "\\|S\\|trip\\|\\| me.\\|\\|\\|\\|\\|\\||\\ |\\ " );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_strip_escape ( string , "|" , _string_length ( "|" ) );
    EXPECT_EQ ( _string_length ( "|S|trip|| me.|||||||\\ |\\ " ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "|S|trip|| me.|||||||\\ |\\ " , string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 1.3: __string_strip_escape removes all backslashes from the string if the escape sequence is empty.
    _string_append ( string , "\\|S\\tri\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\p me.\\|\\\\\\\\\\" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_strip_escape ( string , "" , _string_length ( "" ) );
    EXPECT_EQ ( _string_length ( "|Strip me.|" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "|Strip me.|" , string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 1.4: __string_strip_escape removes a multi-character escape sequence from the string.
    _string_append ( string , "a\\|    Strip me.\\|    \\|    dfa" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_strip_escape ( string , "|    " , _string_length ( "|    " ) );
    EXPECT_EQ ( _string_length ( "a|    Strip me.|    |    dfa" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "a|    Strip me.|    |    dfa" , string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 1.5: __string_strip_escape does not modify the string if the escape sequence cannot be found.
    _string_append ( string , "f\\\\sdfds\\|    Strip me.\\|    d\\fa" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_strip_escape ( string , "|   ." , _string_length ( "|     " ) );
    EXPECT_EQ ( _string_length ( "f\\\\sdfds\\|    Strip me.\\|    d\\fa" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "f\\\\sdfds\\|    Strip me.\\|    d\\fa" , string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 2: string_strip_escape (fixed-length string).

    // TEST 2.1: string_strip_escape does not fail on an empty string.
    _string_append ( string , "" );
    _string_strip_escape ( string , "|" , string );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( 0 , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "" , _string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 2.2: string_strip_escape removes a single character escape sequence from the string.
    _string_append ( string , "\\|S\\|trip\\|\\| me.\\|\\|\\|\\|\\|\\||\\ |\\ " );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_strip_escape ( string , "|" , string );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "|S|trip|| me.|||||||\\ |\\ " ) , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "|S|trip|| me.|||||||\\ |\\ " , _string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 2.3: string_strip_escape removes all backslashes from the string if the escape sequence is empty.
    _string_append ( string , "\\|S\\tri\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\p me.\\|\\\\\\\\\\" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_strip_escape ( string , "" , string );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "|Strip me.|" ) , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "|Strip me.|" , _string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 2.4: string_strip_escape removes a multi-character escape sequence from the string.
    _string_append ( string , "a\\|    Strip me.\\|    \\|    dfa" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_strip_escape ( string , "|    " , string );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "a|    Strip me.|    |    dfa" ) , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "a|    Strip me.|    |    dfa" , _string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 2.5: string_strip_escape does not modify the string if the escape sequence cannot be found.
    _string_append ( string , "f\\\\sdfds\\|    Strip me.\\|    d\\fa" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_strip_escape ( string , "|   ." , string );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "f\\\\sdfds\\|    Strip me.\\|    d\\fa" ) , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "f\\\\sdfds\\|    Strip me.\\|    d\\fa" , _string_length ( string ) + 1 ) );
    string_clear ( string );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    string_destroy ( string );

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_string_strip_ansi
( void )
{
    // u64 global_amount_allocated;
    // u64 array_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    char* string = string_create ();

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string );

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: __string_strip_ansi (resizable string).

    // TEST 1.1: __string_strip_ansi does not fail on an empty string.
    _string_append ( string , "" );
    __string_strip_ansi ( string );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( 0 , string_length ( string ) );
    EXPECT ( memory_equal ( string , "" , string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 1.2: __string_strip_ansi removes a (short) single valid ANSI formatting code from the front and back of a string.
    _string_append ( string , ANSI_CC ( ANSI_CC_BG_DARK_RED ) "Strip me." ANSI_CC_RESET );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_strip_ansi ( string );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Strip me." ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Strip me." , string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 1.3: __string_strip_ansi removes a (long) single valid ANSI formatting code from the front and back of a string.
    _string_append ( string , "Strip \033[0;1;2;43;44;45;46m" ANSI_CC ( ANSI_CC_BG_DARK_RED ) "me." ANSI_CC_RESET );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_strip_ansi ( string );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Strip me." ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Strip me." , string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 1.4: __string_strip_ansi does not modify the string if no formatting codes are encountered.
    _string_append ( string , "Strip me." );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_strip_ansi ( string );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Strip me." ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Strip me." , string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 1.5: __string_strip_ansi truncates a string entirely if it is made up solely of ANSI formatting codes.
    _string_append ( string , ANSI_CC_RESET "\033[0;1;2;43;44;45;46;101m" ANSI_CC2 ( ANSI_CC_BG_CYAN , ANSI_CC_BOLD ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_strip_ansi ( string );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( 0 , string_length ( string ) );
    EXPECT ( memory_equal ( string , "" , 1 ) );
    string_clear ( string );

    // TEST 1.6: __string_strip_ansi ignores substrings which **almost** look like ANSI formatting codes.
    _string_append ( string , "This should not\033[;;;;;]m be stripped." );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_strip_ansi ( string );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "This should not\033[;;;;;]m be stripped." ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "This should not\033[;;;;;]m be stripped." , string_length ( string ) + 1 ) );
    string_clear ( string );
    _string_append ( string , "This should not\033[890345298430958349058;324234234243324234234;23423423423423;234234234234234;234234234234234322342342342342342343\033m be stripped." );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_strip_ansi ( string );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "This should not\033[890345298430958349058;324234234243324234234;23423423423423;234234234234234;234234234234234322342342342342342343\033m be stripped." ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "This should not\033[890345298430958349058;324234234243324234234;23423423423423;234234234234234;234234234234234322342342342342342343\033m be stripped." , string_length ( string ) + 1 ) );
    string_clear ( string );
    _string_append ( string , "This should not\033[47;106 be stripped." );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    __string_strip_ansi ( string );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "This should not\033[47;106 be stripped." ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "This should not\033[47;106 be stripped." , string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 2: string_strip_ansi (fixed-length string).

    // TEST 2.1: string_strip_ansi does not fail on an empty string.
    _string_append ( string , "" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_strip_ansi ( string , string );
    EXPECT_EQ ( 0 , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "" , _string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 2.2: string_strip_ansi removes a (short) single valid ANSI formatting code from the front and back of a string.
    _string_append ( string , ANSI_CC ( ANSI_CC_BG_DARK_RED ) "Strip me." ANSI_CC_RESET );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_strip_ansi ( string , string );
    EXPECT_EQ ( _string_length ( "Strip me." ) , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "Strip me." , _string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 2.3: string_strip_ansi removes a (short) single valid ANSI formatting code from the front and back of a string.
    _string_append ( string , "Strip \033[0;1;2;43;44;45;46m" ANSI_CC ( ANSI_CC_BG_DARK_RED ) "me." ANSI_CC_RESET );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_strip_ansi ( string , string );
    EXPECT_EQ ( _string_length ( "Strip me." ) , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "Strip me." , _string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 2.4: __string_strip_ansi does not modify the string if no formatting codes are encountered.
    _string_append ( string , "Strip me." );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_strip_ansi ( string , string );
    EXPECT_EQ ( _string_length ( "Strip me." ) , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "Strip me." , _string_length ( string ) + 1 ) );
    string_clear ( string );

    // TEST 2.5: string_strip_ansi truncates a string entirely if it is made up solely of ANSI formatting codes.
    _string_append ( string , ANSI_CC_RESET "\033[0;1;2;43;44;45;46;101m" ANSI_CC2 ( ANSI_CC_BG_CYAN , ANSI_CC_BOLD ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_strip_ansi ( string , string );
    EXPECT_EQ ( 0 , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "" , 1 ) );
    string_clear ( string );

    // TEST 2.6: string_strip_ansi ignores substrings which **almost** look like ANSI formatting codes.
    _string_append ( string , "This should not\033[;;;;;]m be stripped." );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_strip_ansi ( string , string );
    EXPECT_EQ ( _string_length ( "This should not\033[;;;;;]m be stripped." ) , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "This should not\033[;;;;;]m be stripped." , _string_length ( string ) + 1 ) );
    string_clear ( string );
    _string_append ( string , "This should not\033[890345298430958349058;324234234243324234234;23423423423423;234234234234234;234234234234234322342342342342342343\033m be stripped." );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_strip_ansi ( string , string );
    EXPECT_EQ ( _string_length ( "This should not\033[890345298430958349058;324234234243324234234;23423423423423;234234234234234;234234234234234322342342342342342343\033m be stripped." ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "This should not\033[890345298430958349058;324234234243324234234;23423423423423;234234234234234;234234234234234322342342342342342343\033m be stripped." , string_length ( string ) + 1 ) );
    string_clear ( string );
    _string_append ( string , "This should not\033[47;106 be stripped." );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    _string_strip_ansi ( string , string );
    EXPECT_EQ ( _string_length ( "This should not\033[47;106 be stripped." ) , _string_length ( string ) );
    EXPECT ( memory_equal ( string , "This should not\033[47;106 be stripped." , _string_length ( string ) + 1 ) );
    string_clear ( string );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    string_destroy ( string );

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_string_u64_and_i64
( void )
{
    char string[ STRING_INTEGER_MAX_LENGTH + 1 ];

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: string_i64, longest signed value, radix 16.
    EXPECT_EQ ( _string_length ( "8000000000000001" ) , string_i64 ( -9223372036854775807 , 16 , string ) );
    EXPECT ( memory_equal ( string , "8000000000000001" , _string_length ( "8000000000000001" ) ) );
    
    // TEST 2: string_i64, longest signed value, radix 10.
    EXPECT_EQ ( _string_length ( "-9223372036854775807" ) , string_i64 ( -9223372036854775807 , 10 , string ) );
    EXPECT ( memory_equal ( string , "-9223372036854775807" , _string_length ( "-9223372036854775807" ) ) );
    
    // TEST 3: string_i64, longest signed value, radix 2.
    EXPECT_EQ ( _string_length ( "1000000000000000000000000000000000000000000000000000000000000001" ) , string_i64 ( -9223372036854775807 , 2 , string ) );
    EXPECT ( memory_equal ( string , "1000000000000000000000000000000000000000000000000000000000000001" , _string_length ( "1000000000000000000000000000000000000000000000000000000000000001" ) ) );
    
    // TEST 4: string_i64, longest signed value, radix 36.
    EXPECT_EQ ( _string_length ( "1Y2P0IJ32E8E9" ) , string_i64 ( -9223372036854775807 , 36 , string ) );
    EXPECT ( memory_equal ( string , "1Y2P0IJ32E8E9" , _string_length ( "1Y2P0IJ32E8E9" ) ) );
    
    // TEST 5: string_u64, longest unsigned value, radix 16.
    EXPECT_EQ ( _string_length ( "FFFFFFFFFFFFFFFF" ) , string_u64 ( 18446744073709551615U , 16 , string ) );
    EXPECT ( memory_equal ( string , "FFFFFFFFFFFFFFFF" , _string_length ( "FFFFFFFFFFFFFFFF" ) ) );
    
    // TEST 6: string_u64, longest unsigned value, radix 10.
    EXPECT_EQ ( _string_length ( "18446744073709551615" ) , string_u64 ( 18446744073709551615U , 10 , string ) );
    EXPECT ( memory_equal ( string , "18446744073709551615" , _string_length ( "18446744073709551615" ) ) );
    
    // TEST 7: string_u64, longest unsigned value, radix 8.
    EXPECT_EQ ( _string_length ( "1777777777777777777777" ) , string_u64 ( 18446744073709551615U , 8 , string ) );
    EXPECT ( memory_equal ( string , "1777777777777777777777" , _string_length ( "1777777777777777777777" ) ) );
    
    // TEST 8: string_u64, longest unsigned value, radix 2.
    EXPECT_EQ ( _string_length ( "1111111111111111111111111111111111111111111111111111111111111111" ) , string_u64 ( 18446744073709551615U , 2 , string ) );
    EXPECT ( memory_equal ( string , "1111111111111111111111111111111111111111111111111111111111111111" , _string_length ( "1111111111111111111111111111111111111111111111111111111111111111" ) ) );
    
    // TEST 9: string_u64, longest unsigned value, radix 36.
    EXPECT_EQ ( _string_length ( "3W5E11264SGSF" ) , string_u64 ( 18446744073709551615U , 36 , string ) );
    EXPECT ( memory_equal ( string , "3W5E11264SGSF" , _string_length ( "3W5E11264SGSF" ) ) );
    
    // TEST 10: string_i64, negative value, radix 16.
    EXPECT_EQ ( _string_length ( "FFFFFFFA8B8DD024" ) , string_i64 ( -23428476892 , 16 , string ) );
    EXPECT ( memory_equal ( string , "FFFFFFFA8B8DD024" , _string_length ( "FFFFFFFA8B8DD024" ) ) );
    
    // TEST 11: string_i64, negative value, radix 10.
    EXPECT_EQ ( _string_length ( "-23428476892" ) , string_i64 ( -23428476892 , 10 , string ) );
    EXPECT ( memory_equal ( string , "-23428476892" , _string_length ( "-23428476892" ) ) );
    
    // TEST 12: string_i64, negative value, radix 2.
    EXPECT_EQ ( _string_length ( "1111111111111111111111111111101010001011100011011101000000100100" ) , string_i64 ( -23428476892 , 2 , string ) );
    EXPECT ( memory_equal ( string , "1111111111111111111111111111101010001011100011011101000000100100" , _string_length ( "1111111111111111111111111111101010001011100011011101000000100100" ) ) );
    
    // TEST 13: string_i64, negative value, radix 36.
    EXPECT_EQ ( _string_length ( "3W5E10REO2WBO" ) , string_i64 ( -23428476892 , 36 , string ) );
    EXPECT ( memory_equal ( string , "3W5E10REO2WBO" , _string_length ( "3W5E10REO2WBO" ) ) );

    // TEST 14: string_u64, radix 16.
    EXPECT_EQ ( _string_length ( "574722FDC" ) , string_u64 ( 23428476892 , 16 , string ) );
    EXPECT ( memory_equal ( string , "574722FDC" , _string_length ( "574722FDC" ) ) );
    
    // TEST 15: string_u64, radix 10.
    EXPECT_EQ ( _string_length ( "23428476892" ) , string_u64 ( 23428476892 , 10 , string ) );
    EXPECT ( memory_equal ( string , "23428476892" , _string_length ( "23428476892" ) ) );
    
    // TEST 16: string_u64, radix 8.
    EXPECT_EQ ( _string_length ( "256434427734" ) , string_u64 ( 23428476892 , 8 , string ) );
    EXPECT ( memory_equal ( string , "256434427734" , _string_length ( "256434427734" ) ) );
    
    // TEST 17: string_u64, radix 2.
    EXPECT_EQ ( _string_length ( "10101110100011100100010111111011100" ) , string_u64 ( 23428476892 , 2 , string ) );
    EXPECT ( memory_equal ( string , "10101110100011100100010111111011100" , _string_length ( "10101110100011100100010111111011100" ) ) );
    
    // TEST 18: string_u64, radix 36.
    EXPECT_EQ ( _string_length ( "ARGPKGS" ) , string_i64 ( 23428476892 , 36 , string ) );
    EXPECT ( memory_equal ( string , "ARGPKGS" , _string_length ( "ARGPKGS" ) ) );

    // TEST 19: string_u64, value == 0, radix 16.
    EXPECT_EQ ( _string_length ( "0" ) , string_u64 ( 0 , 16 , string ) );
    EXPECT ( memory_equal ( string , "0" , _string_length ( "0" ) ) );
    
    // TEST 20: string_u64, value == 0, radix 10.
    EXPECT_EQ ( _string_length ( "0" ) , string_u64 ( 0 , 10 , string ) );
    EXPECT ( memory_equal ( string , "0" , _string_length ( "0" ) ) );
    
    // TEST 21: string_u64, value == 0, radix 8.
    EXPECT_EQ ( _string_length ( "0" ) , string_u64 ( 0 , 8 , string ) );
    EXPECT ( memory_equal ( string , "0" , _string_length ( "0" ) ) );
    
    // TEST 22: string_u64, value == 0, radix 2.
    EXPECT_EQ ( _string_length ( "0" ) , string_u64 ( 0 , 2 , string ) );
    EXPECT ( memory_equal ( string , "0" , _string_length ( "0" ) ) );
    
    // TEST 23: string_u64, value == 0, radix 36.
    EXPECT_EQ ( _string_length ( "0" ) , string_u64 ( 0 , 36 , string ) );
    EXPECT ( memory_equal ( string , "0" , _string_length ( "0" ) ) );

    // TEST 24: string_i64, value == 0, radix 16.
    EXPECT_EQ ( _string_length ( "0" ) , string_i64 ( 0 , 16 , string ) );
    EXPECT ( memory_equal ( string , "0" , _string_length ( "0" ) ) );
    
    // TEST 25: string_i64, value == 0, radix 10.
    EXPECT_EQ ( _string_length ( "0" ) , string_i64 ( 0 , 10 , string ) );
    EXPECT ( memory_equal ( string , "0" , _string_length ( "0" ) ) );
    
    // TEST 26: string_i64, value == 0, radix 8.
    EXPECT_EQ ( _string_length ( "0" ) , string_i64 ( 0 , 8 , string ) );
    EXPECT ( memory_equal ( string , "0" , _string_length ( "0" ) ) );
    
    // TEST 27: string_i64, value == 0, radix 2.
    EXPECT_EQ ( _string_length ( "0" ) , string_i64 ( 0 , 2 , string ) );
    EXPECT ( memory_equal ( string , "0" , _string_length ( "0" ) ) );
    
    // TEST 28: string_i64, value == 0, radix 36.
    EXPECT_EQ ( _string_length ( "0" ) , string_i64 ( 0 , 36 , string ) );
    EXPECT ( memory_equal ( string , "0" , _string_length ( "0" ) ) );
    
    // End test.
    ////////////////////////////////////////////////////////////////////////////

    return true;
}

u8
test_string_f64
( void )
{
    const u8 default_precision = 6;
    char string[ STRING_FLOAT_MAX_LENGTH + 1 ];

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: 0.0.

    // TEST 1.1: Abbreviated.
    EXPECT_EQ ( _string_length ( "0.000000" ) , string_f64 ( 0.0 , default_precision , true , string ) );
    EXPECT ( memory_equal ( string , "0.000000" , _string_length ( "0.000000" ) ) );

    // TEST 1.2: Default precision.
    EXPECT_EQ ( _string_length ( "0.000000" ) , string_f64 ( 0.0 , default_precision , false , string ) );
    EXPECT ( memory_equal ( string , "0.000000" , _string_length ( "0.000000" ) ) );

    // TEST 1.3: 0 precision.
    EXPECT_EQ ( _string_length ( "0." ) , string_f64 ( 0.0 , 0 , false , string ) );
    EXPECT ( memory_equal ( string , "0." , _string_length ( "0." ) ) );

    // TEST 1.4: 1 precision.
    EXPECT_EQ ( _string_length ( "0.0" ) , string_f64 ( 0.0 , 1 , false , string ) );
    EXPECT ( memory_equal ( string , "0.0" , _string_length ( "0.0" ) ) );

    // TEST 1.5: 2 precision.
    EXPECT_EQ ( _string_length ( "0.00" ) , string_f64 ( 0.0 , 2 , false , string ) );
    EXPECT ( memory_equal ( string , "0.00" , _string_length ( "0.00" ) ) );

    // TEST 1.6: Maximum precision.
    EXPECT_EQ ( _string_length ( "0.00000000000000000000" ) , string_f64 ( 0.0 , STRING_FLOAT_MAX_PRECISION , false , string ) );
    EXPECT ( memory_equal ( string , "0.00000000000000000000" , _string_length ( "0.00000000000000000000" ) ) );

    // TEST 2: Infinity.

    // TEST 2.1: Abbreviated.
    EXPECT_EQ ( _string_length ( "Infinity" ) , string_f64 ( INFINITY_64 , default_precision , true , string ) );
    EXPECT ( memory_equal ( string , "Infinity" , _string_length ( "Infinity" ) ) );

    // TEST 2.2: Default precision.
    EXPECT_EQ ( _string_length ( "Infinity" ) , string_f64 ( INFINITY_64 , default_precision , false , string ) );
    EXPECT ( memory_equal ( string , "Infinity" , _string_length ( "Infinity" ) ) );

    // TEST 2.3: 0 precision.
    EXPECT_EQ ( _string_length ( "Infinity" ) , string_f64 ( INFINITY_64 , 0 , false , string ) );
    EXPECT ( memory_equal ( string , "Infinity" , _string_length ( "Infinity" ) ) );

    // TEST 2.4: 1 precision.
    EXPECT_EQ ( _string_length ( "Infinity" ) , string_f64 ( INFINITY_64 , 1 , false , string ) );
    EXPECT ( memory_equal ( string , "Infinity" , _string_length ( "Infinity" ) ) );

    // TEST 2.5: 2 precision.
    EXPECT_EQ ( _string_length ( "Infinity" ) , string_f64 ( INFINITY_64 , 2 , false , string ) );
    EXPECT ( memory_equal ( string , "Infinity" , _string_length ( "Infinity" ) ) );

    // TEST 2.6: Maximum precision.
    EXPECT_EQ ( _string_length ( "Infinity" ) , string_f64 ( INFINITY_64 , STRING_FLOAT_MAX_PRECISION , false , string ) );
    EXPECT ( memory_equal ( string , "Infinity" , _string_length ( "Infinity" ) ) );

    // TEST 3: NaN

    // TEST 3.1: Abbreviated.
    EXPECT_EQ ( _string_length ( "NaN" ) , string_f64 ( 0.0 / 0.0 , default_precision , true , string ) );
    EXPECT ( memory_equal ( string , "NaN" , _string_length ( "NaN" ) ) );

    // TEST 3.2: Default precision.
    EXPECT_EQ ( _string_length ( "NaN" ) , string_f64 ( 0.0 / 0.0 , default_precision , false , string ) );
    EXPECT ( memory_equal ( string , "NaN" , _string_length ( "NaN" ) ) );

    // TEST 3.3: 0 precision.
    EXPECT_EQ ( _string_length ( "NaN" ) , string_f64 ( 0.0 / 0.0 , 0 , false , string ) );
    EXPECT ( memory_equal ( string , "NaN" , _string_length ( "NaN" ) ) );

    // TEST 3.4: 1 precision.
    EXPECT_EQ ( _string_length ( "NaN" ) , string_f64 ( 0.0 / 0.0 , 1 , false , string ) );
    EXPECT ( memory_equal ( string , "NaN" , _string_length ( "NaN" ) ) );

    // TEST 3.5: 2 precision.
    EXPECT_EQ ( _string_length ( "NaN" ) , string_f64 ( 0.0 / 0.0 , 2 , false , string ) );
    EXPECT ( memory_equal ( string , "NaN" , _string_length ( "NaN" ) ) );

    // TEST 3.6: Maximum precision.
    EXPECT_EQ ( _string_length ( "NaN" ) , string_f64 ( 0.0 / 0.0 , STRING_FLOAT_MAX_PRECISION , false , string ) );
    EXPECT ( memory_equal ( string , "NaN" , _string_length ( "NaN" ) ) );

    // TEST 4: Maximum 64-bit signed value.
    
    // TEST 4.1: Abbreviated.
    EXPECT_EQ ( _string_length ( "9.000000E18" ) , string_f64 ( 9E18 , default_precision , true , string ) );
    EXPECT ( memory_equal ( string , "9.000000E18" , _string_length ( "9.000000E18" ) ) );

    // TEST 4.2: Default precision.
    EXPECT_EQ ( _string_length ( "9000000000000000000.000000" ) , string_f64 ( 9E18 , default_precision , false , string ) );
    EXPECT ( memory_equal ( string , "9000000000000000000.000000" , _string_length ( "9000000000000000000.000000" ) ) );

    // TEST 4.3: 0 precision.
    EXPECT_EQ ( _string_length ( "9000000000000000000." ) , string_f64 ( 9E18 , 0 , false , string ) );
    EXPECT ( memory_equal ( string , "9000000000000000000." , _string_length ( "9000000000000000000." ) ) );

    // TEST 4.4: 1 precision.
    EXPECT_EQ ( _string_length ( "9000000000000000000.0" ) , string_f64 ( 9E18 , 1 , false , string ) );
    EXPECT ( memory_equal ( string , "9000000000000000000.0" , _string_length ( "9000000000000000000.0" ) ) );

    // TEST 4.5: 2 precision.
    EXPECT_EQ ( _string_length ( "9000000000000000000.00" ) , string_f64 ( 9E18 , 2 , false , string ) );
    EXPECT ( memory_equal ( string , "9000000000000000000.00" , _string_length ( "9000000000000000000.00" ) ) );

    // TEST 4.6: Maximum precision.
    EXPECT_EQ ( _string_length ( "9000000000000000000.00000000000000000000" ) , string_f64 ( 9E18 , STRING_FLOAT_MAX_PRECISION , false , string ) );
    EXPECT ( memory_equal ( string , "9000000000000000000.00000000000000000000" , _string_length ( "9000000000000000000.00000000000000000000" ) ) );

    // TEST 5: Minimum 64-bit signed value.
    
    // TEST 5.1: Abbreviated.
    EXPECT_EQ ( _string_length ( "-9.000000E18" ) , string_f64 ( -9E18 , default_precision , true , string ) );
    EXPECT ( memory_equal ( string , "-9.000000E18" , _string_length ( "-9.000000E18" ) ) );

    // TEST 5.2: Default precision.
    EXPECT_EQ ( _string_length ( "-9000000000000000000.000000" ) , string_f64 ( -9E18 , default_precision , false , string ) );
    EXPECT ( memory_equal ( string , "-9000000000000000000.000000" , _string_length ( "-9000000000000000000.000000" ) ) );

    // TEST 5.3: 0 precision.
    EXPECT_EQ ( _string_length ( "-9000000000000000000." ) , string_f64 ( -9E18 , 0 , false , string ) );
    EXPECT ( memory_equal ( string , "-9000000000000000000." , _string_length ( "-9000000000000000000." ) ) );

    // TEST 5.4: 1 precision.
    EXPECT_EQ ( _string_length ( "-9000000000000000000.0" ) , string_f64 ( -9E18 , 1 , false , string ) );
    EXPECT ( memory_equal ( string , "-9000000000000000000.0" , _string_length ( "-9000000000000000000.0" ) ) );

    // TEST 5.5: 2 precision.
    EXPECT_EQ ( _string_length ( "-9000000000000000000.00" ) , string_f64 ( -9E18 , 2 , false , string ) );
    EXPECT ( memory_equal ( string , "-9000000000000000000.00" , _string_length ( "-9000000000000000000.00" ) ) );

    // TEST 5.6: Maximum precision.
    EXPECT_EQ ( _string_length ( "-9000000000000000000.00000000000000000000" ) , string_f64 ( -9E18 , STRING_FLOAT_MAX_PRECISION , false , string ) );
    EXPECT ( memory_equal ( string , "-9000000000000000000.00000000000000000000" , _string_length ( "-9000000000000000000.00000000000000000000" ) ) );

    // TEST 6: Value larger than the maximum 64-bit signed value.
    
    // TEST 6.1: Abbreviated.
    EXPECT_EQ ( _string_length ( "9.000000E19" ) , string_f64 ( 9E19 , default_precision , true , string ) );
    EXPECT ( memory_equal ( string , "9.000000E19" , _string_length ( "9.000000E19" ) ) );

    // TEST 6.2: Default precision (force-abbreviated).
    EXPECT_EQ ( _string_length ( "9.000000E19" ) , string_f64 ( 9E19 , default_precision , false , string ) );
    EXPECT ( memory_equal ( string , "9.000000E19" , _string_length ( "9.000000E19" ) ) );

    // TEST 6.3: 0 precision (force-abbreviated).
    EXPECT_EQ ( _string_length ( "9E19" ) , string_f64 ( 9E19 , 0 , false , string ) );
    EXPECT ( memory_equal ( string , "9E19" , _string_length ( "9E19" ) ) );

    // TEST 6.4: 1 precision (force-abbreviated).
    EXPECT_EQ ( _string_length ( "9.0E19" ) , string_f64 ( 9E19 , 1 , false , string ) );
    EXPECT ( memory_equal ( string , "9.0E19" , _string_length ( "9.0E19" ) ) );

    // TEST 6.5: 2 precision (force-abbreviated).
    EXPECT_EQ ( _string_length ( "9.00E19" ) , string_f64 ( 9E19 , 2 , false , string ) );
    EXPECT ( memory_equal ( string , "9.00E19" , _string_length ( "9.00E19" ) ) );

    // TEST 6.6: Maximum precision (force-abbreviated).
    EXPECT_EQ ( _string_length ( "9.00000000000000000000E19" ) , string_f64 ( 9E19 , STRING_FLOAT_MAX_PRECISION , false , string ) );
    EXPECT ( memory_equal ( string , "9.00000000000000000000E19" , _string_length ( "9.00000000000000000000E19" ) ) );

    // TEST 7: Value smaller than the minimum 64-bit signed value.
    
    // TEST 7.1: Abbreviated.
    EXPECT_EQ ( _string_length ( "-9.000000E19" ) , string_f64 ( -9E19 , default_precision , true , string ) );
    EXPECT ( memory_equal ( string , "-9.000000E19" , _string_length ( "-9.000000E19" ) ) );

    // TEST 7.2: Default precision (force-abbreviated).
    EXPECT_EQ ( _string_length ( "-9.000000E19" ) , string_f64 ( -9E19 , default_precision , false , string ) );
    EXPECT ( memory_equal ( string , "-9.000000E19" , _string_length ( "-9.000000E19" ) ) );

    // TEST 7.3: 0 precision (force-abbreviated).
    EXPECT_EQ ( _string_length ( "-9E19" ) , string_f64 ( -9E19 , 0 , false , string ) );
    EXPECT ( memory_equal ( string , "-9E19" , _string_length ( "-9E19" ) ) );

    // TEST 7.4: 1 precision (force-abbreviated).
    EXPECT_EQ ( _string_length ( "-9.0E19" ) , string_f64 ( -9E19 , 1 , false , string ) );
    EXPECT ( memory_equal ( string , "-9.0E19" , _string_length ( "-9.0E19" ) ) );

    // TEST 7.5: 2 precision (force-abbreviated).
    EXPECT_EQ ( _string_length ( "-9.00E19" ) , string_f64 ( -9E19 , 2 , false , string ) );
    EXPECT ( memory_equal ( string , "-9.00E19" , _string_length ( "-9.00E19" ) ) );

    // TEST 7.6: Maximum precision (force-abbreviated).
    EXPECT_EQ ( _string_length ( "-9.00000000000000000000E19" ) , string_f64 ( -9E19 , STRING_FLOAT_MAX_PRECISION , false , string ) );
    EXPECT ( memory_equal ( string , "-9.00000000000000000000E19" , _string_length ( "-9.00000000000000000000E19" ) ) );

    // TEST 8: -Infinity

    // TEST 8.1: Abbreviated.
    EXPECT_EQ ( _string_length ( "-Infinity" ) , string_f64 ( -INFINITY_64 , default_precision , true , string ) );
    EXPECT ( memory_equal ( string , "-Infinity" , _string_length ( "-Infinity" ) ) );

    // TEST 8.2: Default precision.
    EXPECT_EQ ( _string_length ( "-Infinity" ) , string_f64 ( -INFINITY_64 , default_precision , false , string ) );
    EXPECT ( memory_equal ( string , "-Infinity" , _string_length ( "-Infinity" ) ) );

    // TEST 8.3: 0 precision.
    EXPECT_EQ ( _string_length ( "-Infinity" ) , string_f64 ( -INFINITY_64 , 0 , false , string ) );
    EXPECT ( memory_equal ( string , "-Infinity" , _string_length ( "-Infinity" ) ) );

    // TEST 8.4: 1 precision.
    EXPECT_EQ ( _string_length ( "-Infinity" ) , string_f64 ( -INFINITY_64 , 1 , false , string ) );
    EXPECT ( memory_equal ( string , "-Infinity" , _string_length ( "-Infinity" ) ) );

    // TEST 8.5: 2 precision.
    EXPECT_EQ ( _string_length ( "-Infinity" ) , string_f64 ( -INFINITY_64 , 2 , false , string ) );
    EXPECT ( memory_equal ( string , "-Infinity" , _string_length ( "-Infinity" ) ) );

    // TEST 8.6: Maximum precision.
    EXPECT_EQ ( _string_length ( "-Infinity" ) , string_f64 ( -INFINITY_64 , STRING_FLOAT_MAX_PRECISION , false , string ) );
    EXPECT ( memory_equal ( string , "-Infinity" , _string_length ( "-Infinity" ) ) );

    // TEST 9: 1E-20.
    
    // TEST 9.1: Abbreviated.
    EXPECT_EQ ( _string_length ( "1.000000E-20" ) , string_f64 ( 0.00000000000000000001 , default_precision , true , string ) );
    EXPECT ( memory_equal ( string , "1.000000E-20" , _string_length ( "1.000000E-20" ) ) );

    // TEST 9.2: Default precision.
    EXPECT_EQ ( _string_length ( "0.000000" ) , string_f64 ( 0.00000000000000000001 , default_precision , false , string ) );
    EXPECT ( memory_equal ( string , "0.000000" , _string_length ( "0.000000" ) ) );

    // TEST 9.3: 0 precision.
    EXPECT_EQ ( _string_length ( "0." ) , string_f64 ( 0.00000000000000000001 , 0 , false , string ) );
    EXPECT ( memory_equal ( string , "0." , _string_length ( "0." ) ) );

    // TEST 9.4: 1 precision.
    EXPECT_EQ ( _string_length ( "0.0" ) , string_f64 ( 0.00000000000000000001 , 1 , false , string ) );
    EXPECT ( memory_equal ( string , "0.0" , _string_length ( "0.0" ) ) );

    // TEST 9.5: 2 precision.
    EXPECT_EQ ( _string_length ( "0.00" ) , string_f64 ( 0.00000000000000000001 , 2 , false , string ) );
    EXPECT ( memory_equal ( string , "0.00" , _string_length ( "0.00" ) ) );

    // TEST 9.6: Maximum precision.
    EXPECT_EQ ( _string_length ( "0.00000000000000000001" ) , string_f64 ( 0.00000000000000000001 , STRING_FLOAT_MAX_PRECISION , false , string ) );
    EXPECT ( memory_equal ( string , "0.00000000000000000001" , _string_length ( "0.00000000000000000001" ) ) );

    // TEST 10: 9.999E-1.
    
    // TEST 10.1: Abbreviated.
    EXPECT_EQ ( _string_length ( "9.999999E-1" ) , string_f64 ( 0.9999999999999999 , default_precision , true , string ) );
    EXPECT ( memory_equal ( string , "9.999999E-1" , _string_length ( "9.999999E-1" ) ) );

    // TEST 10.2: Default precision.
    EXPECT_EQ ( _string_length ( "0.999999" ) , string_f64 ( 0.9999999999999999 , default_precision , false , string ) );
    EXPECT ( memory_equal ( string , "0.999999" , _string_length ( "0.999999" ) ) );

    // TEST 10.3: 0 precision.
    EXPECT_EQ ( _string_length ( "0." ) , string_f64 ( 0.9999999999999999 , 0 , false , string ) );
    EXPECT ( memory_equal ( string , "0." , _string_length ( "0." ) ) );

    // TEST 10.4: 1 precision.
    EXPECT_EQ ( _string_length ( "0.9" ) , string_f64 ( 0.9999999999999999 , 1 , false , string ) );
    EXPECT ( memory_equal ( string , "0.9" , _string_length ( "0.9" ) ) );

    // TEST 10.5: 2 precision.
    EXPECT_EQ ( _string_length ( "0.99" ) , string_f64 ( 0.9999999999999999 , 2 , false , string ) );
    EXPECT ( memory_equal ( string , "0.99" , _string_length ( "0.99" ) ) );

    // TEST 10.6: Maximum precision.
    // TODO: Fix rounding / precision errors that only make this accurate up to 15 fractional digits.
    // Should be 0.99999999999999999999
    EXPECT_EQ ( _string_length ( "0.999999999999999" ) , string_f64 ( 0.9999999999999999 , /* STRING_FLOAT_MAX_PRECISION */ 15 , false , string ) );
    EXPECT ( memory_equal ( string , "0.999999999999999" , _string_length ( "0.999999999999999" ) ) );

    // TEST 11: π.
    
    // TEST 11.1: Abbreviated.
    EXPECT_EQ ( _string_length ( "3.141592E0" ) , string_f64 ( 3.14159265358979323846 , default_precision , true , string ) );
    EXPECT ( memory_equal ( string , "3.141592E0" , _string_length ( "3.141592E0" ) ) );

    // TEST 11.2: Default precision.
    EXPECT_EQ ( _string_length ( "3.141592" ) , string_f64 ( 3.14159265358979323846 , default_precision , false , string ) );
    EXPECT ( memory_equal ( string , "3.141592" , _string_length ( "3.141592" ) ) );

    // TEST 11.3: 0 precision.
    EXPECT_EQ ( _string_length ( "3." ) , string_f64 ( 3.14159265358979323846 , 0 , false , string ) );
    EXPECT ( memory_equal ( string , "3." , _string_length ( "3." ) ) );

    // TEST 11.4: 1 precision.
    EXPECT_EQ ( _string_length ( "3.1" ) , string_f64 ( 3.14159265358979323846 , 1 , false , string ) );
    EXPECT ( memory_equal ( string , "3.1" , _string_length ( "3.1" ) ) );

    // TEST 11.5: 2 precision.
    EXPECT_EQ ( _string_length ( "3.14" ) , string_f64 ( 3.14159265358979323846 , 2 , false , string ) );
    EXPECT ( memory_equal ( string , "3.14" , _string_length ( "3.14" ) ) );

    // TEST 11.6: Maximum precision.
    // TODO: Fix rounding / precision errors that only make this accurate up to 15 fractional digits.
    // Should be 3.14159265358979323846
    EXPECT_EQ ( _string_length ( "3.14159265358979311616" ) , string_f64 ( 3.14159265358979323846 , STRING_FLOAT_MAX_PRECISION , false , string ) );
    EXPECT ( memory_equal ( string , "3.14159265358979311616" , _string_length ( "3.14159265358979311616" ) ) );

    // TEST 12: -π.
    
    // TEST 12.1: Abbreviated.
    EXPECT_EQ ( _string_length ( "-3.141592E0" ) , string_f64 ( -3.14159265358979323846 , default_precision , true , string ) );
    EXPECT ( memory_equal ( string , "-3.141592E0" , _string_length ( "-3.141592E0" ) ) );

    // TEST 12.2: Default precision.
    EXPECT_EQ ( _string_length ( "-3.141592" ) , string_f64 ( -3.14159265358979323846 , default_precision , false , string ) );
    EXPECT ( memory_equal ( string , "-3.141592" , _string_length ( "-3.141592" ) ) );

    // TEST 12.3: 0 precision.
    EXPECT_EQ ( _string_length ( "-3." ) , string_f64 ( -3.14159265358979323846 , 0 , false , string ) );
    EXPECT ( memory_equal ( string , "-3." , _string_length ( "-3." ) ) );

    // TEST 12.4: 1 precision.
    EXPECT_EQ ( _string_length ( "-3.1" ) , string_f64 ( -3.14159265358979323846 , 1 , false , string ) );
    EXPECT ( memory_equal ( string , "-3.1" , _string_length ( "-3.1" ) ) );

    // TEST 12.5: 2 precision.
    EXPECT_EQ ( _string_length ( "-3.14" ) , string_f64 ( -3.14159265358979323846 , 2 , false , string ) );
    EXPECT ( memory_equal ( string , "-3.14" , _string_length ( "-3.14" ) ) );

    // TEST 12.6: Maximum precision.
    // TODO: Fix rounding / precision errors that only make this accurate up to 15 fractional digits.
    // Should be 3.14159265358979323846
    EXPECT_EQ ( _string_length ( "-3.14159265358979311616" ) , string_f64 ( -3.14159265358979323846 , STRING_FLOAT_MAX_PRECISION , false , string ) );
    EXPECT ( memory_equal ( string , "-3.14159265358979311616" , _string_length ( "-3.14159265358979311616" ) ) );
    
    // TEST 13: -0.01

    // TEST 13.1: Abbreviated.
    EXPECT_EQ ( _string_length ( "-1.000000E-2" ) , string_f64 ( -0.01 , default_precision , true , string ) );
    EXPECT ( memory_equal ( string , "-1.000000E-2" , _string_length ( "-1.000000E-2" ) ) );

    // TEST 13.2: Default precision.
    EXPECT_EQ ( _string_length ( "-0.010000" ) , string_f64 ( -0.01 , default_precision , false , string ) );
    EXPECT ( memory_equal ( string , "-0.010000" , _string_length ( "-0.010000" ) ) );

    // TEST 13.3: 0 precision.
    EXPECT_EQ ( _string_length ( "-0." ) , string_f64 ( -0.01 , 0 , false , string ) );
    EXPECT ( memory_equal ( string , "-0." , _string_length ( "-0." ) ) );

    // TEST 13.4: 1 precision.
    EXPECT_EQ ( _string_length ( "-0.0" ) , string_f64 ( -0.01 , 1 , false , string ) );
    EXPECT ( memory_equal ( string , "-0.0" , _string_length ( "-0.0" ) ) );

    // TEST 13.5: 2 precision.
    EXPECT_EQ ( _string_length ( "-0.01" ) , string_f64 ( -0.01 , 2 , false , string ) );
    EXPECT ( memory_equal ( string , "-0.01" , _string_length ( "-0.01" ) ) );

    // TEST 13.6: Maximum precision.
    EXPECT_EQ ( _string_length ( "-0.01000000000000000000" ) , string_f64 ( -0.01 , STRING_FLOAT_MAX_PRECISION , false , string ) );
    EXPECT ( memory_equal ( string , "-0.01000000000000000000" , _string_length ( "-0.01000000000000000000" ) ) );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    return true;
}

u8
test_to_u64
( void )
{
    u64 out;

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: Empty string fails.
    EXPECT_NOT ( _to_u64 ( "" , &out ) );

    // TEST 2: Whitespace-only string fails.
    EXPECT_NOT ( _to_u64 ( "              " , &out ) );

    // TEST 3: Zero.
    out = ( ( u64 ) -1 );
    EXPECT ( _to_u64 ( "0" , &out ) );
    EXPECT_EQ ( 0 , out );

    // TEST 4: Zero (with whitespace).
    out = ( ( u64 ) -1 );
    EXPECT ( _to_u64 ( "           0             " , &out ) );
    EXPECT_EQ ( 0 , out );

    // TEST 5: Zero (multiple characters).
    out = ( ( u64 ) -1 );
    EXPECT ( _to_u64 ( "00000000000" , &out ) );
    EXPECT_EQ ( 0 , out );

    // TEST 6: Maximum 64-bit unsigned value.
    out = 0;
    EXPECT ( _to_u64 ( "18446744073709551615" , &out ) );
    EXPECT_EQ ( 18446744073709551615U , out );

    // TEST 7: Maximum 64-bit unsigned value (with whitespace).
    out = 0;
    EXPECT ( _to_u64 ( "   18446744073709551615\n\t" , &out ) );
    EXPECT_EQ ( 18446744073709551615U , out );

    // TEST 8: Maximum 64-bit unsigned value (with whitespace and leading zeroes).
    out = 0;
    EXPECT ( _to_u64 ( "     000000000018446744073709551615     " , &out ) );
    EXPECT_EQ ( 18446744073709551615U , out );

    // TEST 9: Misc. 64-bit unsigned value.
    out = 0;
    EXPECT ( _to_u64 ( "23428476892" , &out ) );
    EXPECT_EQ ( 23428476892 , out );

    // TEST 10: Misc. 64-bit unsigned value (with leading zeroes).
    out = 0;
    EXPECT ( _to_u64 ( "0000000000023428476892" , &out ) );
    EXPECT_EQ ( 23428476892 , out );

    // TEST 11: Signed value fails.
    EXPECT_NOT ( _to_u64 ( "-23428476892" , &out ) );
    EXPECT_EQ ( 23428476892 , out );

    // TEST 12: Mixed whitespace and zeroes fails.
    EXPECT_NOT ( _to_u64 ( "0 000000000023428476892  " , &out ) );

    // TEST 13: Value too long.
    // TODO: Invalidate this instead of returning garbage.
    out = 0;
    EXPECT ( _to_u64 ( "00018446744073709551615000" , &out ) );
    EXPECT_EQ ( 18446744073709551615U , out );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    return true;
}

u8
test_to_i64
( void )
{
    i64 out;

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: Empty string fails.
    EXPECT_NOT ( _to_i64 ( "" , &out ) );

    // TEST 2: Whitespace-only string fails.
    EXPECT_NOT ( _to_i64 ( "              " , &out ) );

    // TEST 3: Zero.
    out = ( ( u64 ) -1 );
    EXPECT ( _to_i64 ( "0" , &out ) );
    EXPECT_EQ ( 0 , out );

    // TEST 4: Zero (with whitespace).
    out = ( ( u64 ) -1 );
    EXPECT ( _to_i64 ( "           0             " , &out ) );
    EXPECT_EQ ( 0 , out );

    // TEST 5: Zero (multiple characters).
    out = ( ( u64 ) -1 );
    EXPECT ( _to_i64 ( "00000000000" , &out ) );
    EXPECT_EQ ( 0 , out );

    // TEST 6: Zero (with sign).
    out = ( ( u64 ) -1 );
    EXPECT ( _to_i64 ( "-0" , &out ) );
    EXPECT_EQ ( 0 , out );

    // TEST 7: Sign only fails (1).
    EXPECT_NOT ( _to_i64 ( "-" , &out ) );

    // TEST 8: Sign only fails (2).
    EXPECT_NOT ( _to_i64 ( "+" , &out ) );

    // TEST 9: Maximum 64-bit signed value.
    out = 0;
    EXPECT ( _to_i64 ( "9223372036854775807" , &out ) );
    EXPECT_EQ ( 9223372036854775807 , out );

    // TEST 10: Maximum 64-bit signed value (with whitespace).
    out = 0;
    EXPECT ( _to_i64 ( "   9223372036854775807\n\t" , &out ) );
    EXPECT_EQ ( 9223372036854775807 , out );

    // TEST 11: Maximum 64-bit signed value (with whitespace and leading zeroes).
    out = 0;
    EXPECT ( _to_i64 ( "     00000000009223372036854775807     " , &out ) );
    EXPECT_EQ ( 9223372036854775807 , out );

    // TEST 12: Maximum 64-bit signed value (with sign).
    out = 0;
    EXPECT ( _to_i64 ( "+9223372036854775807" , &out ) );
    EXPECT_EQ ( 9223372036854775807 , out );

    // TEST 13: Minimum 64-bit signed value.
    out = 0;
    EXPECT ( _to_i64 ( "-9223372036854775808" , &out ) );
    EXPECT_EQ ( -9223372036854775807 - 1 , out );

    // TEST 14: Minimum 64-bit signed value (with whitespace).
    out = 0;
    EXPECT ( _to_i64 ( "   -9223372036854775808\n\t" , &out ) );
    EXPECT_EQ ( -9223372036854775807 - 1 , out );

    // TEST 15: Minimum 64-bit signed value (with whitespace and leading zeroes).
    out = 0;
    EXPECT ( _to_i64 ( "\n\n\n\n-00000000009223372036854775808     " , &out ) );
    EXPECT_EQ ( -9223372036854775807 - 1 , out );

    // TEST 16: Misc. 64-bit signed value.
    out = 0;
    EXPECT ( _to_i64 ( "-23428476892" , &out ) );
    EXPECT_EQ ( -23428476892 , out );

    // TEST 17: Misc. 64-bit signed value (with leading zeroes).
    out = 0;
    EXPECT ( _to_i64 ( "-0000000000023428476892" , &out ) );
    EXPECT_EQ ( -23428476892 , out );

    // TEST 18: Misc. 64-bit signed positive value (with sign).
    out = 0;
    EXPECT ( _to_i64 ( "+23428476892" , &out ) );
    EXPECT_EQ ( 23428476892 , out );

    // TEST 19: Mixed whitespace and zeroes fails.
    EXPECT_NOT ( _to_i64 ( "+0 000000000023428476892  " , &out ) );

    // TEST 20: Value too long.
    // TODO: Invalidate this instead of returning garbage.
    out = 0;
    EXPECT ( _to_i64 ( "18446744073709551615" , &out ) );
    EXPECT_EQ ( -1 , out );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    return true;
}

u8
test_to_f64
( void )
{
    const f64 tolerance = FLOAT_EPSILON_64;
    f64 out;

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: Empty string fails.
    EXPECT_NOT ( _to_f64 ( "" , &out ) );

    // TEST 2: Whitespace-only string fails.
    EXPECT_NOT ( _to_f64 ( "              " , &out ) );

    // TEST 3: Zero.
    out = INFINITY_64;
    EXPECT ( _to_f64 ( "0" , &out ) );
    EXPECT_EQF ( 0.0 , out , tolerance );

    // TEST 4: Zero (with whitespace).
    out = INFINITY_64;
    EXPECT ( _to_f64 ( "           0             " , &out ) );
    EXPECT_EQF ( 0.0 , out , tolerance );

    // TEST 5: Zero (multiple characters).
    out = INFINITY_64;
    EXPECT ( _to_f64 ( "00000000000" , &out ) );
    EXPECT_EQF ( 0.0 , out , tolerance );

    // TEST 6: Zero (with sign).
    out = INFINITY_64;
    EXPECT ( _to_f64 ( "-0" , &out ) );
    EXPECT_EQF ( 0.0 , out , tolerance );

    // TEST 7: Zero (with point).
    out = INFINITY_64;
    EXPECT ( _to_f64 ( "0." , &out ) );
    EXPECT_EQF ( 0.0 , out , tolerance );

    // TEST 8: Zero (with sign and point).
    out = INFINITY_64;
    EXPECT ( _to_f64 ( "-0." , &out ) );
    EXPECT_EQF ( 0.0 , out , tolerance );

    // TEST 9: Zero (with whitespace, sign, point, trailing zeroes).
    out = INFINITY_64;
    EXPECT ( _to_f64 ( "  +0.000   " , &out ) );
    EXPECT_EQF ( 0.0 , out , tolerance );

    // TEST 10: Point only yields zero.
    out = INFINITY_64;
    EXPECT ( _to_f64 ( "." , &out ) );
    EXPECT_EQF ( 0.0 , out , tolerance );

    // TEST 11: Point only (with whitespace) yields zero.
    out = INFINITY_64;
    EXPECT ( _to_f64 ( "   .   " , &out ) );
    EXPECT_EQF ( 0.0 , out , tolerance );

    // TEST 12: Point only (with sign) yields zero.
    out = INFINITY_64;
    EXPECT ( _to_f64 ( "+." , &out ) );
    EXPECT_EQF ( 0.0 , out , tolerance );

    // TEST 13: Point only (with sign, whitespace, trailing zeroes) yields zero.
    out = INFINITY_64;
    EXPECT ( _to_f64 ( "\t-.00000" , &out ) );
    EXPECT_EQF ( 0.0 , out , tolerance );

    // TEST 14: Sign only fails (1).
    EXPECT_NOT ( _to_f64 ( "-" , &out ) );

    // TEST 15: Sign only fails (2).
    EXPECT_NOT ( _to_f64 ( "+" , &out ) );

    // TEST 16: Infinity.
    out = 0.0;
    EXPECT ( _to_f64 ( "Infinity" , &out ) );
    EXPECT_EQF ( INFINITY_64 , out , tolerance );

    // TEST 17: Infinity (with whitespace).
    out = 0.0;
    EXPECT ( _to_f64 ( "  Infinity  " , &out ) );
    EXPECT_EQF ( INFINITY_64 , out , tolerance );

    // TEST 18: Infinity with leading zeroes fails.
    EXPECT_NOT ( _to_f64 ( "0Infinity" , &out ) );

    // TEST 19: Infinity with trailing zeroes fails.
    EXPECT_NOT ( _to_f64 ( "Infinity0" , &out ) );

    // TEST 20: Infinity is case-insensitive.
    out = 0.0;
    EXPECT ( _to_f64 ( "iNfInItY" , &out ) );
    EXPECT_EQF ( INFINITY_64 , out , tolerance );

    // TEST 21: -Infinity.
    out = 0.0;
    EXPECT ( _to_f64 ( "-Infinity" , &out ) );
    EXPECT_EQF ( -INFINITY_64 , out , tolerance );

    // TEST 22: -Infinity (with whitespace).
    out = 0.0;
    EXPECT ( _to_f64 ( "  -Infinity  " , &out ) );
    EXPECT_EQF ( -INFINITY_64 , out , tolerance );

    // TEST 23: -Infinity with leading zeroes fails.
    EXPECT_NOT ( _to_f64 ( "-0Infinity" , &out ) );
    
    // TEST 24: -Infinity with trailing zeroes fails.
    EXPECT_NOT ( _to_f64 ( "-Infinity0" , &out ) );
    
    // TEST 25: -Infinity is case-insensitive.
    out = 0.0;
    EXPECT ( _to_f64 ( "-iNfInItY" , &out ) );
    EXPECT_EQF ( -INFINITY_64 , out , tolerance );

    // TEST 26: NaN.
    out = 0.0;
    EXPECT ( _to_f64 ( "NaN" , &out ) );
    EXPECT_EQF ( 0.0 / 0.0 , out , tolerance );

    // TEST 27: NaN (with whitespace).
    out = 0.0;
    EXPECT ( _to_f64 ( "    NaN" , &out ) );
    EXPECT_EQF ( 0.0 / 0.0 , out , tolerance );

    // TEST 28: NaN with leading zeroes fails.
    EXPECT_NOT ( _to_f64 ( "0NaN" , &out ) );

    // TEST 29: NaN with trailing zeroes fails.
    EXPECT_NOT ( _to_f64 ( "NaN0" , &out ) );

    // TEST 30: NaN is case-insensitive.
    out = 0.0;
    EXPECT ( _to_f64 ( "nAn" , &out ) );
    EXPECT_EQF ( 0.0 / 0.0 , out , tolerance );
    
    // TEST 31: High-precision small value.
    out = 0.0;
    EXPECT ( _to_f64 ( "0.0987654321" , &out ) );
    EXPECT_EQF ( 0.0987654321 , out , tolerance );

    // TEST 32: High-precision small value (with whitespace).
    out = 0.0;
    EXPECT ( _to_f64 ( "   0.0987654321\n\t" , &out ) );
    EXPECT_EQF ( 0.0987654321 , out , tolerance );

    // TEST 33: High-precision small value (with leading and trailing zeroes).
    out = 0.0;
    EXPECT ( _to_f64 ( "0000000000.09876543210000000000\n\t" , &out ) );
    EXPECT_EQF ( 0.0987654321 , out , tolerance );

    // TEST 34: High-precision small value (with sign).
    out = 0.0;
    EXPECT ( _to_f64 ( "+.0987654321\n\t" , &out ) );
    EXPECT_EQF ( 0.0987654321 , out , tolerance );

    // TEST 35: Minimum 64-bit signed value.
    // TODO: Fix lossy conversion here.
    out = 0.0;
    EXPECT ( _to_f64 ( "-9223372036854775808" , &out ) );
    EXPECT_EQF ( -9223372036854778240.0 , out , tolerance );

    // TEST 36: Minimum 64-bit signed value (with whitespace).
    // TODO: Fix lossy conversion here.
    out = 0.0;
    EXPECT ( _to_f64 ( "   -9223372036854775808\n\t" , &out ) );
    EXPECT_EQF ( -9223372036854778240.0 , out , tolerance );

    // TEST 37: Minimum 64-bit signed value (with leading and trailing zeroes).
    // TODO: Fix lossy conversion here.
    out = 0.0;
    EXPECT ( _to_f64 ( "-00000000009223372036854775808.0000\n\t" , &out ) );
    EXPECT_EQF ( -9223372036854778240.0 , out , tolerance );

    // TEST 38: Maximum 64-bit signed value.
    // TODO: Fix lossy conversion here.
    out = 0.0;
    EXPECT ( _to_f64 ( "9223372036854775807" , &out ) );
    EXPECT_EQF ( 9223372036854778240.0 , out , tolerance );

    // TEST 39: Maximum 64-bit signed value (with whitespace).
    // TODO: Fix lossy conversion here.
    out = 0.0;
    EXPECT ( _to_f64 ( "   9223372036854775807\n\t" , &out ) );
    EXPECT_EQF ( 9223372036854778240.0 , out , tolerance );

    // TEST 40: Maximum 64-bit signed value (with leading and trailing zeroes).
    // TODO: Fix lossy conversion here.
    out = 0.0;
    EXPECT ( _to_f64 ( "00000000009223372036854775807.0000\n\t" , &out ) );
    EXPECT_EQF ( 9223372036854778240.0 , out , tolerance );

    // TEST 41: Maximum 64-bit signed value (with sign).
    // TODO: Fix lossy conversion here.
    out = 0.0;
    EXPECT ( _to_f64 ( "+9223372036854775808." , &out ) );
    EXPECT_EQF ( 9223372036854778240.0 , out , tolerance );

    // TEST 42: Misc. value.
    out = 0.0;
    EXPECT ( _to_f64 ( "4294967295.12345" , &out ) );
    EXPECT_EQF ( 4294967295.12345 , out , tolerance );

    // TEST 43: Misc. value (with whitespace).
    out = 0.0;
    EXPECT ( _to_f64 ( "   4294967295.12345\n\t" , &out ) );
    EXPECT_EQF ( 4294967295.12345 , out , tolerance );

    // TEST 44: Misc. value (with leading and trailing zeroes).
    out = 0.0;
    EXPECT ( _to_f64 ( "00000000004294967295.123450\n\t" , &out ) );
    EXPECT_EQF ( 4294967295.12345 , out , tolerance );

    // TEST 45: Misc. value (with sign).
    out = 0.0;
    EXPECT ( _to_f64 ( "+4294967295.12345" , &out ) );
    EXPECT_EQF ( 4294967295.12345 , out , tolerance );

    // TEST 46: Value which is too long fails.
    // TODO: Implement abbreviated notation handler to support these values.
    EXPECT_NOT ( _to_f64 ( "10000000000000000000." , &out ) );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    return true;
}

u8
test_string_format
( void )
{
    // u64 global_amount_allocated;
    // u64 array_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    file_t file_in;
    file_stdin ( &file_in );
    file_in.valid = false;
    const char* invalid_file_out = "\n\t{"
                                   "\n\t   File info:  Invalid file!"
                                   "\n\t   Mode:       -"
                                   "\n\t   Size:       -"
                                   "\n\t   Position:   -"
                                   "\n\t}\n"
                                 ;
    const file_t const_file_array_in[ 3 ] = { file_in , file_in , file_in };
    const f32 const_f32_array_in[ 16 ] = { -8 , -7 , -6 , -5 , -4 , -3 , -2 , -1 , 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 };
    const i8 const_i8_array_in[ 16 ] = { -8 , -7 , -6 , -5 , -4 , -3 , -2 , -1 , 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 };
    const char* const_string_array_in[ 4 ] = { "Hello" , " " , "world" , "!" };
    const char* const_string_in = "Hello world!";
    file_t* file_array_in = array_create_from ( file_t , const_file_array_in , 3 );
    f32* f32_array_in = array_create_from ( f32 , const_f32_array_in , 16 );
    i8* i8_array_in = array_create_from ( i8 , const_i8_array_in , 16 );
    char** string_array_in = array_create_from ( char* , const_string_array_in , 4 );
    char* string_in = string_create_from ( const_string_in );
    char* really_long_string_in = _string_create ( 100 * STACK_STRING_MAX_SIZE /* MIN ( 1000 * STACK_STRING_MAX_SIZE , memory_amount_free () / 2 - KiB ( 1 ) ) */ );
    f64 float_in;
    char* string;

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string_in );
    EXPECT_NEQ ( 0 , really_long_string_in );
    
    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: If given a single-character string in which the only character is the format specifier token, string_format returns a copy of the format string.
    string = string_format ( "%" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%" , string_length ( string ) ) );
    string_destroy ( string );

    LOGWARN ( "The following errors are intentionally triggered by a test:" );

    // TEST 2: string_format logs an error and returns an empty string if no format string is supplied.
    string = string_format ( 0 , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( 0 , string_length ( string ) );
    EXPECT_EQ ( 0 , *string );
    string_destroy ( string );

    // TEST 3: string_format logs an error and returns an empty string if the supplied variadic argument list is invalid.
    args_t args;
    args.arg_count = 1;
    args.args = 0;
    string = _string_format ( "" , args );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( 0 , string_length ( string ) );
    EXPECT_EQ ( 0 , *string );
    string_destroy ( string );

    // TEST 4: Raw (unsigned) format specifier.
    string = string_format ( "%u" , 23428476892 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "23428476892" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "23428476892" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 5: Memory address format specifier.
    string = string_format ( "%@" , 45763 /* not a real address */ );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "0xB2C3" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "0xB2C3" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 6: Memory address format specifier prints null pointer correctly.
    string = string_format ( "%@" , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "0x0" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "0x0" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 7: Null-terminated string format specifier.
    string = string_format ( "%s" , const_string_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( const_string_in ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , const_string_in , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 8: Resizable string format specifier (see container/string.h).
    string = string_format ( "%s%S" , string_in , string_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( 2 * string_length ( string_in ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , string_in , string_length ( string_in ) ) );
    EXPECT ( memory_equal ( string + string_length ( string_in ) , string_in , string_length ( string_in ) ) );
    string_destroy ( string );
    
    // Populate a relatively long string with random characters.
    for ( u64 i = 0; i < string_length ( really_long_string_in ); ++i )
    {
        really_long_string_in[ i ] = random2 ( 0 , 255 );
    }

    // TEST 9: string_format can handle relatively long strings without crashing.
    string = string_format ( "%S%S%S%S%S%S%S%S%S%S" , really_long_string_in , really_long_string_in , really_long_string_in , really_long_string_in , really_long_string_in , really_long_string_in , really_long_string_in , really_long_string_in , really_long_string_in , really_long_string_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( 10 * string_length ( really_long_string_in ) , string_length ( string ) );
    for ( u64 i = 0; i < 10; ++i )
    {
        EXPECT ( memory_equal ( string + i * string_length ( really_long_string_in ) , really_long_string_in , string_length ( really_long_string_in ) ) );
    }
    string_destroy ( string );

    // TEST 10: Signed integer format specifier. Positive number.
    string = string_format ( "%i" , 23428476892 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "23428476892" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "23428476892" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 11: Signed integer format specifier. Negative number.
    string = string_format ( "%i" , -23428476892 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "-23428476892" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "-23428476892" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 12: Signed integer format specifier, with hide-sign modifier. Positive number.
    string = string_format ( "%-i" , 23428476892 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "23428476892" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "23428476892" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 13: Signed integer format specifier, with hide-sign modifier. Negative number.
    string = string_format ( "%-i" , -23428476892 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "23428476892" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "23428476892" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 14: Signed integer format specifier, with show-sign modifier. Positive number.
    string = string_format ( "%+i" , 23428476892 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "+23428476892" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "+23428476892" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 15: Signed integer format specifier, with show-sign modifier. Negative number.
    string = string_format ( "%+i" , -23428476892 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "-23428476892" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "-23428476892" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 16: Floating point format specifier, with fix-precision modifier. Negative number.
    float_in = -100098.7893573;
    string = string_format ( "%.9f" , &float_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "-100098.789357300" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "-100098.789357300" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 17: Floating point format specifier, with fix-precision and show-sign modifiers. Negative number.
    float_in = -100098.7893573;
    string = string_format ( "%+.9f" , &float_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "-100098.789357300" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "-100098.789357300" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 18: Floating point format specifier, with fix-precision and hide-sign modifiers. Negative number.
    float_in = -100098.7893573;
    string = string_format ( "%-.9f" , &float_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "100098.789357300" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "100098.789357300" , string_length ( string ) ) );
    string_destroy ( string );
    
    // TEST 19: Floating point format specifier, with fix-precision modifier. Positive number.
    float_in = 100098.7893573;
    string = string_format ( "%.9f" , &float_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "100098.789357300" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "100098.789357300" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 20: Floating point format specifier, with fix-precision and show-sign modifiers. Positive number.
    float_in = 100098.7893573;
    string = string_format ( "%+.9f" , &float_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "+100098.789357300" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "+100098.789357300" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 21: Floating point format specifier, with fix-precision and hide-sign modifiers. Positive number.
    float_in = 100098.7893573;
    string = string_format ( "%-.9f" , &float_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "100098.789357300" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "100098.789357300" , string_length ( string ) ) );
    string_destroy ( string );
    
    // TEST 22: Floating point format specifier, with show-sign modifier. Positive number.
    float_in = 8723941230947.678234563498562343478952734523495893245723495782349057897563274632589346;
    string = string_format ( "%+f" , &float_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "+8723941230947.678710" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "+8723941230947.678710" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 23: Floating point (abbreviated-notation) format specifier, with fix-precision and show-sign modifiers. Positive number.
    // TODO: Fix rounding errors with string_f64. Should be +8.723941230947679E12
    float_in = 8723941230947.678234563498562343478952734523495893245723495782349057897563274632589346;
    string = string_format ( "%+.15e" , &float_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "+8.723941230947678E12" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "+8.723941230947678E12" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 24: Floating point (fractional only) format specifier.
    float_in = -100098.7893573;
    string = string_format ( "%.9d" , &float_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "789357300" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "789357300" , string_length ( string ) ) );
    string_destroy ( string );
    
    // TEST 25: Floating point (abbreviated-notation) format specifier.
    // TODO: Fix rounding errors with string_f64. Should be -1.000988E5.
    float_in = -100098.7893573;
    string = string_format ( "%e" , &float_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "-1.000987E5" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "-1.000987E5" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 26: Fixed-column-width format modifier, width == 80, pad left with '0'.
    float_in = -100098.7893573;
    string = string_format ( "`%Pl080.3f`" , &float_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "`000000000000000000000000000000000000000000000000000000000000000000000-100098.789`" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "`000000000000000000000000000000000000000000000000000000000000000000000-100098.789`" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 27: Fixed-column-width format modifier, width == 80, pad right with '0'.
    float_in = -100098.7893573;
    string = string_format ( "`%Pr080.3f`" , &float_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "`-100098.789000000000000000000000000000000000000000000000000000000000000000000000`" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "`-100098.789000000000000000000000000000000000000000000000000000000000000000000000`" , string_length ( string ) ) );
    string_destroy ( string );
    
    // TEST 28: Fixed-column-width format modifier, width == 14, pad left with newline.
    float_in = -100098.7893573;
    string = string_format ( "`%Pl\n14.3f`" , &float_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "`\n\n\n-100098.789`" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "`\n\n\n-100098.789`" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 29: Min-width format modifier, width <= string length, pad left with tab.
    string = string_format ( "%pl\t5s" , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello world!" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello world!" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 30: Min-width format modifier, width > string length, pad left with 'q'.
    string = string_format ( "%plq35s" , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "qqqqqqqqqqqqqqqqqqqqqqqHello world!" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "qqqqqqqqqqqqqqqqqqqqqqqHello world!" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 31: Min-width format modifier, width > string length, pad right with '.'.
    string = string_format ( "%pr.35s" , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello world!......................." ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello world!......................." , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 32: If the format string contains an unterminated format specifier, string_format ignores it.
    string = string_format ( "%;" , 25 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%;" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%;" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%s%%%%%Pl 1r2.%" , &"Hello world" , 25 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello world%%%Pl 1r2.%" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello world%%%Pl 1r2.%" , string_length ( string ) ) );
    string_destroy ( string );
    
    // TEST 33: If the format string contains an illegal format specifier, string_format ignores it.
    // NOTE: Fix-width format modifier only supports a single character for the padding character, followed by any number of digits for the width.
    string = string_format ( "`%P .3f`" , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "`%P .3f`" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "`%P .3f`" , string_length ( string ) ) );
    string_destroy ( string );
    
    // TEST 34: If the format string contains an illegal format specifier, string_format ignores it.
    // NOTE: Fix-width format modifier only supports a single character for the padding character, followed by any number of digits for the width.
    string = string_format ( "`%Pr 0.3F`" , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "`%Pr 0.3F`" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "`%Pr 0.3F`" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 35: Show-sign and hide-sign format modifiers do not affect inapplicable types.
    string = string_format ( "%+s" , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello world!" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello world!" , string_length ( string ) ) );
    string_destroy ( string );
    string = string_format ( "%-s" , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello world!" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello world!" , string_length ( string ) ) );
    string_destroy ( string );
    
    // TEST 36: Fix-precision format modifier does not affect inapplicable types.
    string = string_format ( "%-.7@ljldkb]l-045[pwrsg439p80tu[]" , 45763 /* not a real address */ );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "0xB2C3ljldkb]l-045[pwrsg439p80tu[]" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "0xB2C3ljldkb]l-045[pwrsg439p80tu[]" , string_length ( string ) ) );
    string_destroy ( string );
    
    // TEST 37: A format modifier which overwrites a previous format modifier of the same type on the same format specifier will be considered invalid.
    string = string_format ( "%.2.8f" , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%.2.8f" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%.2.8f" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%+-iSsfa\\n\nm``lpmr2kl\r\t" , -23428476892 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%+-iSsfa\\n\nm``lpmr2kl\r\t" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%+-iSsfa\\n\nm``lpmr2kl\r\t" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x3 )
    string = string_format ( "%Plr0i" , -23428476892 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%Plr0i" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%Plr0i" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x4 )
    string = string_format ( "%Pl 190234.6+Pr190234i" , -23428476892 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%Pl 190234.6+Pr190234i" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%Pl 190234.6+Pr190234i" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x5 )
    string = string_format ( "%.2AAF" , 0 , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%.2AAF" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%.2AAF" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x6 )
    string = string_format ( "%.2aaF" , 0 , 0 , 0 , 0 , 0 , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%.2aaF" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%.2aaF" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 38: string_format can handle null pointers for the following format specifiers: %s, %S, %f, %F, %e, %d, %file, %A.
    string = string_format ( "%s" , 0 );
    EXPECT_NEQ ( 0 , string );
    string_destroy ( string );
    string = string_format ( "%S" , 0 );
    EXPECT_NEQ ( 0 , string );
    string_destroy ( string );
    string = string_format ( "%f" , 0 );
    EXPECT_NEQ ( 0 , string );
    string_destroy ( string );
    string = string_format ( "%F" , 0 );
    EXPECT_NEQ ( 0 , string );
    string_destroy ( string );
    string = string_format ( "%e" , 0 );
    EXPECT_NEQ ( 0 , string );
    string_destroy ( string );
    string = string_format ( "%d" , 0 );
    EXPECT_NEQ ( 0 , string );
    string_destroy ( string );
    string = string_format ( "%file" , 0 );
    EXPECT_NEQ ( 0 , string );
    string_destroy ( string );
    string = string_format ( "%Ai" , 0 );
    EXPECT_NEQ ( 0 , string );
    string_destroy ( string );

    // TEST 39: Floating point format specifier, with fix-precision and resizable array format modifiers.
    string = string_format ( "%.2AF" , f32_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ -8.00, -7.00, -6.00, -5.00, -4.00, -3.00, -2.00, -1.00, 0.00, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ -8.00, -7.00, -6.00, -5.00, -4.00, -3.00, -2.00, -1.00, 0.00, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00 }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 40: Integer format specifier, with minimum-column-width and resizable array format modifiers.
    string = string_format ( "%Apl 2i" , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ -8, -7, -6, -5, -4, -3, -2, -1,  0,  1,  2,  3,  4,  5,  6,  7 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ -8, -7, -6, -5, -4, -3, -2, -1,  0,  1,  2,  3,  4,  5,  6,  7 }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 41: Character format specifier, with resizable array format modifier.
    string = string_format ( "%Ac" , string_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ `H`, `e`, `l`, `l`, `o`, ` `, `w`, `o`, `r`, `l`, `d`, `!`, `` }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ `H`, `e`, `l`, `l`, `o`, ` `, `w`, `o`, `r`, `l`, `d`, `!`, `` }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 42: Integer format specifier, with radix format modifier.
    string = string_format ( "%r2i" , -23428476892 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "1111111111111111111111111111101010001011100011011101000000100100" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "1111111111111111111111111111101010001011100011011101000000100100" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 43: Integer format specifier, with radix format modifier.
    string = string_format ( "%r36i" , -23428476892 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "3W5E10REO2WBO" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "3W5E10REO2WBO" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 44: Format specifiers which have the same starting characters are parsed correctly.
    // ( x1 ) File format specifier + floating point format specifier
    float_in = -1.0;
    string = string_format ( "%+filfile%file." , &float_in , &file_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "-1ilfile" ) + _string_length ( invalid_file_out ) + _string_length ( "." ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "-1ilfile" , _string_length ( "-1ilfile" ) ) );
    EXPECT ( memory_equal ( string + _string_length ( "-1ilfile" ) , invalid_file_out , _string_length ( invalid_file_out ) ) );
    EXPECT ( memory_equal ( string + _string_length ( "-1ilfile" ) + _string_length ( invalid_file_out ) , "." , _string_length ( "." ) ) );
    string_destroy ( string );
    // ( x2 ) Bytesize format specifier + string format specifier
    string = string_format ( "%.2siz%.2size" , &"Hello world!" , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello world!iz0 B" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello world!iz0 B" , _string_length ( "Hello world!iz0 B" ) ) );
    string_destroy ( string );
    
    // TEST 45: If the format string contains an illegal format specifier, string_format ignores it.
    // NOTE: Fix-precision floating point format modifier only supports up to 20 digits of precision.
    string = string_format ( "`%.99F`" , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "`%.99F`" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "`%.99F`" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 46: If the format string contains an illegal format specifier, string_format ignores it.
    // NOTE: Radix format modifier only supports bases 2-36.
    string = string_format ( "%r37i %r1i" , 0 , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%r37i %r1i" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%r37i %r1i" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 47: Floating point format specifier, with fix-precision and fixed-length array format modifiers.
    string = string_format ( "%.2aF" , const_f32_array_in , 16 , sizeof ( f32 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ -8.00, -7.00, -6.00, -5.00, -4.00, -3.00, -2.00, -1.00, 0.00, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ -8.00, -7.00, -6.00, -5.00, -4.00, -3.00, -2.00, -1.00, 0.00, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00 }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 48: Integer format specifier, with minimum-column-width and fixed-length array format modifiers.
    string = string_format ( "%apl 2i" , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ -8, -7, -6, -5, -4, -3, -2, -1,  0,  1,  2,  3,  4,  5,  6,  7 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ -8, -7, -6, -5, -4, -3, -2, -1,  0,  1,  2,  3,  4,  5,  6,  7 }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 49: Character format specifier, with fixed-length array format modifier.
    string = string_format ( "%ac" , const_string_in , _string_length ( const_string_in ) + 1 , sizeof ( char ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ `H`, `e`, `l`, `l`, `o`, ` `, `w`, `o`, `r`, `l`, `d`, `!`, `` }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ `H`, `e`, `l`, `l`, `o`, ` `, `w`, `o`, `r`, `l`, `d`, `!`, `` }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 50: Fixed-length array format modifier causes string_format to fail if not enough arguments are provided.
    string = string_format ( "%i %ac" , 0 , const_string_in , _string_length ( const_string_in ) + 1 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "0 %ac" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "0 %ac" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 51: Floating point format specifier, with wildcard fix-precision format modifier.
    float_in = -100098.7893573;
    string = string_format ( "%.?F" , 11 , &float_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "-100098.78935730000" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "-100098.78935730000" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 52: Integer format specifier, with wildcard radix format modifier.
    string = string_format ( "%r?i" , 36 , -23428476892 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "3W5E10REO2WBO" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "3W5E10REO2WBO" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 53: Fix-column-width format modifier, width == 24, pad wildcard with whitespace.
    string = string_format ( "%P? 24s" , &"r" , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello world!            " ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello world!            " , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 54: Fix-column-width + min-column-width format modifier, width == 24, pad with wildcard.
    string = string_format ( "%Pl?24s%pr?24s" , ' ' , &"Hello world!" , '.' , &":" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "            Hello world!:......................." ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "            Hello world!:......................." , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 55: Fix-column-width format modifier, wildcard width, pad with whitespace.
    string = string_format ( "%Pl ?s%Pr ?s" , 4 , &"Hello world!" , 2 , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "HellHe" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "HellHe" , string_length ( string ) ) );
    string_destroy ( string );
    
    // TEST 56: Fix-column-width format modifier, multiple wildcards.
    string = string_format ( "%P???s%P???s" , &"l" , '4' , 4 , &"Hello world!" , &"r" , '2' , 2 , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "HellHe" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "HellHe" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 57: Wildcards cause string_format to fail if not enough arguments are provided.
    string = string_format ( "%P???s%P???r4i" , &"l" , '4' , 4 , &"Hello world!" , &"r" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hell%P???r4i" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hell%P???r4i" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 58: Nested format substring, fix-column width format modifier with multiple wildcards.
    string = string_format ( "%Pl??{}" , '4' , 11 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "44444444444" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "44444444444" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 59: Nested format substring, min-column width format modifier with multiple wildcards.
    float_in = -999416.9999875;
    string = string_format ( "%pl??{%i\t\t%.?f\t\t%s}" , '*' , 48 , -23428476892 , 7 , &float_in , &"^v" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "***************-23428476892\t\t-999416.9999875\t\t^v" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "***************-23428476892\t\t-999416.9999875\t\t^v" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 60: Nested format substring, fix-column width format modifier can handle nest depth > 0 and escape sequences for printing `{` and `}` characters without changing the depth.
    string = string_format ( "%Pl??{ \\{\\} %Pr??{ %s \\{\\} } }%Pl??{%ac}%Pr??{ %s \\{\\} }" , ' ' , 45 , ' ' , 32 , &"Hello world!" , ' ' , 32 , &"Hello world!" , _string_length ( "Hello world!" ) + 1 , sizeof ( char ) , ' ' , 32 , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "         {}  Hello world! {}                 { `H`, `e`, `l`, `l`, `o`, ` `,  Hello world! {}                " ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "         {}  Hello world! {}                 { `H`, `e`, `l`, `l`, `o`, ` `,  Hello world! {}                " , string_length ( string ) ) );
    string_destroy ( string );
 
    // TEST 61: Nested format substrings with unbalanced braces cause string_format to fail.
    string = string_format ( "%Pl??{%pl??{%Pl??{%pl??{%pl??{%Pl??{%Pl??{}}}}}}" , '0' , 32 , '0' , 32 , '0' , 32 , '0' , 32 , '0' , 32 , '0' , 32 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%Pl??{%pl??{%Pl??{%pl??{%pl??{%Pl??{%Pl??{}}}}}}" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%Pl??{%pl??{%Pl??{%pl??{%pl??{%Pl??{%Pl??{}}}}}}" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 62: string_format can jump over arguments if an invalid format specifier is provided while still consuming other arguments which correspond to valid format specifiers.
    float_in = 1.5;
    string = string_format ( "`_%Pr%1478{`%i %c %+.1f %r2i %r4? %s %Pl.2c %S`}_`" , 16 , '~' , &float_in , 3 , '!' , &"##" , ',' , string_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "`_`16 ~ +1.5 11 %r4? ## ., Hello world!`%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%_`" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "`_`16 ~ +1.5 11 %r4? ## ., Hello world!`%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%_`" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 63: Wildcard token (i.e. `?`) for the padding character may be escaped to use the token itself as the padding character.
    string = string_format ( "%p?\\??{;}" , &"l" , 10 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "?????????;" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "?????????;" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 64: Wildcard for the padding character may be used to pass the token itself as the argument from which to parse the padding character.
    string = string_format ( "%p???{;}" , &"l" , '?' , 10 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "?????????;" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "?????????;" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 65: Fix-width format modifier, pad with multi-character strings.
    string = string_format ( "%Pl'-='14{} SAMPLE TEXT %Pr'=-'14{}" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "-=-=-=-=-=-=-= SAMPLE TEXT =-=-=-=-=-=-=-" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "-=-=-=-=-=-=-= SAMPLE TEXT =-=-=-=-=-=-=-" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 66: Multi-character padding token (i.e. `'`) for the padding character may be escaped to use the token itself as the padding character.
    string = string_format ( "%pl\\'4i" , 2 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "'''2" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "'''2" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 67: Multi-character padding token (i.e. `'`) may be escaped within a multi-character padding token to use the token itself within the padding string.
    string = string_format ( "%pl'\\'Escape token\\' '30i" , 2 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "'Escape token' 'Escape token'2" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "'Escape token' 'Escape token'2" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%pr'\\'Escape token\\' \\'\\' '30i" , 2 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "2'Escape token' '' 'Escape tok" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "2'Escape token' '' 'Escape tok" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 68: Wildcard may be used within a multi-character padding token to read the value of the padding string from an argument.
    string = string_format ( "%pl'?'30i" , &"'Escape token' " , 2 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "'Escape token' 'Escape token'2" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "'Escape token' 'Escape token'2" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 69: When a wildcard is used to parse a multi-character padding string, the multi-character escape token (i.e. `\'`) is not processed (i.e. printed verbatim).
    string = string_format ( "%pl'?'60i" , &"\\'Escape token\\' \\'\\''' " , 2 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "\\'Escape token\\' \\'\\''' \\'Escape token\\' \\'\\''' \\'Escape to2" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "\\'Escape token\\' \\'\\''' \\'Escape token\\' \\'\\''' \\'Escape to2" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%pr'?'60i" , &"\\'Escape token\\' \\'\\''' " , 2 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "2\\'Escape token\\' \\'\\''' \\'Escape token\\' \\'\\''' \\'Escape to" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "2\\'Escape token\\' \\'\\''' \\'Escape token\\' \\'\\''' \\'Escape to" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 70: Wildcard token (i.e. `?`) may be escaped within a multi-character padding string to use the token itself as the padding string.
    string = string_format ( "%p?'\\?'?{;}" , &"l" , 10 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "?????????;" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "?????????;" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 71: Multi-character padding strings containing wildcard tokens (i.e. `?`) which are **not an exact match** to the token (i.e. `?`) do not need to be escaped.
    string = string_format ( "%p?'?\\?'?{;}" , &"l" , 10 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "?\\??\\??\\?;" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "?\\??\\??\\?;" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 72: Format specifiers contained within multi-character padding strings are printed verbatim.
    string = string_format ( "%p?'%i%s'?{;}" , &"l" , 10 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%i%s%i%s%;" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%i%s%i%s%;" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 73: Format specifiers contained within **invalid** multi-character padding strings are printed verbatim (not evaluated).
    string = string_format ( "%pl'%i%s'1r1i" , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%pl'%i%s'1r1i" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%pl'%i%s'1r1i" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 74: An empty multi-character padding string will cause the format modifier to be invalidated.
    string = string_format ( "%pl''10{;}" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%pl''10{;}" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%pl''10{;}" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 75: An empty multi-character padding string (from wildcard) will cause the format modifier to be invalidated.
    string = string_format ( "%pl'?'10{;}" , &"" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%pl'?'10{;}" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%pl'?'10{;}" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 76: Multi-character padding format modifiers support the use of ANSI formatting codes included in the padding string.
    string = string_format ( "%Pl'?'?{} r %Pr-1{}%Pl'?'?{} g %Pr-1{}%Pl'?'?{} b %Pr-1{}%Pl'?'?{} a %Pr-1{}"
                           , &"" ANSI_CC2 ( ANSI_CC_BG_RED , ANSI_CC_FG_BLACK ) "-" , _string_length ( "" ANSI_CC2 ( ANSI_CC_BG_RED , ANSI_CC_FG_BLACK ) "-" )
                           , &"" ANSI_CC2 ( ANSI_CC_BG_GREEN , ANSI_CC_FG_BLACK ) "-" , _string_length ( "" ANSI_CC2 ( ANSI_CC_BG_GREEN , ANSI_CC_FG_BLACK ) "-" )
                           , &"" ANSI_CC2 ( ANSI_CC_BG_BLUE , ANSI_CC_FG_BLACK ) "-" , _string_length ( "" ANSI_CC2 ( ANSI_CC_BG_BLUE , ANSI_CC_FG_BLACK ) "-" )
                           , &"" ANSI_CC2 ( ANSI_CC_BG_WHITE , ANSI_CC_FG_BLACK ) "-" , _string_length ( "" ANSI_CC2 ( ANSI_CC_BG_WHITE , ANSI_CC_FG_BLACK ) "-" )
                           );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( ANSI_CC2 ( ANSI_CC_BG_RED , ANSI_CC_FG_BLACK ) "- r -" ANSI_CC2 ( ANSI_CC_BG_GREEN , ANSI_CC_FG_BLACK ) "- g -" ANSI_CC2 ( ANSI_CC_BG_BLUE , ANSI_CC_FG_BLACK ) "- b -" ANSI_CC2 ( ANSI_CC_BG_WHITE , ANSI_CC_FG_BLACK ) "- a -" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , ANSI_CC2 ( ANSI_CC_BG_RED , ANSI_CC_FG_BLACK ) "- r -" ANSI_CC2 ( ANSI_CC_BG_GREEN , ANSI_CC_FG_BLACK ) "- g -" ANSI_CC2 ( ANSI_CC_BG_BLUE , ANSI_CC_FG_BLACK ) "- b -" ANSI_CC2 ( ANSI_CC_BG_WHITE , ANSI_CC_FG_BLACK ) "- a -" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 77: Padding format modifiers supports using backslash for the padding character, given that the character is not followed by an escape sequence (i.e. `\?` or `\'`).
    string = string_format ( "%Pl\\16r3i" , 10 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "\\\\\\\\\\\\\\\\\\\\\\\\\\101" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "\\\\\\\\\\\\\\\\\\\\\\\\\\101" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 78: Array slice format modifier prints the entire array if supplied no start and end index.
    string = string_format ( "%[:]ai" , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7 }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 79: Array slice format modifier prints the correct range of elements from the array.
    string = string_format ( "%Pl03r2[8:10]ai\n%Pl03r2[10:12]ai\n%Pl03r2[12:14]ai" , const_i8_array_in , 16 , sizeof ( i8 ) , const_i8_array_in , 16 , sizeof ( i8 ) , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ 000, 001 }\n{ 010, 011 }\n{ 100, 101 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ 000, 001 }\n{ 010, 011 }\n{ 100, 101 }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 80: Array slice format modifier prints a single array element if no index separator is provided.
    string = string_format ( "%Pl08r2[8]ai" , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ 00000000 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ 00000000 }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 81: Array slice format modifier prints no array elements if start and end index are identical.
    string = string_format ( "%[0:0]ai" , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{  }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{  }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 82: Array slice format modifier supports wildcards for either index of the range.
    string = string_format ( "%[?:]ai" , 8 , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ 0, 1, 2, 3, 4, 5, 6, 7 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ 0, 1, 2, 3, 4, 5, 6, 7 }" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%[:?]ai" , 9 , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ -8, -7, -6, -5, -4, -3, -2, -1, 0 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ -8, -7, -6, -5, -4, -3, -2, -1, 0 }" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x3 )
    string = string_format ( "%[?:?]ai" , 0 , 16 , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7 }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 83: Resizable array slice format modifier prints the entire array if supplied no start and end index.
    string = string_format ( "%[:]Ai" , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7 }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 84: Resizable array slice format modifier prints the correct range of elements from the array.
    string = string_format ( "%Pl03r2[8:10]Ai\n%Pl03r2[10:12]Ai\n%Pl03r2[12:14]Ai" , i8_array_in , i8_array_in , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ 000, 001 }\n{ 010, 011 }\n{ 100, 101 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ 000, 001 }\n{ 010, 011 }\n{ 100, 101 }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 85: Resizable array slice format modifier prints a single array element if no index separator is provided.
    string = string_format ( "%Pl08r2[8]Ai" , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ 00000000 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ 00000000 }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 86: Resizable array slice format modifier prints no array elements if start and end index are identical.
    string = string_format ( "%[0:0]Ai" , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{  }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{  }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 87: Resizable array slice format modifier supports wildcards for either index of the range.
    string = string_format ( "%[?:]Ai" , 8 , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ 0, 1, 2, 3, 4, 5, 6, 7 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ 0, 1, 2, 3, 4, 5, 6, 7 }" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%[:?]Ai" , 9 , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ -8, -7, -6, -5, -4, -3, -2, -1, 0 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ -8, -7, -6, -5, -4, -3, -2, -1, 0 }" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x3 )
    string = string_format ( "%[?:?]Ai" , 0 , array_length ( i8_array_in ) , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7 }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7 }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 88: String slice format modifier prints the entire string if supplied no start and end index.
    string = string_format ( "%[:]s" , &"abcdefghijklmnopqrstuvwxyz" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "abcdefghijklmnopqrstuvwxyz" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "abcdefghijklmnopqrstuvwxyz" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 89: String slice format modifier prints the correct range of characters from the string.
    string = string_format ( "%[1:4]s%[0:2]s%[9:12]s" , const_string_in , const_string_in , const_string_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "ellHeld!" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "ellHeld!" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 90: String slice format modifier prints a single character if no index separator is provided.
    string = string_format ( "%[7]s" , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "o" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "o" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 91: String slice format modifier prints nothing if start and end index are identical.
    string = string_format ( "%[0:0]s" , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 92: String slice format modifier supports wildcards for either index of the range.
    string = string_format ( "%[?:]s" , 6 , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "world!" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "world!" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%[:?]s" , 5 , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x3 )
    string = string_format ( "%[?:?]s" , 0 , _string_length ( const_string_in ) , const_string_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( const_string_in ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , const_string_in , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 93: Resizable string slice format modifier prints the entire string if supplied no start and end index.
    string = string_format ( "%[:]S" , string_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( string_length ( string_in ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , string_in , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 94: Resizable string slice format modifier prints the correct range of characters from the string.
    string = string_format ( "%[1:4]S%[0:2]S%[9:12]S" , string_in , string_in , string_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "ellHeld!" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "ellHeld!" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 95: Resizable string slice format modifier prints a single character if no index separator is provided.
    string = string_format ( "%[7]S" , string_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "o" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "o" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 96: Resizable string slice format modifier prints nothing if start and end index are identical.
    string = string_format ( "%[0:0]S" , string_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 97: Resizable string slice format modifier supports wildcards for either index of the range.
    string = string_format ( "%[?:]S" , 6 , string_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "world!" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "world!" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%[:?]S" , 5 , string_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x3 )
    string = string_format ( "%[?:?]S" , 0 , string_length ( string_in ) , string_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( string_length ( string_in ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , string_in , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 98: Slice format modifier is invalidated if no lower index is provided.
    string = string_format ( "%[]s" , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[]s" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[]s" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%[ ]s" , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[ ]s" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[ ]s" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x3 )
    string = string_format ( "%[0  ]s" , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[0  ]s" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[0  ]s" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 99: Slice format modifier is invalidated if the provided range is invalid.
    //          (Lower index exceeds upper index)
    string = string_format ( "%[1:0]s" , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[1:0]s" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[1:0]s" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%[11:10]s" , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[11:10]s" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[11:10]s" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 100: Slice format modifier is invalidated if the provided range is invalid.
    //           (Either index negative)
    string = string_format ( "%[-1:]s" , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[-1:]s" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[-1:]s" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%[:-1]s" , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[:-1]s" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[:-1]s" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 101: Slice format modifier is invalidated if the provided range is invalid.
    //           (Either index exceeds collection length).
    string = string_format ( "%[?]s" , _string_length ( "Hello world!" ) + 1 , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[?]s" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[?]s" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%[:?]s" , _string_length ( "Hello world!" ) + 1 , &"Hello world!" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[:?]s" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[:?]s" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 102: Slice format modifier is invalidated if not immediately **succeeded** by an array or string format specifier.
    string = string_format ( "%.2[7]F" , float_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%.2[7]F" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%.2[7]F" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%.2A[7]F" , f32_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%.2A[7]F" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%.2A[7]F" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 103: Array and resizable array format modifiers are mutually-exclusive.
    string = string_format ( "%aAi" , 0 , 0 , 0 , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%aAi" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%aAi" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%Aai" , 0 , 0 , 0 , 0 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%Aai" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%Aai" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 104: Array format modifier correctly prints an array of null-terminated strings.
    string = string_format ( "%as" , const_string_array_in , 4 , sizeof ( char* ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ `Hello`, ` `, `world`, `!` }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ `Hello`, ` `, `world`, `!` }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 105: Resizable array format modifier correctly prints an array of null-terminated strings.
    string = string_format ( "%As" , string_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ `Hello`, ` `, `world`, `!` }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ `Hello`, ` `, `world`, `!` }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 106: Array slice format modifier correctly prints a range of elements from an array of null-terminated strings.
    string = string_format ( "%[2:]as" , const_string_array_in , 4 , sizeof ( char* ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ `world`, `!` }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ `world`, `!` }" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 107: Resizable array slice format modifier correctly prints a range of elements from an array of null-terminated strings.
    string = string_format ( "%[2:]As" , string_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "{ `world`, `!` }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "{ `world`, `!` }" , string_length ( string ) ) );
    string_destroy ( string );
 
    // TEST 108: Array format modifier with custom array start, terminator, and separator strings.
    string = string_format ( "%a[(\n\t|_|\n)]s" , const_string_array_in , 4 , sizeof ( char* ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "(\n\tHello_ _world_!\n)" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "(\n\tHello_ _world_!\n)" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%[8:]a[THE ARRAY:\n{\n\t|\n\t|\n}].1F" , const_f32_array_in , 16 , sizeof ( f32 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "THE ARRAY:\n{\n\t0.0\n\t1.0\n\t2.0\n\t3.0\n\t4.0\n\t5.0\n\t6.0\n\t7.0\n}" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "THE ARRAY:\n{\n\t0.0\n\t1.0\n\t2.0\n\t3.0\n\t4.0\n\t5.0\n\t6.0\n\t7.0\n}" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 109: Array format modifier with no array start, terminator, and separator strings.
    string = string_format ( "%a[||]c" , const_string_in , _string_length ( const_string_in ) , sizeof ( char ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello world!" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello world!" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%a[]c" , const_string_in , _string_length ( const_string_in ) , sizeof ( char ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello world!" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello world!" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 110: Array format modifier with custom array start, terminator, and separator strings containing escape sequences `\[` and `\]`.
    string = string_format ( "%[:8]a[\\[ | \\[\\] | \\]]i" , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "[ -8 [] -7 [] -6 [] -5 [] -4 [] -3 [] -2 [] -1 ]" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "[ -8 [] -7 [] -6 [] -5 [] -4 [] -3 [] -2 [] -1 ]" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 111: Array format modifier with custom array start, terminator, and separator strings containing escape sequence `\|`.
    string = string_format ( "%[:8]a[\\| | \\| | \\|]i" , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "| -8 | -7 | -6 | -5 | -4 | -3 | -2 | -1 |" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "| -8 | -7 | -6 | -5 | -4 | -3 | -2 | -1 |" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 112: Array format modifier supports reading custom array start, terminator, and separator strings from wildcard.
    string = string_format ( "%[:8]a[?|?|?]i" , const_i8_array_in , 16 , sizeof ( i8 ) , &"( " , &" *** " , &" )" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "( -8 *** -7 *** -6 *** -5 *** -4 *** -3 *** -2 *** -1 )" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "( -8 *** -7 *** -6 *** -5 *** -4 *** -3 *** -2 *** -1 )" , string_length ( string ) ) );
    string_destroy ( string );
    
    // TEST 113: Array format modifier which uses `|`, `[` or `]` tokens as part of an array start, terminator, or separator string does not have to escape these tokens if they are read from a wildcard.
    string = string_format ( "%[:8]a[?|?|?]i" , const_i8_array_in , 16 , sizeof ( i8 ) , &"\\|\\[\n|[ " , &" ]|[ " , &" ]|\n\\]\\|" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "\\|\\[\n|[ -8 ]|[ -7 ]|[ -6 ]|[ -5 ]|[ -4 ]|[ -3 ]|[ -2 ]|[ -1 ]|\n\\]\\|" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "\\|\\[\n|[ -8 ]|[ -7 ]|[ -6 ]|[ -5 ]|[ -4 ]|[ -3 ]|[ -2 ]|[ -1 ]|\n\\]\\|" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 114: Array format modifier supports using `\?` to print `?` as part of a custom array start, terminator, or separator string instead of reading from wildcard.
    string = string_format ( "%[0:2]a[|\\?|]i" , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "-8?-7" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "-8?-7" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 115: Array format modifier which uses `?` token as part of an array start, terminator, or separator string does not have to escape this token if it is read from a wildcard.
    string = string_format ( "%[0:2]a[?|?|?]i" , const_i8_array_in , 16 , sizeof ( i8 ) , &"\\?\n  " , &"?" , &"  \n\\?" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "\\?\n  -8?-7  \n\\?" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "\\?\n  -8?-7  \n\\?" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 116: array start, terminator, or separator strings containing wildcard tokens (i.e. `?`) which are **not an exact match** to the token (i.e. `?`) do not need to be escaped.
    string = string_format ( "%[2:5]a[|? |\\? ]i" , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "-6? -5? -4\\? " ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "-6? -5? -4\\? " , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 117: Array format modifier is invalidated if a custom array start, terminator, or separator string is present but the number of separator tokens is incorrect.
    string = string_format ( "%[2:5]a[|]i" , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[2:5]a[|]i" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[2:5]a[|]i" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%[2:5]a[|||]i" , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[2:5]a[|||]i" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[2:5]a[|||]i" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x3 )
    string = string_format ( "%[2:5]a[ ]i" , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[2:5]a[ ]i" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[2:5]a[ ]i" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 118: Array format modifier is invalidated if a custom array start, terminator, or separator string is present but the terminating bracket cannot be found.
    string = string_format ( "%[2:5]a[||i" , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[2:5]a[||i" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[2:5]a[||i" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 119: Array format modifier is invalidated if a custom array start, terminator, or separator string is present but one of them contains an illegal, unescaped `[` token.
    string = string_format ( "%[2:5]a[|[|i" , const_i8_array_in , 16 , sizeof ( i8 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[2:5]a[|[|i" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[2:5]a[|[|i" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 120: Resizable array format modifier with custom array start, terminator, and separator strings.
    string = string_format ( "%A[(\n\t|_|\n)]s" , string_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "(\n\tHello_ _world_!\n)" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "(\n\tHello_ _world_!\n)" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%[8:]A[THE ARRAY:\n{\n\t|\n\t|\n}].1F" , f32_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "THE ARRAY:\n{\n\t0.0\n\t1.0\n\t2.0\n\t3.0\n\t4.0\n\t5.0\n\t6.0\n\t7.0\n}" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "THE ARRAY:\n{\n\t0.0\n\t1.0\n\t2.0\n\t3.0\n\t4.0\n\t5.0\n\t6.0\n\t7.0\n}" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 121: Resizable array format modifier with no array start, terminator, and separator strings.
    string = string_format ( "%A[||]s" , string_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello world!" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello world!" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%A[]s" , string_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "Hello world!" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "Hello world!" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 122: Resizable array format modifier with custom array start, terminator, and separator strings containing escape sequences `\[` and `\]`.
    string = string_format ( "%[:8]A[\\[ | \\[\\] | \\]]i" , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "[ -8 [] -7 [] -6 [] -5 [] -4 [] -3 [] -2 [] -1 ]" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "[ -8 [] -7 [] -6 [] -5 [] -4 [] -3 [] -2 [] -1 ]" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 123: Resizable array format modifier with custom array start, terminator, and separator strings containing escape sequence `\|`.
    string = string_format ( "%[:8]A[\\| | \\| | \\|]i" , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "| -8 | -7 | -6 | -5 | -4 | -3 | -2 | -1 |" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "| -8 | -7 | -6 | -5 | -4 | -3 | -2 | -1 |" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 124: Resizable array format modifier supports reading custom array start, terminator, and separator strings from wildcard.
    string = string_format ( "%[:8]A[?|?|?]i" , i8_array_in , &"( " , &" *** " , &" )" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "( -8 *** -7 *** -6 *** -5 *** -4 *** -3 *** -2 *** -1 )" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "( -8 *** -7 *** -6 *** -5 *** -4 *** -3 *** -2 *** -1 )" , string_length ( string ) ) );
    string_destroy ( string );
    
    // TEST 125: Resizable array format modifier which uses `|`, `[` or `]` tokens as part of an array start, terminator, or separator string does not have to escape these tokens if they are read from a wildcard.
    string = string_format ( "%[:8]A[?|?|?]i" , i8_array_in , &"\\|\\[\n|[ " , &" ]|[ " , &" ]|\n\\]\\|" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "\\|\\[\n|[ -8 ]|[ -7 ]|[ -6 ]|[ -5 ]|[ -4 ]|[ -3 ]|[ -2 ]|[ -1 ]|\n\\]\\|" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "\\|\\[\n|[ -8 ]|[ -7 ]|[ -6 ]|[ -5 ]|[ -4 ]|[ -3 ]|[ -2 ]|[ -1 ]|\n\\]\\|" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 126: Resizable array format modifier supports using `\?` to print `?` as part of a custom array start, terminator, or separator string instead of reading from wildcard.
    string = string_format ( "%[0:2]A[|\\?|]i" , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "-8?-7" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "-8?-7" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 127: Resizable array format modifier which uses `?` token as part of an array start, terminator, or separator string does not have to escape this token if it is read from a wildcard.
    string = string_format ( "%[0:2]A[?|?|?]i" , i8_array_in , &"\\?\n  " , &"?" , &"  \n\\?" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "\\?\n  -8?-7  \n\\?" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "\\?\n  -8?-7  \n\\?" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 128: Resizable array start, terminator, or separator strings containing wildcard tokens (i.e. `?`) which are **not an exact match** to the token (i.e. `?`) do not need to be escaped.
    string = string_format ( "%[2:5]A[|? |\\?]i" , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "-6? -5? -4?" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "-6? -5? -4?" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 129: Resizable array format modifier is invalidated if a custom array start, terminator, or separator string is present but the number of separator tokens is incorrect.
    string = string_format ( "%[2:5]A[|]i" , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[2:5]A[|]i" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[2:5]A[|]i" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%[2:5]A[|||]i" , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[2:5]A[|||]i" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[2:5]A[|||]i" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x3 )
    string = string_format ( "%[2:5]A[ ]i" , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[2:5]A[ ]i" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[2:5]A[ ]i" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 130: Resizable array format modifier is invalidated if a custom array start, terminator, or separator string is present but the terminating bracket cannot be found.
    string = string_format ( "%[2:5]A[||i" , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[2:5]A[||i" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[2:5]A[||i" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 131: Resizable array format modifier is invalidated if a custom array start, terminator, or separator string is present but one of them contains an illegal, unescaped `[` token.
    string = string_format ( "%[2:5]A[|[|i" , i8_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%[2:5]A[|[|i" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%[2:5]A[|[|i" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 132: An invalid format specifier containing retroactive wildcards, or otherwise corresponding to multiple arguments in the variadic argument list, results in the correct number of arguments being skipped.
    float_in = 27.5;
    string = string_format ( "%i%c %.2f %[0:]a[?| |?]r1u %i %[1:8]A[?| |\\|\\]].0002F %s %i" , -4 , 'w' , &float_in , const_i8_array_in , 16 , sizeof ( i8 ) , &"| " , &" |" , 24 , f32_array_in , &"[]" , &"Hello world!" , -8 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "-4w 27.50 %[0:]a[?| |?]r1u 24 %[1:8]A[?| |\\|\\]].0002F Hello world! -8" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "-4w 27.50 %[0:]a[?| |?]r1u 24 %[1:8]A[?| |\\|\\]].0002F Hello world! -8" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 133: Boolean format specifier.
    string = string_format ( "%B %B %B" , false , true , -1 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "False True True" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "False True True" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 134: Truncated boolean format specifier.
    string = string_format ( "%b %b %b" , false , true , -1 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "F T T" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "F T T" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 135: File format modifier, with fixed-length array format modifier.
    string = string_format ( "%a[]file" , const_file_array_in , 3 , sizeof ( file_t ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( 3 * _string_length ( invalid_file_out )  , string_length ( string ) );
    EXPECT ( memory_equal ( string , invalid_file_out , _string_length ( invalid_file_out ) ) );
    EXPECT ( memory_equal ( string + _string_length ( invalid_file_out ) , invalid_file_out , _string_length ( invalid_file_out ) ) );
    EXPECT ( memory_equal ( string + _string_length ( invalid_file_out ) + _string_length ( invalid_file_out ) , invalid_file_out , _string_length ( invalid_file_out ) ) );
    string_destroy ( string );

    // TEST 136: File format modifier, with resizable array format modifier.
    string = string_format ( "%A[]file" , file_array_in );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( 3 * _string_length ( invalid_file_out )  , string_length ( string ) );
    EXPECT ( memory_equal ( string , invalid_file_out , _string_length ( invalid_file_out ) ) );
    EXPECT ( memory_equal ( string + _string_length ( invalid_file_out ) , invalid_file_out , _string_length ( invalid_file_out ) ) );
    EXPECT ( memory_equal ( string + _string_length ( invalid_file_out ) + _string_length ( invalid_file_out ) , invalid_file_out , _string_length ( invalid_file_out ) ) );
    string_destroy ( string );

    // TEST 137: Bytesize format specifier.
    string = string_format ( "%size | %size | %size | %size | %size | %size | %size | %size" , KiB ( 1 ) - 1 , KiB ( 1 ) , MiB ( 1 ) - 1 , MiB ( 1 ) , GiB ( 1 ) - 1 , GiB ( 1 ) , KiB ( 1400 ) , GiB ( 400.25 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "1023 B | 1 KiB | 1023.999023 KiB | 1 MiB | 1023.999999 MiB | 1 GiB | 1.367187 MiB | 400.250000 GiB" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "1023 B | 1 KiB | 1023.999023 KiB | 1 MiB | 1023.999999 MiB | 1 GiB | 1.367187 MiB | 400.250000 GiB" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 138: Bytesize format modifier, with fix-precision modifier.
    string = string_format ( "%.2size | %.2size | %.2size | %.2size | %.2size | %.2size | %.2size | %.2size" , KiB ( 1 ) - 1 , KiB ( 1 ) , MiB ( 1 ) - 1 , MiB ( 1 ) , GiB ( 1 ) - 1 , GiB ( 1 ) , KiB ( 1400 ) , GiB ( 400.25 ) );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "1023 B | 1 KiB | 1023.99 KiB | 1 MiB | 1023.99 MiB | 1 GiB | 1.36 MiB | 400.25 GiB" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "1023 B | 1 KiB | 1023.99 KiB | 1 MiB | 1023.99 MiB | 1 GiB | 1.36 MiB | 400.25 GiB" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 139: Empty nested format substring, standalone (no padding).
    string = string_format ( "%{}" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 140: Nested format substring, standalone (no padding).
    string = string_format ( "%i %{%c,%c,%c,%c} %i" , -1 , 'a' , 'b' , 'c' , 'd' , 123 );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "-1 a,b,c,d 123" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "-1 a,b,c,d 123" , string_length ( string ) ) );
    string_destroy ( string );

    // TEST 141: Nested format substring cannot be combined with collection format modifiers.
    string = string_format ( "%A{    }" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%A{    }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%A{    }" , string_length ( string ) ) );
    string_destroy ( string );
    // ( x2 )
    string = string_format ( "%a{    }" );
    EXPECT_NEQ ( 0 , string ); // Verify there was no memory error prior to the test.
    EXPECT_EQ ( _string_length ( "%a{    }" ) , string_length ( string ) );
    EXPECT ( memory_equal ( string , "%a{    }" , string_length ( string ) ) );
    string_destroy ( string );

    // TODO: Add support for passing a single backslash as a multi-character
    //       padding string. Currently, this does not work because the
    //       terminating delimiter matches against its escape sequence
    //       (i.e. `\'`) instead of the token itself.
    //       
    // (ex.) string_format ( "%Pl'\\'4{}" ) -> (should yield) "\\\\\\\\"

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    string_destroy ( string_in );
    string_destroy ( really_long_string_in );
    array_destroy ( file_array_in );
    array_destroy ( f32_array_in );
    array_destroy ( i8_array_in );
    array_destroy ( string_array_in );

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

void
test_register_string
( void )
{
    // test_register ( test_string_allocate_and_free , "Allocating memory for a fixed-length string." );
    // test_register ( test_string_create_and_destroy , "Allocating memory for a resizable string data structure." );
    // test_register ( test_string_append , "Testing string 'push' operation." );
    // test_register ( test_string_insert_and_remove , "Testing string 'insert' and 'remove' operations." );
    // test_register ( test_string_insert_and_remove_random , "Testing string 'insert' and 'remove' operations with random indices and elements." );
    test_register ( test_string_empty , "Detecting an empty string." );
    test_register ( test_string_truncate , "Testing string 'truncate' operation." );
    test_register ( test_string_trim , "Testing string 'trim' operation." );
    test_register ( test_string_contains , "Testing string 'contains' operation." );
    test_register ( test_string_reverse , "Testing string in-place 'reverse' operation." );
    test_register ( test_string_replace , "Testing string 'replace' operation." );
    test_register ( test_string_strip_ansi , "Stripping a string of ANSI formatting codes." );
    test_register ( test_string_strip_escape , "Stripping a string of escape sequences." );
    test_register ( test_string_u64_and_i64 , "Testing 'stringify' operation on 64-bit integers." );
    test_register ( test_string_f64 , "Testing 'stringify' operation on 64-bit floating point numbers." );
    test_register ( test_to_u64 , "Parsing a string as a u64 value." );
    test_register ( test_to_i64 , "Parsing a string as a i64 value." );
    test_register ( test_to_f64 , "Parsing a string as a f64 value." );
    test_register ( test_string_format , "Constructing a string using format specifiers." );
}