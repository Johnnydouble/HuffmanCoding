// CS 133 Spring 2022

// Support classes for bit I/O
// Standard C++ streams allow you to easily read or write by line, token and character
// with a single operation however they do not have a simple interface for reading
// or writing bit by bit. If you write out each bit as a character (the smallest
// option) your file will end up bigger than it started when you try to compress
// it. Therefore, we are providing you with these classes so that your result files 
// will actually end up smaller and you don't have to worry about complex bit
// manipulation.

#ifndef _bitstream_h_
#define _bitstream_h_

#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>

using namespace std;

// An input stream that allows you to read a single bit at a time
class IBitStream {
private:
    // the input stream it reads the data from
    istream* in;
    // the current amount we need to shift the last read in character to get the
    // next bit when we and it with 1 
    int index;
    // the most recent character we have read
    char data;
public:
    // constructs a bit stream over the passed in string, if the second parameter
    // is false. Or, if the second parameter is true, assumes the first is a 
    // file name, opens that file and makes the bit stream be over its contents
    IBitStream(string text, bool isFile);

    // reads the next bit in the file and returns it as an integer. If there are
    // no more bits in the file it returns -1. 
    int readBit();

    bool good();

    // closes the input stream
    void close();
};

// an output stream that allows you to output individual bits
class OBitStream {
private:
    // the stream we actually want our data written to
    ostream* out;
    // the next index in the next character that we need to write to
    int index;
    // the current character we are writing to
    unsigned char current;
    // true if we are in debug mode, false otherwise. In debug mode your bits 
    // will not be output as single bits, instead they will be output as the 
    // characters '1' and '0'. This can help you check whether or not you are 
    // outputting the right data. However, you cannot decompress a file encoded 
    // in debug mode!
    bool debug;
    // true if the last write call was to write a variable number of bits instead
    // of to write a whole character
    bool lastWroteBits;

    bool canGetString;
public:
    // constructs an output stream that will write to the file with the passed in 
    // name, or if none is passed in or the filename is nullptr, to the console. 
    OBitStream(string* filename = nullptr, bool debug = false, bool makeString = false);

    // takes a string of '1' and '0' characters and writes out each '1' as a single
    // bit with value 1 and each '0' as a single bit with value 0
    void writeBits(string value);

    // writes out a full character to the output stream
    void write(unsigned char c);

    // closes the output stream, outputting any last partial characters with 
    // zeros in the non-written places
    void close();

    string str();
};

#endif