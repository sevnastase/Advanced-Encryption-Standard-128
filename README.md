# AES Encryption in C++

## Table of contents

1. Introduction
2. Supported input
3. AES-specific operations
4. AES-128 structure
5. Project configuration
6. Compilation and execution
7. Relevant numerical values

### Introduction 

This project implements AES encryption in C++. It functions for the AES-128 version, which entails that the key to be used is 128 bits.
The key can therefore be visualized as a 4x4 quadratic matrix. Round keys (which will be introduced in section 3) are also 4x4 matrices.
The programming language of choice is C++ because of its good performance over computationally expensive tasks such as **encryption**. 
C++ also facilitates quick bitwise operations (which are used in the Galois multiplication employed by AES).  

### Supported input

The input is expected to be raw bytes. For simplicity, the project does not account for input consisting of strings. 
However, that can be easily accommodated by adding a stringToByte() functionality that takes the contents of a string and creates a table with the corresponding bytes.
- First 16 bytes are the key bytes.
- After the first 16 bytes have been parsed, every new 16-byte batch is considered a block to be encrypted.

### AES-specific operations

Firstly, let us define keyExpansion, which is performed on the initial key, rather than the block.
- Key expansion: given the key, expand it into 11 new round keys to increase security. This is done by using R_CON (see section 7 "Relevant numerical values") and performing certain XOR operations on the columns of the previously derived key.
- The first key is equal to the initial key, then the 2nd key is derived using the first, 3rd using the second, etc. until all 11 keys are formed

Let us define the operations performed on an arbitrary block of the input.
- subBytes: given S_BOX (see section 7 "Relevant numerical values"), for every element in block, look up its corresponding S_BOX value and assign that value to it.
- shiftRows: For i form 0 to 3, on the i-th row of the block, shift the elements i cells to the left.
- mixColumns: given the Galois matrix (see section 7 "Relevant numerical values"), multiply it in the Galois field GF(2^8) with every column in the block. 
- addRoundKey: XOR the block with its corresponding round key. addRoundKey also takes the corresponding round key as a parameter, along with the block.

### AES-128 structure

Since we are treating the 128-bit version of AES, there will be a total of 11 rounds.
- The initial round: only perform addRoundKey on the block and the first round key. Let us denote this as round 0.
Then, 9 full rounds, all 4 of the operations defined above are performed. For round i (i from 1 to 9):
- subBytes(block)
- shiftRows(block)
- mixColumns(block)
- addRoundKey(block, i-th round key)
In the last round, perform all operations except for mixColumns. Last round looks as follows:
- subBytes(block)
- shiftRows(block)
- addROundKey(block, i-th round key)

### Project configuration

Clone the repository.
git clone https://github.com/sevnastase/Advanced-Encryption-Standard-128.git
Navigate to directory.
cd Advanced-Encryption-Standard-128


### Compilation and execution

Ensure GCC compiler is installed. This project was compiled using G++. To verify G++ is installed on system, run: g++ --version
In terminal, run the following commands:
g++ <source_file_name> -o <output_file_name> (to compile the program using G++ compiler)
.\<path_to_output_file> (execute compiled program on system)
Then, provide necessary input (in raw bytes).
