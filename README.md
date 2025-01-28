# AES Encryption in C++

## Table of contents

1. Introduction
2. Supported input
3. AES-128 structure
4. Project configuration
5. Compilation and execution

### Introduction 

This project implements AES encryption in C++. It functions for the AES-128 version, which entails that the key to be used is 128 bits.
The key can therefore be visualized as a 4x4 quadratic matrix. Round keys (which will be introduced in section 3) are also 4x4 matrices.
The programming language of choice is C++ because of its good performance over computationally expensive tasks such as **encryption**. 
C++ also facilitates quick bitwise operations (which are used in the Galois multiplication employed by AES).  

### Supported input

The input is expected to be raw bytes. For simplicity, the project does not account for input consisting of strings. 
However, that can be easily accommodated by adding a stringToByte() functionality that takes the contents of a string and creates a table with the corresponding bytes.

### Compilation and execution

In terminal, run the following commands:
g++ <source_file_name> -o <output_file_name> (to compile the program using G++ compiler)
.\<path_to_output_file> (execute compiled program on system)
Then, provide necessary input (in raw bytes).
