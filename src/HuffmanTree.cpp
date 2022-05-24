#include "HuffmanTree.h"

void printBT(const std::string& prefix, const HuffmanNode* node, bool isLeft) //                                     remove this
{
    if( node != nullptr )
    {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
        std::cout << node->c << std::endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), node->zero, true);
        printBT( prefix + (isLeft ? "│   " : "    "), node->one, false);
    }
}

void printBT(const HuffmanNode* node)
{
    printBT("", node, false);    
}



HuffmanTree::HuffmanTree(std::map<char, int>& counts) {
    //fill priority queue
    auto cmp = [](HuffmanNode* h1, HuffmanNode* h2){
        return h1->freq > h2->freq;
    };
    std::priority_queue<HuffmanNode*, vector<HuffmanNode*>, decltype(cmp)> pq(cmp);
    
    for (auto& pair : counts) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }
    pq.push(new HuffmanNode(FILE_END, 0)); //add EOF representation

    HuffmanNode* uno;
    HuffmanNode* dos;
    while (pq.size() > 1) {
        uno = pq.top(); pq.pop();
        dos = pq.top(); pq.pop();

        pq.push(new HuffmanNode('_', uno->freq + dos->freq, uno, dos));
    }

    treeRoot = pq.top();
    printBT(this->treeRoot);
}

void addCharToTree(HuffmanNode*& root, std::istream* bits, char c){
    char curBit = bits->get();
    if(curBit == '\r' || curBit == '\n'){
        root = new HuffmanNode(c, 5); //should be one entry per char, no nullptr check needed
    } else if(curBit != -1) { //if the stream
        if(root == nullptr){
            root = new HuffmanNode(); 
        }
        if(curBit == '0'){
            addCharToTree(root->zero, bits, c);
        } else if (curBit == '1'){
            addCharToTree(root->one, bits,c );
        } else {
            cout << "FUCK" << endl;
        }
    }
}

void trimLineEndings(std::string& str){
    for (int i = 0; i < str.length(); i++){
        if(str[i] == '\r' || str[i] == '\n'){
            str.erase(i, 1);
        }
    }
}

HuffmanTree::HuffmanTree(std::istream* in) {
    string charData;
    string garbageData;
    while(in->good()){
        // getline(*in, charData);
        // trimLineEndings(charData);
        *in >> charData; 
        std::getline(*in, garbageData);
        addCharToTree(treeRoot, in, (char)stoi(charData));
    }
    cout << "nothing\nnothing\nnothing\nnothing\nnothing\n" << endl;
}

// template<typename T>
void encodingHelper(HuffmanNode* root, string& accumulator, std::map<char, std::string>& output){   //eliminate output param if make class member
    if(root != nullptr){
        if(root->isLeaf()){
            output[root->c] = accumulator;
        }

        accumulator += "0"; //choose 0
        encodingHelper(root->zero, accumulator, output);
        accumulator[accumulator.length() - 1] = '1'; //unchoose 0    choose 1
        encodingHelper(root->one, accumulator, output);
        accumulator = accumulator.substr(accumulator.length() - 1); //unchoose 1
    }
}

std::map<char, std::string> HuffmanTree::createEncodings() {
    if(charBinMap.size() == 0){
    // std::map<char, std::string> charBinMap;
        std::string accumulator;
        encodingHelper(treeRoot, accumulator, charBinMap);
    }
    return charBinMap;
}

void condThrowNPE(bool shouldThrow, string message){
    if(shouldThrow){
        throw message;
    }
}

void HuffmanTree::compress(ifstream* input, OBitStream* output) {
    condThrowNPE(input == nullptr || output == nullptr, 
        "Neither parameter of decompress may be nullptr");
    char c;
    while (input->good()){
        input->read(&c, 1); 
        output->writeBits(createEncodings()[c]);
    }
    
}

char decompHelper(HuffmanNode* root, IBitStream* input, OBitStream* output){   //eliminate root param if make class member
    if(root->isLeaf()){
        return root->c;
    } else {
        int res = input->readBit();
        if(res == 1){
            return decompHelper(root->one, input, output);
        } else {
            return decompHelper(root->zero, input, output);
        }
    }
}

void HuffmanTree::decompress(IBitStream* input, OBitStream* output) {
    condThrowNPE(input == nullptr || output == nullptr, 
        "Neither parameter of decompress may be nullptr");
    while(input->good()){
        output->write(decompHelper(treeRoot, input, output));
    }
}
