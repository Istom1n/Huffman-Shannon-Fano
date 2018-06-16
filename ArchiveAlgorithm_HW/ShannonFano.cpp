//
// Created by Ivan Istomin on 15/11/16.
//

#include "ShannonFano.h"
#include <cmath>

using namespace std;

void ShannonFano::startShannonFano(vector<Letter>& letters, int start, int end) {
    if (end > start) {
        int m = median(letters, start, end);

        for (int i = start; i <= end; ++i) {
            letters[i].code.append(to_string(i > m));

            countOfOperations += 4;
        }

        startShannonFano(letters, start, m);
        startShannonFano(letters, m + 1, end);

        countOfOperations += 3;
    }
}

int ShannonFano::median(vector<Letter>& letters, int start, int end) {
    int sumOne = 0;

    for (int i = start; i < end; ++i) {
        sumOne += letters[i].freq;

        countOfOperations += 2;
    }

    int sumTwo = letters[end].freq;
    int m = end;

    double d = 0;

    do {
        d = abs(sumOne - sumTwo);
        m -= 1;
        sumOne -= letters[m].freq;
        sumTwo += letters[m].freq;
    } while (abs(sumOne - sumTwo) <= d);

    countOfOperations += 20;

    return m;
}

void ShannonFano::encodeNode(Node* node, BitOutput*& writer) {
    if (node->isLeafNode()) {
        (*writer).writeBit(1);
        (*writer).writeChar32(node->data);
    } else {
        (*writer).writeBit(0);

        encodeNode(node->left, writer);
        encodeNode(node->right, writer);
    }
}

void ShannonFano::buildTree(vector<Letter>& letters) {
    root = new Node(0, nullptr, nullptr);

    for (Letter l : letters) {
        Node* state = root;

        for (int i = 0; i < l.code.length(); ++i) {
            if (l.code[i] == '0') {
                if (state->left == nullptr) {
                    state->left= new Node(0, nullptr, nullptr);
                }

                state = state->left;
            } else {
                if (state->right== nullptr) {
                    state->right= new Node(0, nullptr, nullptr);
                }

                state = state->right;
            }
        }

        state->data = l.letter;
    }
}

ShannonFano::Node* ShannonFano::readNode(BitInput*& reader) {
    int tmp = (*reader).readBit();

    if (tmp == 1) {
        countOfOperations += 3;

        return new Node((*reader).readChar32(), nullptr, nullptr);
    } else {
        Node* left = readNode(reader);
        Node* right = readNode(reader);

        root = new Node(0, left, right);

        countOfOperations += 5;

        return root;
    }
}

void ShannonFano::read(BitInput*& reader, basic_ofstream<char32_t>& outputStream, int length) {
    Node* state = root;
    int lengthState = 0;

    while (!reader->eof) {
        while (!state->isLeafNode()) {
            if (reader->readBit() == 0) {
                state = state->left;

                countOfOperations += 1;
            } else {
                state = state->right;

                countOfOperations += 1;
            }

            countOfOperations += 1;
        }

        countOfOperations += 5;

        ++lengthState;
        outputStream << state->data;

        if (lengthState == length - 1) break;

        countOfOperations += 1;

        state = root;
    }
}