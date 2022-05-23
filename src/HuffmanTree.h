#include "HuffmanNode.h"
#include <map>
#include <iostream>
#include "BitStream.h"
#include <queue>

class HuffmanTree {
    HuffmanNode* treeRoot; 
public:
    HuffmanTree(std::map<char, int>& counts);
    HuffmanTree(std::istream* in);
    std::map<char, std::string> createEncodings();
    void compress(ifstream* input, OBitStream* output);
    void decompress(IBitStream* input, OBitStream* output);
};