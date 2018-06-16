//
// Created by Ivan Istomin on 03/12/16.
//

#ifndef ARCHIVEALGORITHM_HW_BITIO_H
#define ARCHIVEALGORITHM_HW_BITIO_H

#include <fstream>
#include <vector>

using namespace std;

class BitOutput {
    using byte = unsigned char;

public:
    BitOutput(ofstream& stream);
    int countOfOperations = 0;

    void writeBit(const int i);
    void writeChar32(const char32_t char32);
    void writeBits(const string bits);
    void forceWrite();
private:
    ofstream& stream;
    byte buffer;
    byte bufferBitSize;

    void clearBuffer();
};

class BitInput {
public:
    BitInput(ifstream& stream);
    int countOfOperations = 0;

    bool eof = false;
    int readBit();
    char32_t readChar32();
private:
    ifstream& stream;
    int state;
    char c;

    void clearState();
};


#endif //ARCHIVEALGORITHM_HW_BITIO_H
