//
// Created by Ivan Istomin on 03/12/16.
//

#include <iostream>
#include "BitIO.h"
#include "Utils.h"

using namespace std;

BitOutput::BitOutput(ofstream& outputStream) : stream(outputStream) {
    clearBuffer();
};

void BitOutput::writeBit(const int i) {
    buffer |= i << (7 - bufferBitSize);
    bufferBitSize++;

    if (bufferBitSize == 8) {
        stream.write((const char*)& buffer, sizeof(buffer));
        clearBuffer();

        countOfOperations += 2;
    }

    countOfOperations += 5;
}

void BitOutput::writeChar32(const char32_t char32) {
    for (int i = 31; i >= 0; --i) {
        bool b = (bool) ((char32 >> i) & 1);
        writeBit(b);

        countOfOperations += 4;
    }
}

void BitOutput::writeBits(const string bits) {
    for (int i = 0; i < bits.size(); i++) {
        writeBit(to_bool(bits[i]));

        countOfOperations += 1;
    }
}

void BitOutput::forceWrite() {
    if (bufferBitSize > 0) {
        stream << buffer;
        clearBuffer();
    }

    countOfOperations += 2;
}

void BitOutput::clearBuffer() {
    buffer = 0;
    bufferBitSize = 0;

    countOfOperations += 2;
}

BitInput::BitInput(ifstream &inputStream) : stream(inputStream) {
    clearState();
    stream.read(&c, sizeof(c));

    countOfOperations += 2;
}

int BitInput::readBit() {
    if (state == -1) {
        stream.read(&c, sizeof(c));
        clearState();
    }

    int bit = ((c >> state) & 1);

    --state;

    if (state == -1) {
        if (stream.peek() == EOF) eof = true;
        countOfOperations += 1;
    }

    countOfOperations += 6;

    return bit;
}

char32_t BitInput::readChar32() {
    char32_t char32 = 0;

    for (int i = 0; i < 32; ++i) {
        int bit = readBit();
        char32 |= bit << (31 - i);

        countOfOperations += 2;
    }

    countOfOperations += 1;

    return char32;
}

void BitInput::clearState() {
    countOfOperations += 1;

    state = 7;
}