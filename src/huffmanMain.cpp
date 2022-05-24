// CS 133 Spring 2022

// Main program for Huffman Coding

// This program uses the HuffmanTree member functions that you will write.
// You MUST have all of them declared and return a value, if they return, before
// this will compile. 

// This program will ask the user whether they want to encode or decode a file 
// and then will prompt the user for three file names. If you are encoding these
// will be the file you want to encode and the names of the files you want to 
// output the code and the compressed text to. If you are decoding these will be
// the compressed file, the file that stores the codes that were used to compress
// it and the file to output the decompressed text to.

#include <iostream>
#include <fstream>
#include <istream>
#include <map>

#include "BitStream.h"
#include "huffmanConstants.h"
#include "HuffmanTree.h"

#ifdef USING_QT
const string PREFIX = "res/";
#include "console.h"
#else
const string PREFIX = "";
#endif

using namespace std;

// takes references to three strings to store the values the user intputs in.
// Prompts the user for three file names and stores them in the passed in 
// variables. Prompts the user with the text of the last parameter.
void getFileNames(string& inFile, string& codeFile, string& outputFile, string prompt) {
    cout << prompt << " file name? ";
    getline(cin, inFile);
    cout << "code file name? ";
    getline(cin, codeFile);
    cout << "output file name (blank for console)? ";
    getline(cin, outputFile);
}

// counts the words in the passed in stream, constructing and returning a map
// from each word to a count of how many times it occurred in the stream
map<char, int> countWords(istream* in) {
    map<char, int> counts;
    char letter = in->get();
    while(letter != EOF) {
        if(counts.count(letter) < 1) {
            counts[letter] = 1;
        } else {
            counts[letter]++;
        }
        letter = in->get();
    }
    return counts;
}

// outputs the encodings stored in the passed in map to the file with the passed
// in name. Outputs the key character's ASCII value on its own line and then 
// the value string on the next line for each key/value pair.
void saveEncodings(string& codeFile, map<char, string>& codes) {
    ofstream file(codeFile);
    for(auto pair : codes) {
        int letter = pair.first;
        string code = pair.second;
        file << letter << endl;
        file << code << endl;
    }
    file.close();
}

// Takes a file name as a parameter and returns a pointer to an output bit stream
// outputting to that file, if the file name wasn't empty. If it was empty returns
// an output bit stream that will print to the console. 
OBitStream* openOutput(string& outputFile) {
    OBitStream* output;
    if (outputFile.length() > 0) {
        output = new OBitStream(&outputFile);
    } else {
        output = new OBitStream(nullptr, DEBUG); 
    }
    return output;
}

// prompts the user for a compressed file, a code file and an output file and 
// then decompresses the compressed file using the codes in the code file and 
// writes the result to the output file
void decode() {	
    string inFile, codeFile, outputFile;
    getFileNames(inFile, codeFile, outputFile, "encoded");

    // adds the res/ to the beginning of the file name if USING_QT is set to 
    // true. Otherwise doesn't alter the file names.
    inFile = PREFIX + inFile;
    outputFile = PREFIX + outputFile;
    codeFile = PREFIX + codeFile;

    // open code file and construct tree
    istream* codeInput = new ifstream(codeFile);
    HuffmanTree t (codeInput);

    // open encoded file, open output, decode
    IBitStream* input = new IBitStream(inFile, true);
    OBitStream* output = openOutput(outputFile);

    // decompress the file using the function you write	
    cout << "Right before decompress is called." << endl;
    t.decompress(input, output);
    cout << "Congratulations! Decompression succeeded without a segfault." << endl;
    output->close();
}

// prompts the user for the name of the file they want to compress, the name of
// the file they want to output codes to and the name of the file they want to 
// store the compressed version in. It then compresses the first file writing 
// the codes used in copression to the second file and the compressed data to 
// the third file. 
void encode() {
    string inFile, codeFile, outputFile;
    getFileNames(inFile, codeFile, outputFile, "input");

    // create a map of characters to counts from the file to compress
    ifstream inputStream(inFile);
    map<char, int> wordCounts = countWords(&inputStream);

    // use the class you are writing to figure out efficeint encodings for each
    // character and save these to a file so we can decode later
    HuffmanTree tree(wordCounts);
    map<char, string> codes = tree.createEncodings();
    saveEncodings(codeFile, codes);

    // open source file, open output, encode
    ifstream* input = new ifstream(inFile);
    OBitStream* output = openOutput(outputFile);
    cout << "Right before compress is called." << endl;
    tree.compress(input, output);
    cout << "Congratulations! Compression succeeded without a segfault." << endl;
}


int main() {
    cout << "This program can encode a file with a Huffman code ";
    cout << "or decode a file with a Huffman code." << endl;
    cout << "(e)ncode or (d)ecode? ";
    string operation;
    getline(cin, operation);
    cout << endl;
    
    if(operation[0] == 'e' || operation[0] == 'E') {
        encode();
    } else {
        decode();
    }
    return 0;
}