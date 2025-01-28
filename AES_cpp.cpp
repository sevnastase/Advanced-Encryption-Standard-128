#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <algorithm>

using namespace std;

const uint8_t s_box[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const uint8_t Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};


void subBytes(uint8_t state[4][4]) {
    uint8_t new_state[4][4]; 
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            state[i][j] = s_box[state[i][j]];
        }
    }
}

void subBytesColumn(uint8_t column[4]) {
    for(int i = 0; i < 4; ++i) {
        column[i] = s_box[column[i]];
    }
}

void shiftRows(uint8_t state[4][4]) {
    uint8_t temp;

    temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;

    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;

    temp = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = temp;
}

void addRoundKey(uint8_t state[4][4], uint8_t key[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = state[i][j] ^ key[i][j];
        }
    }
}

uint8_t galoisMultiplication(uint8_t a, uint8_t b) {
    uint8_t result = 0;

    for (int i = 0; i < 8; ++i) {
        if (b & 1) { // If the least significant bit of b is set
            result ^= a; // XOR with a
        }
        bool highBitSet = a & 0x80; // Check if the high bit of a is set
        a = (a << 1) & 0xFF; // Shift a left by 1 and mask to a byte
        if (highBitSet) {
            a ^= 0x1B; // Apply reduction polynomial
        }
        b >>= 1; // Shift b right by 1
    }

    return result & 0xFF;
}

// Matrix-Column Multiplication
void matrixColumnMultiplication(const uint8_t matrix[4][4], const uint8_t col[4], uint8_t result[4]) {
    for (int i = 0; i < 4; ++i) {
        result[i] =
            galoisMultiplication(matrix[i][0], col[0]) ^
            galoisMultiplication(matrix[i][1], col[1]) ^
            galoisMultiplication(matrix[i][2], col[2]) ^
            galoisMultiplication(matrix[i][3], col[3]);
    }
}

// Mix Columns
void mixColumns(uint8_t state[4][4]) {
    const uint8_t galoisMatrix[4][4] = {
        {0x02, 0x03, 0x01, 0x01},
        {0x01, 0x02, 0x03, 0x01},
        {0x01, 0x01, 0x02, 0x03},
        {0x03, 0x01, 0x01, 0x02}
    };

    uint8_t newColumn[4];

    // Process each column of the state
    for (int col = 0; col < 4; ++col) {
        uint8_t currentColumn[4] = {state[0][col], state[1][col], state[2][col], state[3][col]};
        matrixColumnMultiplication(galoisMatrix, currentColumn, newColumn);

        // Replace the column in the state with the new values
        for (int row = 0; row < 4; ++row) {
            state[row][col] = newColumn[row];
        }
    }
}

// RotWord: Rotate a 4-byte word
void rotWord(uint8_t word[4]) {
    uint8_t temp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;
}

// SubWord: Apply S-box substitution on a 4-byte word
void subWord(uint8_t word[4]) {
    for (int i = 0; i < 4; ++i) {
        word[i] = s_box[word[i]];
    }
}

void keyExpansion(const uint8_t key[4][4], uint8_t expandedKeys[11][4][4]) {
    // Copy the original key into the first round key
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            expandedKeys[0][i][j] = key[i][j];
        }
    }

    // Generate the remaining round keys
    for (int round = 1; round <= 10; ++round) {
        uint8_t temp[4];
        for (int i = 0; i < 4; ++i) {
            temp[i] = expandedKeys[round - 1][i][3]; // Last column of the previous key
        }

        rotWord(temp);
        subWord(temp);
        temp[0] ^= Rcon[round - 1];

        for (int i = 0; i < 4; ++i) {
            expandedKeys[round][i][0] = expandedKeys[round - 1][i][0] ^ temp[i];
        }

        for (int col = 1; col < 4; ++col) {
            for (int row = 0; row < 4; ++row) {
                expandedKeys[round][row][col] = expandedKeys[round - 1][row][col] ^ expandedKeys[round][row][col - 1];
            }
        }
    }
}

void AES(uint8_t state[4][4], uint8_t key[4][4]) {
    
    uint8_t round_keys[11][4][4];
    keyExpansion(key, round_keys);
    // Initial step:
    addRoundKey(state, round_keys[0]);

    // Intermediate steps (indices 1 to 9):
    for(int i = 1; i < 10; i++) {
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, round_keys[i]);
    }

    // Final round
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, round_keys[10]);

}

void transposeMatrix(uint8_t matrix[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            std::swap(matrix[i][j], matrix[j][i]);
        }
    }
}

int main() {

    uint8_t key[4][4]; 
    uint8_t state[4][4];     

    for (int i = 0; i < 16; i++) {
        if (scanf("%c", ((uint8_t*)key) + i) != 1) {
            printf("Error: Insufficient key bytes.\n");
            return 1;
        }
    }

    transposeMatrix(key);

    while (1) {
        int bytes_read = 0;

        for (int i = 0; i < 16; i++) {
            if (scanf("%c", ((uint8_t*)state) + i) != 1) {
                return 0; 
            }
            bytes_read++;
        }

        transposeMatrix(state);
        AES(state, key);
        transposeMatrix(state);
        fwrite(state, 1, 16, stdout);
        transposeMatrix(state);
    }

    return 0;
}