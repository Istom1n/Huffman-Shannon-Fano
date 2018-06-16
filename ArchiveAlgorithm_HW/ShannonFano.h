//
// Created by Ivan Istomin on 15/11/16.
//

#ifndef ARCHIVEALGORITHM_HW_SHANNON_FANO_H
#define ARCHIVEALGORITHM_HW_SHANNON_FANO_H

#include <map>
#include <string>
#include <vector>
#include "BitIO.h"

using namespace std;

class ShannonFano {
    struct Node {
        char32_t data;
        int freq;
        Node* left, * right;

        Node(char32_t data, int freq) {
            left = right = nullptr;
            this->data = data;
            this->freq = freq;
        }

        Node(char32_t data, Node* left, Node* right) {
            this->data = data;
            this->left = left;
            this->right = right;
        }

        bool isLeafNode() {
            return left == nullptr;
        };
    };

public:
    struct Letter {
        char32_t letter;
        int freq;
        string code = "";

        Letter(char32_t letter, int freq) {
            this->letter = letter;
            this->freq = freq;
        }
    };

    Node* root;
    int countOfOperations = 0;

    void buildTree(vector<Letter>& letters);
    void startShannonFano(vector<Letter>& letters, int b, int e);
    int median(vector<Letter>& letters, int b, int e);
    void encodeNode(Node* node, BitOutput*&writer);
    Node* readNode(BitInput*&reader);
    void read(BitInput*& reader, basic_ofstream<char32_t>& outputStream, int length);

};


#endif //ARCHIVEALGORITHM_HW_SHANNON_FANO_H
