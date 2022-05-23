struct HuffmanNode {
    char c;
    int freq;
    HuffmanNode* zero;
    HuffmanNode* one;

    HuffmanNode(char c = '_', int freq = 0, 
        HuffmanNode* zero = nullptr, HuffmanNode* one = nullptr);
    bool isLeaf() const;

    bool operator<(HuffmanNode& h2);
    bool operator()(HuffmanNode* h1, HuffmanNode* h2);
};