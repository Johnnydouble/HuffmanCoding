// HuffmanTree.cpp
// Christen Spadavecchia
// CS 133
// 5/23/22
// 
// 
// 
// 

#include "HuffmanTree.h"

// void printBT(const std::string& prefix, const HuffmanNode* node, bool isLeft) //                                     remove this
// {
//     if (node != nullptr)
//     {
//         std::cout << prefix;

//         std::cout << (isLeft ? "├──" :"└──" );

//         // print the value of the node
//         if (node->c == '*') {
//             std::cout << "─┐" << std::endl;
//         }
//         else if (node->c == '\n') {
//             std::cout << "\\n" << std::endl;
//         }
//         else {
//             std::cout << node->c << std::endl;
//         }

//         // enter the next tree level - left and right branch
//         printBT(prefix + (isLeft ? "│   " : "    "), node->zero, true);
//         printBT(prefix + (isLeft ? "│   " : "    "), node->one, false);
//     }
// }

// void printBT(const HuffmanNode* node)
// {
//     printBT("", node, false);
// }



HuffmanTree::HuffmanTree(std::map<char, int>& counts) {
    //fill priority queue
    auto cmp = [](HuffmanNode* h1, HuffmanNode* h2) {
        return h1->freq > h2->freq;
    };
    std::priority_queue<HuffmanNode*, vector<HuffmanNode*>, decltype(cmp)> pq(cmp);

    for (auto& pair : counts) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }
    pq.push(new HuffmanNode(FILE_END, 1)); //add EOF representation

    HuffmanNode* uno;
    HuffmanNode* dos;
    while (pq.size() > 1) {
        uno = pq.top();
        pq.pop();
        dos = pq.top();
        pq.pop();
        HuffmanNode* res = new HuffmanNode('*', uno->freq + dos->freq, uno, dos);
        pq.push(res);
    }

    treeRoot = pq.top();
    // printBT(treeRoot);
}

void HuffmanTree::addCharToTree(HuffmanNode*& root, std::istream* bits, char c) {
    char curBit = bits->get();
    if (curBit == '\r' || curBit == '\n') {
        root = new HuffmanNode(c); //should be one entry per char, no nullptr check needed
    }
    else /*if(curBit != FILE_END)*/ { //if the stream is NOT empty
        if (root == nullptr) {
            root = new HuffmanNode();
        }
        if (curBit == '0') {
            addCharToTree(root->zero, bits, c);
        }
        else if (curBit == '1') {
            addCharToTree(root->one, bits, c);
        } else {
            cout << "SHOULD BE UNREACHABLE" << endl;
        }
        //else do nothing
    }
}

void HuffmanTree::trimLineEndings(std::string& str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '\r' || str[i] == '\n') {
            str.erase(i, 1);
            i--;                                                // HMMMM
        }
    }
}

HuffmanTree::HuffmanTree(std::istream* in) : treeRoot(nullptr){
    string charData;

    if (in->good()) {
        do {
            getline(*in, charData);
            trimLineEndings(charData);
            if(charData.length() > 0){
                addCharToTree(treeRoot, in, (char)stoi(charData));
            }
        } while (in->good());
    }

    // printBT(treeRoot);
}

// template<typename T>
void encodingHelper2(HuffmanNode* root, string& accumulator, std::map<char, std::string>& output) {   //eliminate output param if make class member
    if (root != nullptr) {
        if (root->isLeaf()) {
            output[root->c] = accumulator;
        }
        else {
            accumulator += "0"; //choose 0
            encodingHelper2(root->zero, accumulator, output);
            accumulator[accumulator.length() - 1] = '1'; //unchoose 0    choose 1
            encodingHelper2(root->one, accumulator, output);
            accumulator = accumulator.substr(accumulator.length() - 1); //unchoose 1
        }
    }
}

void HuffmanTree::encodingHelper(HuffmanNode* root, string accumulator, std::map<char, std::string>& output) {
    if (root->isLeaf()) {
        output[root->c] = accumulator;
    }
    else {
        encodingHelper(root->zero, accumulator + "0", output);
        encodingHelper(root->one, accumulator + "1", output);
    }
}

std::map<char, std::string> HuffmanTree::createEncodings() {
    if (charBinMap.size() == 0) {
        encodingHelper(treeRoot, "", charBinMap);
    }
    return charBinMap;
}

template<typename T>
void HuffmanTree::condThrowNPE(T* ptr, string message) {
    if (ptr == nullptr) {
        throw message;
    }
}

void HuffmanTree::compress(ifstream* input, OBitStream* output) {
    condThrowNPE(input, "input may not be nullptr for compress");
    condThrowNPE(output, "output may not be nullptr for compress");

    char c;
    createEncodings();
    while (input->good()) {
        input->read(&c, 1);
        output->writeBits(charBinMap[c]);
    }
    output->writeBits(charBinMap[FILE_END]);
    output->close(); //this shouldn't be my job
}

char HuffmanTree::decompHelper(HuffmanNode* root, IBitStream* input) {   //eliminate root param if make class member
    if (root->isLeaf()) {
        return root->c;
    }
    else {
        int res = input->readBit();
        if (res == 1) {
            return decompHelper(root->one, input);
        }
        else if(res == 0) {
            return decompHelper(root->zero, input);
        } else {
            return FILE_END;
        }
    }
}

void HuffmanTree::decompress(IBitStream* input, OBitStream* output) {
    condThrowNPE(input, "input must not be nullptr for decompress");
    condThrowNPE(input, "output must not be nullptr for decompress");
    char c = FILE_END + 1;
    char clast = FILE_END - 1;
    while (c != FILE_END) {
        c = decompHelper(treeRoot, input);
        if(c != FILE_END){
            if(clast == 'd' && c == 'e'){
                // cout << "";                                                                    //htop    
            }
            output->write(c);
        }

        if(c == FILE_END){
            // cout.flush();
        }

        clast = c;
    }
    output->close();
}

void HuffmanTree::nodeMuncher(HuffmanNode*& node){
    if(node != nullptr){
        nodeMuncher(node->zero);
        nodeMuncher(node->one);
        delete node;
    }
}

HuffmanTree::~HuffmanTree(){
    nodeMuncher(treeRoot);
}
