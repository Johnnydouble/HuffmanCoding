// CS 133 Spring 2022

// Implementation of IBitStream and OBitStream support classes for bit I/O

#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <ios>

#include "BitStream.h"

using namespace std;

// constructs a bit stream over the passed in string, if the second parameter
// is false. Or, if the second parameter is true, assumes the first is a 
// file name, opens that file and makes the bit stream be over its contents
IBitStream::IBitStream(string filename, bool isFile) {
    if(isFile) {
        in = new ifstream(filename, ios::binary);
    } else {
        in = new istringstream(filename, ios::binary);
    }
    index = -1;
}

bool IBitStream::good() {
    return in->good() || index != -1;
}

// reads the next bit in the file and returns it as an integer. If there are
// no more bits in the file it returns -1. 
int IBitStream::readBit() {
    if(in->eof() && index < 0) { // test!!!!!
        return -1;
    }
    if(index < 0) {
        data = in->get();
        index = 7;
    } 
    char copy = data >> index;
    index -= 1;
    //cout << (copy & 1);
    return copy & 1;
}

// closes the input stream
void IBitStream::close() {
    delete in;
    in = nullptr;
}


// constructs an output stream that will write to the file with the passed in 
// name, or if none is passed in or the filename is nullptr, to the console. 
OBitStream::OBitStream(string* filename, bool debug, bool makeString) {
    this->debug = debug;
    canGetString = false;
    if(filename != nullptr) { 
        out = new ofstream(*filename, ios::binary);
    } else if (!makeString) {
        this->debug = true; // always debug when writing to the console
        out = &cout;
    } else {
        out = new ostringstream();
        canGetString = true;
    }
    index = 7;
    lastWroteBits = false;
}

// takes a string of '1' and '0' characters and writes out each '1' as a single
// bit with value 1 and each '0' as a single bit with value 0
void OBitStream::writeBits(string value) {
    // print out the string and do nothing else when in debug mode
    if(debug) {
        *out << value;
        return;
    } 
    // convert each character to a bit one at a time
    for(int i = 0; i < value.length(); i++) {
        // ignore new lines and other no '1' or '0' characters
        if(value[i] == '1' || value[i] == '0') {
            // index will be -1 if we have filled up all bits in the current character
            // so we know we now can print that character and start filling up a new 
            // one
            if(index == -1) {
                out->put(current);
                index = 7;
                current = 0;
            }
            current = current << 1;
            index--;

            int val = value[i] == '1'? 1 : 0;
            current = current | val;
        }
    }
    // remember that we last wrote some number of bits so that we know we may be
    // partway through filling up a character
    lastWroteBits = true;
}

// writes out a full character to the output stream
void OBitStream::write(unsigned char c) {
    // flush any partially written character first
    if(lastWroteBits && index != -1) {
        current = current << index;
        out->put(c);
        lastWroteBits = false;
    }
    out->put(c);
}

// closes the output stream, outputting any last partial characters with 
// zeros in the non-written places
void OBitStream::close() {
    // if our stream is nullptr we must have closed the stream already so do 
    // nothing
    if(out == nullptr) {
        return;
    }

    // flush any partially full characters if we are partway through writing one
    if(lastWroteBits && index != -1) {
        current = current << (index + 1);
        out->put(current);
    }
    //*out << endl;

    // testing hack so that can still get str after student close
    if(!canGetString) {
        // don't delete if we are in debug mode as we don't want to delete cout
        if(!debug) {
            delete out;
        }
        out = nullptr;
    }
}

string OBitStream::str() {
    if(canGetString) {
        return ((ostringstream*)out)->str();
    } else {
        return "";
    }
}