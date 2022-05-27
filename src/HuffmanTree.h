#include "HuffmanNode.h"
#include <map>
#include <iostream>
#include "BitStream.h"
#include <queue>

class HuffmanTree {
    HuffmanNode* treeRoot;
    std::map<char, std::string> charBinMap;

    void encodingHelper(HuffmanNode* root, string accumulator, std::map<char, std::string>& output);
    void addCharToTree(HuffmanNode*& root, std::istream* bits, char c);
    char decompHelper(HuffmanNode* root, IBitStream* input);
    void nodeMuncher(HuffmanNode*& node);
    void trimLineEndings(std::string& str);
    template<typename T>
    void condThrowNPE(T* ptr, string message);


public:
    HuffmanTree(std::map<char, int>& counts);
    HuffmanTree(std::istream* in);
    ~HuffmanTree();

    std::map<char, std::string> createEncodings();
    void compress(ifstream* input, OBitStream* output);
    void decompress(IBitStream* input, OBitStream* output);
};