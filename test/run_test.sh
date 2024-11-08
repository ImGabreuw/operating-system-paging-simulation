#!/bin/sh

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <test_file.c>"
    exit 1
fi

TEST_FILE=$1
EXEC_FILE="test"

INCLUDE_DIR="../include"
SRC_DIR="../src"
TEST_DIR="../test"

SRC_FILES=$(find $SRC_DIR -name "*.c" ! -name "main.c" ! -path "$TEST_DIR/*")

TEST_FILES=$TEST_FILE

CFLAGS="-I$INCLUDE_DIR -pthread"

gcc -o $EXEC_FILE $TEST_FILES $SRC_FILES $CFLAGS

if [ $? -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi

./$EXEC_FILE

rm -f $EXEC_FILE

exit 0
