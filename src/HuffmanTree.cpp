#include "HuffmanTree.h"

HuffmanTree::HuffmanTree(std::map<char, int>& counts) {
    //fill priority queue
    auto cmp = [](HuffmanNode* h1, HuffmanNode* h2){
        return h1->freq < h2->freq;
    };
    std::priority_queue<HuffmanNode*, vector<HuffmanNode*>, decltype(cmp)> pq(cmp);
    for (auto& pair : counts) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    HuffmanNode* uno;
    HuffmanNode* dos;
    while (pq.size() > 1) {
        uno = pq.top(); pq.pop();
        dos = pq.top(); pq.pop();

        pq.push(new HuffmanNode('_', uno->freq + dos->freq, uno, dos));
    }

    treeRoot = pq.top();
    cout << "boobear";
}

HuffmanTree::HuffmanTree(std::istream* in) {

}

std::map<char, std::string> HuffmanTree::createEncodings() {
    std::map<char, std::string> charBinMap;
    return charBinMap;
}

void HuffmanTree::compress(ifstream* input, OBitStream* output) {

}

void HuffmanTree::decompress(IBitStream* input, OBitStream* output) {

}
