/**
 * @author Matthew Weissel
 * @file memory/test_linear_allocator.c
 * @brief Implementation of the memory/test_linear_allocator header.
 * (see memory/test_linear_allocator.h for additional details)
 * 
 * TODO: If one of these fails, they all fail because of a broken pipe;
 * when the test exits early, the file is not closed. Fix this.
 */
#include "platform/test_filesystem.h"

#include "test/expect.h"

#include "container/string.h"

#include "platform/memory.h"

#define FILE_NAME_TEST_DNE             "test/assets/file-dne"
#define FILE_NAME_TEST_IN_FILE         "test/assets/in-file.txt"
#define FILE_NAME_TEST_IN_FILE_EMPTY   "test/assets/in-file-empty.txt"
#define FILE_NAME_TEST_IN_FILE_BINARY  "test/assets/in-file-binary"
#define FILE_NAME_TEST_OUT_FILE        "test/assets/out-file"

static const char* file_content_test_in_file = "This is a file with\nthree lines and 50\ncharacters.";
static const i8 file_content_test_in_file_binary[] = { 89 , 44 , 7 , -63 , 107 , -29 , 125 , -104 , -114 , -98 , -101 , -21 , -96 , -103 , 92 , 47 , 52 , 31 , 107 , -60 , -18 , -64 , 41 , 120 , -76 , -20 , -2 , -57 , 40 , 29 , 4 , -66 , 117 , -96 , 121 , 32 , -80 , -90 , 54 , 14 , 0 , -77 , -4 , -104 , -76 , -83 , -58 , 36 , -69 , 55 };

// /** @brief Computes current global number of unfreed allocations. */
/* #define MEMORY_ALLOCATION_COUNT \ */
//     ( memory_allocation_count () - memory_free_count () )

u8
test_file_exists
( void )
{
    // u64 global_amount_allocated;
    // u64 file_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // file_amount_allocated = memory_amount_allocated ( MEMORY_TAG_FILE );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: FILE_MODE_ACCESS.

    // TEST 1.1: file_exists fails on a filepath at which a file does not exist.
    EXPECT_NOT ( file_exists ( FILE_NAME_TEST_DNE , FILE_MODE_ACCESS ) );

    // TEST 1.2: file_exists succeeds, verifying the existence of all input files that will be used for subsequent tests.
    EXPECT ( file_exists ( FILE_NAME_TEST_IN_FILE , FILE_MODE_ACCESS ) );
    EXPECT ( file_exists ( FILE_NAME_TEST_IN_FILE_EMPTY , FILE_MODE_ACCESS ) );
    EXPECT ( file_exists ( FILE_NAME_TEST_IN_FILE_BINARY , FILE_MODE_ACCESS ) );

    // TEST 1.3: file_exists succeeds, verifying user has read access on all input files that will be used for subsequent tests.
    EXPECT ( file_exists ( FILE_NAME_TEST_IN_FILE , FILE_MODE_READ ) );
    EXPECT ( file_exists ( FILE_NAME_TEST_IN_FILE_EMPTY , FILE_MODE_READ ) );
    EXPECT ( file_exists ( FILE_NAME_TEST_IN_FILE_BINARY , FILE_MODE_READ ) );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( file_amount_allocated , memory_amount_allocated ( MEMORY_TAG_FILE ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_file_open_and_close
( void )
{
    // u64 global_amount_allocated;
    // u64 file_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // file_amount_allocated = memory_amount_allocated ( MEMORY_TAG_FILE );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    file_t file;

    ////////////////////////////////////////////////////////////////////////////

    // TEST 1: file_open handles invalid arguments.

    LOGWARN ( "The following errors are intentionally triggered by a test:" );

    // TEST 1.1: file_open logs an error and fails if no path is provided.
    EXPECT_NOT ( file_open ( 0 , FILE_MODE_READ , &file ) );

    // TEST 1.2: file_open logs an error and fails if no output buffer is provided for the file.
    EXPECT_NOT ( file_open ( FILE_NAME_TEST_IN_FILE , FILE_MODE_READ , 0 ) );

    // TEST 1.3: file_open logs an error and fails if the provided file mode is invalid.
    EXPECT_NOT ( file_open ( FILE_NAME_TEST_IN_FILE , 0 , &file ) );

    // TEST 2: Text file, read mode.

    // TEST 2.1: file_open succeeds in read mode on the first input file.
    EXPECT ( file_open ( FILE_NAME_TEST_IN_FILE , FILE_MODE_READ , &file ) );

    // TEST 2.2: File opened via file_open contains a valid file handle.
    EXPECT_NEQ ( 0 , file.handle );

    // TEST 2.3: File opened via file_open is valid.
    EXPECT ( file.valid );

    // TEST 2.4: file_open positions the file pointer at the start of the file.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    // Verify input file is not empty prior to additional testing.
    EXPECT_NEQ ( 0 , file_size ( &file ) );

    file_close ( &file );

    // TEST 2.5: file_close sets file handle to 0.
    EXPECT_EQ ( 0 , file.handle );

    // TEST 2.6: file_close invalidates the file.
    EXPECT_NOT ( file.valid );

    // TEST 3: Binary file, read mode.

    // TEST 3.1: file_open succeeds in read mode on the second input file.
    EXPECT ( file_open ( FILE_NAME_TEST_IN_FILE_BINARY , FILE_MODE_READ , &file ) );

    // TEST 3.2: File opened via file_open contains a valid file handle.
    EXPECT_NEQ ( 0 , file.handle );

    // TEST 3.3: File opened via file_open is valid.
    EXPECT ( file.valid );

    // TEST 3.4: file_open positions the file pointer at the start of the file.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    // Verify input file is not empty prior to additional testing.
    EXPECT_NEQ ( 0 , file_size ( &file ) );

    file_close ( &file );

    // TEST 3.5: file_close sets file handle to 0.
    EXPECT_EQ ( 0 , file.handle );

    // TEST 3.6: file_close invalidates the file.
    EXPECT_NOT ( file.valid );

    // TEST 4: Empty file, read mode.
    
    // TEST 4.1: file_open succeeds in read mode on the third input file.
    EXPECT ( file_open ( FILE_NAME_TEST_IN_FILE_EMPTY , FILE_MODE_READ , &file ) );

    // TEST 4.2: File opened via file_open contains a valid file handle.
    EXPECT_NEQ ( 0 , file.handle );

    // TEST 4.3: File opened via file_open is valid.
    EXPECT ( file.valid );

    // TEST 4.4: file_open positions the file pointer at the start of the file.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    // Verify (empty) input file **is** empty prior to additional testing.
    EXPECT_EQ ( 0 , file_size ( &file ) );

    file_close ( &file );

    // TEST 4.5: file_close sets file handle to 0.
    EXPECT_EQ ( 0 , file.handle );

    // TEST 4.6: file_close invalidates the file.
    EXPECT_NOT ( file.valid );

    // TEST 5: Empty (test) file, write mode.

    // TEST 5.1: file_open succeeds in write mode on the test file.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );

    // TEST 5.2: File opened via file_open contains a valid file handle.
    EXPECT_NEQ ( 0 , file.handle );

    // TEST 5.3: File opened via file_open is valid.
    EXPECT ( file.valid );

    // TEST 5.4: file_open positions the file pointer at the start of the file.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    // TEST 5.5: Opening in write mode truncates a file.
    // TODO: Implement a way of ensuring the file **does** contain content prior to opening, to ensure it was really truncated and is not just empty.
    EXPECT_EQ ( 0 , file_size ( &file ) );

    file_close ( &file );

    // TEST 5.6: file_close sets file handle to 0.
    EXPECT_EQ ( 0 , file.handle );

    // TEST 5.7: file_close invalidates the file.
    EXPECT_NOT ( file.valid );

    // TEST 6: Empty (test) file, read mode.

    // TEST 6.1: file_open succeeds in read mode on the test file.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_READ , &file ) );

    // TEST 6.2: File opened via file_open contains a valid file handle.
    EXPECT_NEQ ( 0 , file.handle );

    // TEST 6.3: File opened via file_open is valid.
    EXPECT ( file.valid );

    // TEST 6.4: file_open positions the file pointer at the start of the file.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    file_close ( &file );

    // TEST 6.5: file_close sets file handle to 0.
    EXPECT_EQ ( 0 , file.handle );

    // TEST 6.6: file_close invalidates the file.
    EXPECT_NOT ( file.valid );

    // TEST 7: Empty (test) file, read+write mode.

    // TEST 7.1: file_open succeeds in read+write mode on the test file.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_READ | FILE_MODE_WRITE , &file ) );

    // TEST 7.2: File opened via file_open contains a valid file handle.
    EXPECT_NEQ ( 0 , file.handle );

    // TEST 7.3: File is valid following successful file_open.
    EXPECT ( file.valid );

    // TEST 7.4: file_open positions the file pointer at the start of the file.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    file_close ( &file );

    // TEST 7.5: file_close sets file handle to 0.
    EXPECT_EQ ( 0 , file.handle );

    // TEST 7.6: file_close invalidates the file.
    EXPECT_NOT ( file.valid );

    // TEST 8: Non-empty file, read+write mode.

    // TEST 8.1: file_open succeeds in read+write mode on the second input file.
    // TODO: Implement a way of ensuring OS permission is allowed to write to this file prior to running this test.
    EXPECT ( file_open ( FILE_NAME_TEST_IN_FILE_BINARY , FILE_MODE_READ | FILE_MODE_WRITE , &file ) );
    
    // TEST 8.2: File opened via file_open contains a valid file handle.
    EXPECT_NEQ ( 0 , file.handle );

    // TEST 8.3: File opened via file_open is valid.
    EXPECT ( file.valid );

    // TEST 8.4: file_open positions the file pointer at the start of the file.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    // TEST 8.5: file_open does not truncate a non-empty file opened in read+write mode.
    EXPECT_NEQ ( 0 , file_size ( &file ) );

    file_close ( &file );

    // TEST 8.6: file_close sets file handle to 0.
    EXPECT_EQ ( 0 , file.handle );

    // TEST 8.7: file_close invalidates the file.
    EXPECT_NOT ( file.valid );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( file_amount_allocated , memory_amount_allocated ( MEMORY_TAG_FILE ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_file_read
( void )
{
    // u64 global_amount_allocated;
    // u64 file_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // file_amount_allocated = memory_amount_allocated ( MEMORY_TAG_FILE );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    char buffer[ 100 ];
    file_t file;
    u64 read;

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: file_read handles invalid arguments.

    LOGWARN ( "The following errors are intentionally triggered by a test:" );

    // TEST 1.1: file_read fails if no file is provided.
    EXPECT_NOT ( file_read ( 0 , 100 , buffer , &read ) );

    // TEST 1.2: file_read fails if invalid file is provided.
    file_t invalid_file;
    invalid_file.valid = false;
    invalid_file.handle = 0;
    EXPECT_NOT ( file_read ( &invalid_file , 100 , buffer , &read ) );

    EXPECT ( file_open ( FILE_NAME_TEST_IN_FILE_EMPTY , FILE_MODE_READ , &file ) );

    // Verify the file position is at the beginning of the file prior to the test.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    // TEST 1.3: file_read fails if no output buffer for file content is provided.
    EXPECT_NOT ( file_read ( &file , 100 , 0 , &read ) );

    // TEST 1.4: file_read does not modify the file position on failure.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    // TEST 1.5: file_read fails if no output buffer for number of bytes read is provided.
    EXPECT_NOT ( file_read ( &file , 100 , buffer , 0 ) );

    // TEST 1.6: file_read does not modify the file position on failure.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    file_close ( &file );

    // TEST 1.7: file_read fails if file is not open for read.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );
    EXPECT_NOT ( file_read ( &file , 100 , buffer , &read ) );

    // TEST 1.8: file_read does not modify the file position on failure.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );
    
    file_close ( &file );

    // TEST 2: Empty file.

    EXPECT ( file_open ( FILE_NAME_TEST_IN_FILE_EMPTY , FILE_MODE_READ , &file ) );

    // TEST 2.1: file_read succeeds.
    read = 1;
    memory_clear ( buffer , 100 );
    EXPECT ( file_read ( &file , 100 , buffer , &read ) );

    // TEST 2.2: file_read reads 0 bytes.
    EXPECT_EQ ( 0 , read );

    // TEST 2.3: file_read does not modify the file position.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    file_close ( &file );

    // TEST 3: Non-empty file, input buffer size is 0.

    EXPECT ( file_open ( FILE_NAME_TEST_IN_FILE , FILE_MODE_READ , &file ) );

    // TEST 3.1: file_read succeeds if buffer size is 0.
    read = 1;
    memory_clear ( buffer , 100 );
    EXPECT ( file_read ( &file , 0 , buffer , &read ) );

    // TEST 3.2: file_read reads 0 bytes if buffer size is 0.
    EXPECT_EQ ( 0 , read );

    // TEST 3.3: file_read does not modify the file position if buffer size is 0.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    // TEST 4: Text file. Read entire file beginning-to-end.

    read = 1;
    memory_clear ( buffer , 100 );

    // Verify file position is at the start of the file prior to the test.
    EXPECT ( file_position_set ( &file , 0 ) );

    // TEST 4.1: file_read succeeds.
    EXPECT ( file_read ( &file , 100 , buffer , &read ) );

    // TEST 4.2: The value written to the 'read' output buffer is the correct number of bytes read.
    EXPECT_EQ ( _string_length ( file_content_test_in_file ) , read );

    // TEST 4.3: Given an adequately-sized buffer, file_read reads the entire file contents at once (assuming file position is at the beginning of the file).
    EXPECT ( memory_equal ( buffer , file_content_test_in_file , _string_length ( file_content_test_in_file ) + 1 ) );
    
    // TEST 4.4: When buffer size exceeds amount remaining in the file to read, the file position is set to the end of the file following file_read.
    EXPECT_EQ ( file_size ( &file ) , file_position_get ( &file ) );

    file_close ( &file );

    // TEST 5: Binary file. Read entire file beginning-to-end.

    EXPECT ( file_open ( FILE_NAME_TEST_IN_FILE_BINARY , FILE_MODE_READ , &file ) );
    
    read = 0;
    memory_clear ( buffer , 100 );

    // Verify file position is at the start of the file prior to the test.
    EXPECT ( file_position_set ( &file , 0 ) );

    // TEST 5.1: file_read succeeds.
    EXPECT ( file_read ( &file , 100 , buffer , &read ) );

    // TEST 5.2: The value written to the 'read' output buffer is the correct number of bytes read.
    EXPECT_EQ ( sizeof ( file_content_test_in_file_binary ) , read );

    // TEST 5.3: Given an adequately-sized buffer, file_read reads the entire file contents at once (assuming file position is at the beginning of the file).
    EXPECT ( memory_equal ( buffer , file_content_test_in_file_binary , read ) );

    // TEST 5.4: When buffer size exceeds amount remaining in the file to read, the file position is set to the end of the file following file_read.
    EXPECT_EQ ( file_size ( &file ) , file_position_get ( &file ) );

    file_close ( &file );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( file_amount_allocated , memory_amount_allocated ( MEMORY_TAG_FILE ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_file_write
( void )
{
    // u64 global_amount_allocated;
    // u64 file_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // file_amount_allocated = memory_amount_allocated ( MEMORY_TAG_FILE );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    char buffer[ 100 ];
    file_t file;
    u64 written;
    u64 read;
    u64 old_file_position;

    ////////////////////////////////////////////////////////////////////////////

    // TEST 1: file_write handles invalid arguments.

    LOGWARN ( "The following errors are intentionally triggered by a test:" );

    // TEST 1.1: file_write fails when no file is provided.
    EXPECT_NOT ( _file_write ( 0 , file_content_test_in_file , &written ) );

    // TEST 1.2: file_write fails when the provided file is invalid.
    file_t invalid_file;
    invalid_file.valid = false;
    invalid_file.handle = 0;
    EXPECT_NOT ( _file_write ( &invalid_file , file_content_test_in_file , &written ) );

    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );

    // Verify the file position is at the beginning of the file prior to the test.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    // TEST 1.3: file_write fails when no handle to the data to write is provided.
    EXPECT_NOT ( file_write ( &file , 100 , 0 , &written ) );

    // TEST 1.4: file_write does not modify the file position on failure.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    // TEST 1.5: file_write fails when no output buffer for number of bytes written is provided.
    EXPECT_NOT ( _file_write ( &file , file_content_test_in_file , 0 ) );

    // TEST 1.6: file_write does not modify the file position on failure.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    file_close ( &file );

    // TEST 1.7: file_write fails if file is not open for write.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_READ , &file ) );
    EXPECT_NOT ( file_write ( &file , 100 , buffer , &written ) );

    // TEST 1.8: file_write does not modify the file position on failure.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    file_close ( &file );

    // TEST 2: Write null-terminated string to file.

    // Opening in write mode truncates the file.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );

    // Preserve file position prior to the test.
    old_file_position = file_position_get ( &file );

    // TEST 2.1: file_write succeeds.
    written = 0;
    EXPECT ( _file_write ( &file , file_content_test_in_file , &written ) );

    // TEST 2.2: The value written to the 'written' output buffer is the correct number of bytes written.
    EXPECT_EQ ( _string_length ( file_content_test_in_file ) , written );

    // TEST 2.3: Attempt to verify that the correct number of characters were written to the file by checking the file position and size.
    EXPECT_EQ ( old_file_position + _string_length ( file_content_test_in_file ) , file_position_get ( &file ) );
    EXPECT_EQ ( file_position_get ( &file ) , file_size ( &file ) );

    // TEST 2.4: file_write succeeds if input buffer size is 0.
    written = 0;
    EXPECT ( file_write ( &file , 0 , file_content_test_in_file , &written ) );
    
    // TEST 2.5: If input buffer size is 0, the value written to the 'written' output buffer is the correct number of bytes written (0).
    EXPECT_EQ ( 0 , written );

    // TEST 2.6: file_write does not modify the size of the file if input buffer size is 0.
    EXPECT_EQ ( _string_length ( file_content_test_in_file ) , file_size ( &file ) );

    // TEST 2.7: file_write does not modify the file position if input buffer size is 0.
    EXPECT_EQ ( file_size ( &file ) , file_position_get ( &file ) );

    file_close ( &file );

    // Open file in read mode for post-write validation.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_READ , &file ) );
    
    // Verify that the read succeeds prior to the test.
    memory_clear ( buffer , 100 );
    EXPECT ( file_read ( &file , 100 , buffer , &read ) );

    // TEST 2.8: The size of the input buffer is equal to the number of bytes written to the file.
    EXPECT_EQ ( _string_length ( file_content_test_in_file ) , read );

    // TEST 2.9: The bytes of the input buffer are identical to the bytes written to the file.
    EXPECT ( memory_equal ( buffer , file_content_test_in_file , _string_length ( file_content_test_in_file ) + 1 ) );
    
    file_close ( &file );

    // TEST 3: Write binary string to file.

    // Opening in write mode truncates the file.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );

    // Preserve file position prior to the test.
    old_file_position = file_position_get ( &file );
    
    // TEST 3.1: file_write succeeds.
    written = 0;
    EXPECT ( file_write ( &file , sizeof ( file_content_test_in_file_binary ) , file_content_test_in_file_binary , &written ) );

    // TEST 3.2: The value written to the 'written' output buffer is the correct number of bytes written.
    EXPECT_EQ ( sizeof ( file_content_test_in_file_binary ) , written );

    // TEST 3.3: Attempt to verify that the correct number of characters were written to the file by checking the file position and size.
    EXPECT_EQ ( old_file_position + sizeof ( file_content_test_in_file_binary ) , file_position_get ( &file ) );
    EXPECT_EQ ( file_position_get ( &file ) , file_size ( &file ) );

    // TEST 3.4: file_write succeeds if input buffer size is 0.
    written = 0;
    EXPECT ( file_write ( &file , 0 , file_content_test_in_file , &written ) );
    
    // TEST 3.5: If input buffer size is 0, the value written to the 'written' output buffer is the correct number of bytes written (0).
    EXPECT_EQ ( 0 , written );

    // TEST 3.6: file_write does not modify the size of the file if input buffer size is 0.
    EXPECT_EQ ( sizeof ( file_content_test_in_file_binary ) , file_size ( &file ) );

    // TEST 3.7: file_write does not modify the file position if input buffer size is 0.
    EXPECT_EQ ( file_size ( &file ) , file_position_get ( &file ) );

    file_close ( &file );

    // Open file in read mode for post-write validation.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_READ , &file ) );

    // Verify that the read succeeds prior to the test.
    memory_clear ( buffer , 100 );
    EXPECT ( file_read ( &file , 100 , buffer , &read ) );

    // TEST 3.8: The size of the input buffer is equal to the number of bytes written to the file.
    EXPECT_EQ ( sizeof ( file_content_test_in_file_binary ) , read );

    // TEST 3.9: The bytes of the input buffer are identical to the bytes written to the file.
    EXPECT ( memory_equal ( buffer , file_content_test_in_file_binary , read ) );

    file_close ( &file );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    // Truncate the test file.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );
    file_close ( &file );

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( file_amount_allocated , memory_amount_allocated ( MEMORY_TAG_FILE ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_file_read_line
( void )
{
    // u64 global_amount_allocated;
    // u64 array_amount_allocated;
    // u64 file_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // file_amount_allocated = memory_amount_allocated ( MEMORY_TAG_FILE );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    const u64 max_line_length = MB ( 1 );
    const u64 line_count = 100;
    file_t file;
    u64 written;
    u64 old_file_position;

    char* in_lines[ 100 ];
    char* out_lines[ 101 ];
    for ( u64 i = 0; i < line_count; ++i )
    {
        in_lines[ i ] = string_create ();
        EXPECT_NEQ ( 0 , in_lines[ i ] );
    }
    memory_clear ( out_lines , sizeof ( char* ) * 100 );

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // TEST 1: file_read_line handles invalid arguments.

    LOGWARN ( "The following errors are intentionally triggered by a test:" );

    // TEST 1.1: file_read_line fails when no file is provided.
    EXPECT_NOT ( file_read_line ( 0 , &out_lines[ 0 ] ) );

    // TEST 1.2: file_read_line fails when the provided file is invalid.
    file_t invalid_file;
    invalid_file.valid = false;
    invalid_file.handle = 0;
    EXPECT_NOT ( file_read_line ( &invalid_file , &out_lines[ 0 ] ) );

    EXPECT ( file_open ( FILE_NAME_TEST_IN_FILE , FILE_MODE_READ , &file ) );

    // TEST 1.3: file_read_line fails when no output buffer for line content is provided.
    EXPECT_NOT ( file_read_line ( &file , 0 ) );

    // TEST 1.4: file_read_line does not modify the file position on failure.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    file_close ( &file );

    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );

    // TEST 1.5: file_read_line fails if file is not open for read.
    EXPECT_NOT ( file_read_line ( &file , &out_lines[ 0 ] ) );

    // TEST 1.6: file_read_line does not modify the file position on failure.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    // TEST 2: file_read_line.

    // Populate the file with random-length lines of (non-zero) random characters.
    for ( u64 i = 0; i < line_count; ++i )
    {
        // Generate a random-length string of random characters.
        const u64 length = random2 ( STACK_STRING_MAX_SIZE + 1
                                   , max_line_length
                                   );
        for ( u64 j = 0; j < length; ++j )
        {
            _string_append ( in_lines[ i ] , string_char ( random2 ( 33 , 126 ) ) );
        }

        // Append a newline to the string.
        _string_append ( in_lines[ i ] , string_char ( '\n' ) );

        // Preserve file position prior to write.
        old_file_position = file_position_get ( &file );

        // Write the string to the file.
        EXPECT ( file_write ( &file , string_length ( in_lines[ i ] ) , in_lines[ i ] , &written ) );
        
        // Attempt to verify that the correct number of characters were written to the file by checking the output buffer, file position, and size.
        EXPECT_EQ ( string_length ( in_lines[ i ] ) , written );
        EXPECT_EQ ( old_file_position + string_length ( in_lines[ i ] ) , file_position_get ( &file ) );
        EXPECT_EQ ( file_position_get ( &file ) , file_size ( &file ) );
    }

    file_close ( &file );

    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_READ , &file ) );

    for ( u64 i = 0; i < line_count; ++i )
    {
        old_file_position = file_position_get ( &file );

        // TEST 2.1: file_read_line succeeds.
        EXPECT ( file_read_line ( &file , &out_lines[ i ] ) );

        // Verify there was no memory error prior to the test.
        EXPECT_NEQ ( 0 , out_lines[ i ] );

        // TEST 2.2: file_read_line advances the file position by the length of the line that was read.
        EXPECT_EQ ( old_file_position + string_length ( in_lines[ i ] ) , file_position_get ( &file ) );

        // TEST 2.3: file_read_line outputs a string containing the correct number of characters from the file (excludes the newline).
        EXPECT_EQ ( string_length ( in_lines[ i ] ) - 1 , string_length ( out_lines[ i ] ) );

        // TEST 2.4: file_read_line outputs a string containing the correct line from the file.
        EXPECT ( memory_equal ( in_lines[ i ] , out_lines[ i ] , string_length ( out_lines[ i ] ) ) );
    }
    
    // Verify the file position is at the end of the file.
    EXPECT_EQ ( file_size ( &file ) , file_position_get ( &file ) );

    // TEST 2.5: file_read_line succeeds when file position is at the end of the file.
    EXPECT ( file_read_line ( &file , &out_lines[ line_count ] ) );

    // TEST 2.6: file_read_line outputs an empty string when file position is at the end of the file.
    EXPECT_EQ ( 0 , string_length ( out_lines[ line_count ] ) );
    EXPECT_EQ ( 0 , *( out_lines[ line_count ] ) );

    file_close ( &file );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    for ( u64 i = 0; i < line_count; ++i )
    {
        string_destroy ( in_lines[ i ] );
        string_destroy ( out_lines[ i ] );
    }
    string_destroy ( out_lines[ line_count ] );

    // Truncate the test file.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );
    file_close ( &file );

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( file_amount_allocated , memory_amount_allocated ( MEMORY_TAG_FILE ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );
    
    return true;
}

u8
test_file_write_line
( void )
{
    // u64 global_amount_allocated;
    // u64 array_amount_allocated;
    // u64 file_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // array_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ARRAY );
    // file_amount_allocated = memory_amount_allocated ( MEMORY_TAG_FILE );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    const char* in_line = "This is the line to be written to the file.";
    char* out_line;
    char buffer[ 100 ];
    file_t file;
    u64 old_file_position;

    ////////////////////////////////////////////////////////////////////////////

    // TEST 1: file_write_line handles invalid arguments.

    LOGWARN ( "The following errors are intentionally triggered by a test:" );

    // TEST 1.1: file_write_line fails if no file is provided.
    EXPECT_NOT ( file_write_line ( 0 , 100 , buffer ) );

    // TEST 1.2: file_write_line fails if the provided file is invalid.
    file_t invalid_file;
    invalid_file.valid = false;
    invalid_file.handle = 0;
    EXPECT_NOT ( file_write_line ( &invalid_file , 100 , buffer ) );

    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );

    // Verify the file position is at the beginning of the file prior to the test.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );
    
    // TEST 1.3: file_write_line fails if no handle is provided to content to write.
    EXPECT_NOT ( file_write_line ( &file , 100 , 0 ) );

    // TEST 1.4: file_write_line does not modify the file position on failure.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    file_close ( &file );

    // TEST 1.5: file_write_line fails if file is not open for write.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_READ , &file ) );
    EXPECT_NOT ( file_write_line ( &file , 100 , buffer ) );

    // TEST 1.6: file_write_line does not modify the file position on failure.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );
    
    file_close ( &file );

    // TEST 2: file_write_line.

    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );

    // Verify the file position is at the beginning of the file prior to the test.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );
    
    memory_copy ( buffer , in_line , _string_length ( in_line ) + 1 );

    // Preserve file position prior to the test.
    old_file_position = file_position_get ( &file );

    // TEST 2.1: file_write_line succeeds.
    EXPECT ( _file_write_line ( &file , buffer ) );

    // TEST 2.2: Attempt to verify that the correct number of characters were written to the file by checking the file position and size.
    EXPECT_EQ ( old_file_position + _string_length ( buffer ) + 1 , file_position_get ( &file ) );
    EXPECT_EQ ( file_position_get ( &file ) , file_size ( &file ) );

    // Preserve file position prior to the test.
    old_file_position = file_position_get ( &file );

    // TEST 2.3: file_write_line succeeds.
    EXPECT ( _file_write_line ( &file , buffer ) );

    // TEST 2.4: Attempt to verify that the correct number of characters were written to the file by checking the file position and size.
    EXPECT_EQ ( old_file_position + _string_length ( buffer ) + 1 , file_position_get ( &file ) );
    EXPECT_EQ ( file_position_get ( &file ) , file_size ( &file ) );

    // Preserve file position prior to the test.
    old_file_position = file_position_get ( &file );

    // TEST 2.5: file_write_line succeeds if size of input buffer is 0.
    EXPECT ( file_write_line ( &file , 0 , buffer ) );

    // TEST 2.6: Attempt to verify that the correct number of characters were written to the file by checking the file position and size.
    EXPECT_EQ ( old_file_position + 1 , file_position_get ( &file ) );
    EXPECT_EQ ( file_position_get ( &file ) , file_size ( &file ) );

    file_close ( &file );

    // Open file in read mode for post-write validation.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_READ , &file ) );

    // Verify file_read_line succeeds prior to the test.
    EXPECT ( file_read_line ( &file , &out_line ) );

    // TEST 2.7: The length of the input line is equal to the number of characters written to the file.
    EXPECT_EQ ( _string_length ( in_line ) , string_length ( out_line ) );

    // TEST 2.8: The bytes of the input line are identical to the bytes written to the file.
    EXPECT ( memory_equal ( out_line , in_line , string_length ( out_line ) ) );

    string_destroy ( out_line );

    EXPECT ( file_read_line ( &file , &out_line ) );

    // TEST 2.9: The length of the input line is equal to the number of characters written to the file.
    EXPECT_EQ ( _string_length ( in_line ) , string_length ( out_line ) );
    
    // TEST 2.10: The bytes of the input line are identical to the bytes written to the file.
    EXPECT ( memory_equal ( out_line , in_line , string_length ( out_line ) ) );

    string_destroy ( out_line );

    // TEST 2.11: file_write_line wrote one additional newline to the file (from the invocation which used an empty input buffer).
    EXPECT_EQ ( file_size ( &file ) - 1 , file_position_get ( &file ) );
    EXPECT ( file_read_line ( &file , &out_line ) );
    EXPECT_EQ ( 0 , string_length ( out_line ) );
    EXPECT_EQ ( file_size ( &file ) , file_position_get ( &file ) );

    string_destroy ( out_line );

    file_close ( &file );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    // Truncate the test file.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );
    file_close ( &file );

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( array_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ARRAY ) );
    // EXPECT_EQ ( file_amount_allocated , memory_amount_allocated ( MEMORY_TAG_FILE ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_file_read_all
( void )
{
    // u64 global_amount_allocated;
    // u64 file_amount_allocated;
    // u64 string_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // file_amount_allocated = memory_amount_allocated ( MEMORY_TAG_FILE );
    // string_amount_allocated = memory_amount_allocated ( MEMORY_TAG_STRING );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    const u64 filesize = MiB ( 100 ); /* MIN ( GiB ( 1 ) , memory_amount_free () / 2 - KiB ( 1 ) ) */
    file_t file;
    u64 read;
    u64 written;

    char* string_in = string_allocate ( filesize + 1 );
    u8* string_out;

    ////////////////////////////////////////////////////////////////////////////

    // TEST 1: file_read_all handles invalid arguments.

    LOGWARN ( "The following errors are intentionally triggered by a test:" );

    // TEST 1.1: file_read_all logs an error and fails if no file is provided.
    EXPECT_NOT ( file_read_all ( 0 , &string_out , &read ) );

    // TEST 1.2: file_read_all logs an error and fails if the provided file is invalid.
    file_t invalid_file;
    invalid_file.valid = false;
    invalid_file.handle = 0;
    EXPECT_NOT ( file_read_all ( &invalid_file , &string_out , &read ) );
    
    EXPECT ( file_open ( FILE_NAME_TEST_IN_FILE , FILE_MODE_READ , &file ) );

    // Verify the file position is at the beginning of the file prior to the test.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );
    
    // TEST 1.3: file_read_all fails if no output buffer for file content is provided.
    EXPECT_NOT ( file_read_all ( &file , 0 , &read ) );

    // TEST 1.4: file_read_all does not modify the file position on failure.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    // TEST 1.5: file_read_all fails if no output buffer for number of bytes read is provided.
    EXPECT_NOT ( file_read_all ( &file , &string_out , 0 ) );

    // TEST 1.6: file_read_all does not modify the file position on failure.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    file_close ( &file );
    
    // TEST 1.7: file_read_all fails if file is not open for read.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );
    EXPECT_NOT ( file_read_all ( &file , &string_out , &read ) );

    // TEST 1.8: file_read_all does not modify the file position on failure.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    file_close ( &file );
    
    // TEST 2: Empty file.

    EXPECT ( file_open ( FILE_NAME_TEST_IN_FILE_EMPTY , FILE_MODE_READ , &file ) );

    // TEST 2.1: file_read_all succeeds.
    EXPECT ( file_read_all ( &file , &string_out , &read ) );

    // TEST 2.2: file_read_all does not modify the file position.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    // TEST 2.3: If input buffer size is 0, the value written to the 'read' output buffer is the correct number of bytes read (0).
    EXPECT_EQ ( 0 , read );

    // TEST 2.4: file_read_all writes an empty string to output buffer which stores file content.
    EXPECT_EQ ( 0 , _string_length ( ( char* ) string_out ) );
    EXPECT_EQ ( 0 , *( string_out ) );

    file_close ( &file );

    string_free ( string_out );

    // TEST 3: File position is at beginning of file.

    // Open file for write.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );

    // Generate 1.00 GiB of random binary data to be used for populating the test file with content.
    for ( u64 i = 0; i < filesize / sizeof ( u64 ); ++i )
    {
        ( ( u64* ) string_in )[ i ] = random64 ();
    }

    // Populate the file with 1.00 GiB of random binary data.
    EXPECT ( file_write ( &file , filesize , string_in , &written ) );
    EXPECT_EQ ( filesize , written );
    EXPECT_EQ ( filesize , file_size ( &file ) );
    file_close ( &file );

    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_READ , &file ) );

    // Verify the file position is at the beginning of the file prior to the test.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    // TEST 3.1: file_read_all succeeds.
    EXPECT ( file_read_all ( &file , &string_out , &read ) );

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string_out );

    // TEST 3.2: The value written to the 'read' output buffer is the correct number of bytes read.
    EXPECT_EQ ( filesize , read );

    // TEST 3.3: The bytes of the output string are identical to the bytes originally written to the file.
    EXPECT ( memory_equal ( string_in , string_out , read ) );

    // TEST 3.4: file_read_all resets the file pointer to the beginning of the file.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    string_free ( string_out );

    // TEST 4: File position is **not** at beginning of file.

    // Explicitly move file pointer from the beginning of file.
    EXPECT ( file_position_set ( &file , file_size ( &file ) ) );
    EXPECT_NEQ ( 0 , file_position_get ( &file ) );
    
    // TEST 4.1: file_read_all succeeds.
    EXPECT ( file_read_all ( &file , &string_out , &read ) );

    // Verify there was no memory error prior to the test.
    EXPECT_NEQ ( 0 , string_out );

    // TEST 4.2: The value written to the 'read' output buffer is the correct number of bytes read.
    EXPECT_EQ ( filesize , read );

    // TEST 4.3: The bytes of the output string are identical to the bytes originally written to the file.
    EXPECT ( memory_equal ( string_in , string_out , read ) );

    // TEST 4.4: file_read_all resets the file pointer to the beginning of the file.
    EXPECT_EQ ( 0 , file_position_get ( &file ) );

    file_close ( &file );

    string_free ( string_out );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    string_free ( string_in );

    // Truncate the test file.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );
    file_close ( &file );

    // // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( file_amount_allocated , memory_amount_allocated ( MEMORY_TAG_FILE ) );
    // EXPECT_EQ ( string_amount_allocated , memory_amount_allocated ( MEMORY_TAG_STRING ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

u8
test_file_read_and_write_large_file
( void )
{
    // u64 global_amount_allocated;
    // u64 file_amount_allocated;
    // u64 string_amount_allocated;
    // u64 global_allocation_count;

    // // Copy the current global allocator state prior to the test.
    // global_amount_allocated = memory_amount_allocated ( MEMORY_TAG_ALL );
    // file_amount_allocated = memory_amount_allocated ( MEMORY_TAG_FILE );
    // string_amount_allocated = memory_amount_allocated ( MEMORY_TAG_STRING );
    // global_allocation_count = MEMORY_ALLOCATION_COUNT;

    const u64 buffer_size = MiB ( 100 ) /* MIN ( GiB ( 1 ) , memory_amount_free () / 2 - KiB ( 1 ) ) */;
    char* in_buffer = string_allocate ( buffer_size );
    char* out_buffer = string_allocate ( buffer_size );
    file_t file;
    u64 read;
    u64 written;
    u64 old_file_position;

    // Generate 1.00 GiB of random binary data to be used for populating the test file with content.
    for ( u64 i = 0; i < buffer_size / sizeof ( u64 ); ++i )
    {
        ( ( u64* ) in_buffer )[ i ] = random64 ();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Start test.

    // Write a 6.00 GiB temporary file to disk.
    u64 written_ = 0;
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );
    while ( written_ < GiB ( 6 ) )
    {
        // Preserve file position prior to the test.
        old_file_position = file_position_get ( &file );

        // TEST 1: file_write succeeds with a buffer size of 1.00 GiB.
        written = 0;
        EXPECT ( file_write ( &file , buffer_size , in_buffer , &written ) );

        // TEST 2: The value written to the 'written' output buffer is correct.
        EXPECT_EQ ( buffer_size , written );

        // TEST 3: Attempt to verify that the correct number of characters were written to the file by checking the file position and size.
        EXPECT_EQ ( old_file_position + buffer_size , file_position_get ( &file ) );
        EXPECT_EQ ( file_position_get ( &file ) , file_size ( &file ) );

        written_ += written;
    }

    // TEST 4: File size is correct.
    EXPECT ( file_size ( &file ) >= GiB ( 6 ) );

    file_close ( &file );

    // Read 6.00 GiB back from the file and validate the file content that was written is correct.
    u64 read_ = 0;
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_READ , &file ) );
    while ( read_ < GiB ( 6 ) )
    {
        // Preserve file position prior to the test.
        old_file_position = file_position_get ( &file );

        // TEST 5: file_read succeeds with a buffer size of 1.00 GiB.
        read = 0;
        memory_clear ( out_buffer , buffer_size );
        EXPECT ( file_read ( &file , buffer_size , out_buffer , &read ) );

        // TEST 6: The value written to the 'read' output buffer is correct.
        EXPECT_EQ ( buffer_size , read );

        // TEST 7: The bytes of the input line are identical to the bytes written to the file.
        EXPECT ( memory_equal ( out_buffer , in_buffer , buffer_size ) );

        // TEST 8: The file position has been advanced by the correct amount.
        EXPECT_EQ ( old_file_position + buffer_size , file_position_get ( &file ) );

        read_ += read;
    }

    // TEST 9: The file position is at the end of the file.
    EXPECT_EQ ( file_size ( &file ) , file_position_get ( &file ) );

    file_close ( &file );

    // End test.
    ////////////////////////////////////////////////////////////////////////////

    string_free ( in_buffer );
    string_free ( out_buffer );

    // Truncate the test file.
    EXPECT ( file_open ( FILE_NAME_TEST_OUT_FILE , FILE_MODE_WRITE , &file ) );
    file_close ( &file );

    // Verify the test allocated and freed all of its memory properly.
    // EXPECT_EQ ( global_amount_allocated , memory_amount_allocated ( MEMORY_TAG_ALL ) );
    // EXPECT_EQ ( file_amount_allocated , memory_amount_allocated ( MEMORY_TAG_FILE ) );
    // EXPECT_EQ ( string_amount_allocated , memory_amount_allocated ( MEMORY_TAG_STRING ) );
    // EXPECT_EQ ( global_allocation_count , MEMORY_ALLOCATION_COUNT );

    return true;
}

void
test_register_filesystem
( void )
{
    test_register ( test_file_exists , "Querying the host platform for the existence of a file." );
    test_register ( test_file_open_and_close , "Opening or closing a file on the host platform." );
    test_register ( test_file_read , "Reading a file on the host platform into a local buffer." );
    test_register ( test_file_write , "Writing from a local buffer to a file on the host platform." );
    test_register ( test_file_read_line , "Reading a line of text from a file on the host platform." );
    test_register ( test_file_write_line , "Writing a line of text to a file on the host platform." );
    test_register ( test_file_read_all , "Reading the entire contents of a file on the host platform into program memory." );
    test_register ( test_file_read_and_write_large_file , "Testing file 'read' and 'write' operations on a file larger than 4 GiB." );
}
