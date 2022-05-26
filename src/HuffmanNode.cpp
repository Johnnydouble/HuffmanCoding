#include "HuffmanNode.h"

HuffmanNode::HuffmanNode(char c, int freq, HuffmanNode* zero, HuffmanNode* one) :
    c(c), freq(freq), zero(zero), one(one) {}

bool HuffmanNode::isLeaf() const{
    return zero == nullptr && one == nullptr;
}

bool HuffmanNode::operator<(HuffmanNode& h2){
    return freq < h2.freq;
}

bool HuffmanNode::operator()(HuffmanNode* h1, HuffmanNode* h2){
    return h1->freq < h2->freq;
}