#include "HuffmanTree.h"

void printBT(const std::string& prefix, const HuffmanNode* node, bool isLeft) //                                     remove this
{
    if (node != nullptr)
    {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" :"└──" );

        // print the value of the node
        if (node->c == '*') {
            std::cout << "─┐" << std::endl;
        }
        else if (node->c == '\n') {
            std::cout << "\\n" << std::endl;
        }
        else {
            std::cout << node->c << std::endl;
        }

        // enter the next tree level - left and right branch
        printBT(prefix + (isLeft ? "│   " : "    "), node->zero, true);
        printBT(prefix + (isLeft ? "│   " : "    "), node->one, false);
    }
}

void printBT(const HuffmanNode* node)
{
    printBT("", node, false);
}



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
    printBT(this->treeRoot);
}

void addCharToTree(HuffmanNode*& root, std::istream* bits, char c) {
    char curBit = bits->get();
    if (curBit == '\r' || curBit == '\n') {
        root = new HuffmanNode(c, 5); //should be one entry per char, no nullptr check needed
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
        }
        else {
            cout << "FUCK | FUCK | FUCK | FUCK | FUCK | FUCK | FUCK | FUCK | FUCK | FUCK | FUCK | FUCK | FUCK | FUCK | FUCK | FUCK | " << endl; //                                                                                  remove
        }
    }
}

void trimLineEndings(std::string& str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '\r' || str[i] == '\n') {
            str.erase(i, 1);
        }
    }
}

HuffmanTree::HuffmanTree(std::istream* in) {
    string charData;
    string garbageData;

    if (in->good()) {
        do {
            *in >> charData;
            std::getline(*in, garbageData); //hoover up line endings
            addCharToTree(treeRoot, in, (char)stoi(charData));
        } while (in->good());
    }

    printBT(this->treeRoot);
    cout << "REMOVEME | REMOVEME | REMOVEME | REMOVEME | REMOVEME | REMOVEME | REMOVEME | " << endl;
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

void condThrowNPE(bool shouldThrow, string message) {
    if (shouldThrow) {
        throw message;
    }
}

void HuffmanTree::compress(ifstream* input, OBitStream* output) {
    condThrowNPE(input == nullptr || output == nullptr,
        "Neither parameter of compress may be nullptr");
    char c;
    while (input->good()) {
        input->read(&c, 1);
        output->writeBits(createEncodings()[c]);
    }
    output->writeBits(createEncodings()[FILE_END]);
    printBT(this->treeRoot);
    output->close(); //this shouldn't be my job
}

char decompHelper(HuffmanNode* root, IBitStream* input, OBitStream* output) {   //eliminate root param if make class member
    if (root->isLeaf()) {
        return root->c;
    }
    else {
        int res = input->readBit();
        if (res == 1) {
            return decompHelper(root->one, input, output);
        }
        else {
            return decompHelper(root->zero, input, output);
        }
    }
}

void HuffmanTree::decompress(IBitStream* input, OBitStream* output) {
    condThrowNPE(input == nullptr || output == nullptr,
        "Neither parameter of decompress may be nullptr");
    bool notEOF = true;
    char c;
    while (notEOF) {
        c = decompHelper(treeRoot, input, output);
        if (c == -1) {
            notEOF = false;
        }
        else {
            output->write(c);
        }
    }
}
