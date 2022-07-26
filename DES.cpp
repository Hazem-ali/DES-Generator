#include <iostream>
#include <string>
#include <math.h>
#include <sstream>
#include <bitset>
#include <assert.h>
#include <stdlib.h>
using namespace std;

#ifdef __GNUC__
#define __rdtsc __builtin_ia32_rdtsc
#else
#include <intrin.h>
#endif

typedef unsigned long long u64;
typedef unsigned long u32;

// defining tables
int pc1[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4};

int pc2[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32};

int permutation_table[32] = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25};

int inverse_permutation[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25};

int initial_permutation[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7};

int expansion_table[48] = {
    32, 1, 2, 3, 4, 5, 4, 5,
    6, 7, 8, 9, 8, 9, 10, 11,
    12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21,
    22, 23, 24, 25, 24, 25, 26, 27,
    28, 29, 28, 29, 30, 31, 32, 1};

int s_box[8][4][16] =
    {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
      0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
      4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
      15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
     {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
      3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
      0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
      13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
     {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
      13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
      13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
      1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
     {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
      13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
      10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
      3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
     {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
      14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
      4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
      11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
     {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
      10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
      9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
      4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
     {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
      13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
      1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
      6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
     {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
      1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
      7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
      2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}};

u64 round_keys[16];

// Utility Functions

void Swap(u64 &a, u64 &b)
{
    u64 temp_A = a;
    a = b;
    b = temp_A;
}

// Function to do a circular left shift by 1
u32 RotateLeft(u32 number, u32 times)
{
    /* In number<<times, last times bits are 0. To
     put first 3 bits of number at
    last, do bitwise or of number<<times
    with number >> (28 - times) */
    // All result is anded with 0x0FFFFFFF to take out 28 bits
    return ((number << times) | (number >> (28 - times))) & 0x0FFFFFFF;
}

void create_DES_Keys(u64 key)
{
    // applying pc1
    u64 PC1_Key = 0;
    for (int i = 0; i < 56; i++)
    {

        PC1_Key |= (key >> (64 - pc1[56 - 1 - i]) & 1) << i;
    }
    u32 left = u32((PC1_Key >> 28) & 0x0FFFFFFF);
    u32 right = u32(PC1_Key & 0x0FFFFFFF);
    for (int i = 0; i < 16; i++)
    {
        // For rounds 1, 2, 9, 16 the key_chunks are shifted by one.
        if (i == 0 || i == 1 || i == 8 || i == 15)
        {
            // Shift Once
            left = RotateLeft(left, 1);
            right = RotateLeft(right, 1);
        }
        // For other rounds, the key_chunks are shifted by two
        else
        {
            // Shift Twice
            left = RotateLeft(left, 2);
            right = RotateLeft(right, 2);
        }

        // Combining right and left
        u64 combined_key = u64(u64(left) << 28 | u64(right));


        // applying pc2
        u64 round_key = 0;
        for (int j = 0; j < 48; j++)
        {

            round_key |= (combined_key >> (56 - pc2[48 - 1 - j]) & 1) << j;

        }
        // key is ready
        round_keys[i] = round_key;
    }
}

// in case of decryption, we reverse key orders
void reverseKeys()
{
    // string round_keys[16];
    u64 newKeys[16];
    for (int i = 0; i < 16; i++)
    {
        newKeys[i] = round_keys[15 - i];
    }

    for (int i = 0; i < 16; i++)
    {
        round_keys[i] = newKeys[i];
    }
}

u64 Apply_DES(u64 plainText, u64 key)
{

    // Apply initial permutation for plaintext output 64
    u64 initially_permutated_text = 0;
    for (int i = 0; i < 64; i++)
    {
        initially_permutated_text |= (plainText >> (64 - initial_permutation[64 - 1 - i]) & 1) << i;
    }

    // splitting into left and right

    u64 left = u64((initially_permutated_text >> 32) & 0xFFFFFFFF);
    u64 right = u64(initially_permutated_text & 0xFFFFFFFF);


    // Rounding 16 times
    for (int i = 0; i < 16; i++)
    {
        // Applying DES inner Function
        // Expansion
        u64 right_expanded = 0;
        for (int j = 0; j < 48; j++)
        {
            // right_expanded is 48 bits
            right_expanded |= (right >> (32 - expansion_table[48 - 1 - j]) & 1) << j;
        }
 
        // XOR
        u64 inner_XOR = (right_expanded ^ round_keys[i]);
 

        // S-Box
        u64 S_Box_Result = 0;
        for (int j = 0; j < 8; j++)
        {
            // for each s-box

            // Extracting needed 6 bits
            u64 working_bits = (inner_XOR & (0xFC0000000000 >> (j * 6))) >> ((7 - j) * 6);

            // Concatenating bits

            u32 row_bits = working_bits & 0x21; // & 100001 to take row position
            u32 first_bit = row_bits & 1; // holding first bit

            u32 row = (row_bits >> 4) | first_bit;
            u32 col = (working_bits & 0x1E) >> 1; // & 011110 to take col position

            

            u32 val = s_box[j][row][col];

            // emptying place for newcoming value
            S_Box_Result = S_Box_Result << 4;

            // appending new value
            S_Box_Result |= val;
        }

        // Permutation
        u64 permutated = 0;
        for (int j = 0; j < 32; j++)
        {
            permutated |= (S_Box_Result >> (32 - permutation_table[32 - 1 - j]) & 1) << j;
        }

        // XOR with left to get new right
        u64 outer_XOR = (permutated ^ left);
       

        // Adjusting places
        left = right;
        right = outer_XOR;
        
    }

    // Apply 32bit Swap and combine

    Swap(left, right);

    u64 combined_text = u64((left << 32) | right);

    // Apply inverse initial permutation output 64
    u64 Cipher = 0;
    for (int i = 0; i < 64; i++)
    {
        Cipher |= (combined_text >> (64 - inverse_permutation[64 - 1 - i]) & 1) << i;
    }

    return Cipher;
}

u64 read_DES_input(const char *data)
{
    
    u64 value = 0;
    for (int i = 0; i < 16; ++i)
    {
        char c = data[i];
        if (c >= '0' && c <= '9')
            value |= (u64)(c - '0') << ((15 - i) << 2);
        else if (c >= 'A' && c <= 'F')
            value |= (u64)(c - 'A' + 10) << ((15 - i) << 2);
        else if (c >= 'a' && c <= 'f')
            value |= (u64)(c - 'a' + 10) << ((15 - i) << 2);
    }
    return value;
}

int main(int argc, char **argv)
{

    // first bit is bit 0
    // u32 x = 0xE;
    // x = RotateLeft(x, 1);
    // printf("%04llX \n", x);
    // cout << ("%0ix", x);


    // taking input (operation, data in hex, key in hex)
    string operation = argv[1];
    u64 u64_data = read_DES_input(argv[2]);
    u64 u64_key = read_DES_input(argv[3]);

    

    // // Generating Keys
    create_DES_Keys(u64_key);
    string message = "cipher: ";
    if (operation == "decrypt")
    {
        reverseKeys();
        message = "plain: ";
    }

    //code under benchmark...

    long long t1 = __rdtsc();
    u64 result = Apply_DES(u64_data, u64_key);
    long long t2 = __rdtsc();
    cout << message << hex << uppercase << result ;
    printf("\nCycles: %lld\n", t2 - t1);
    

    // 7DB033DD14D6F975
    // 7DB033DD14D6F975

    // encrypt 7A6C731D22347676 1323445A6D788381
    // encrypt 0000000000000000 0000000000000000
    // correct cipher: 7DB033DD14D6F975
    // correct cipher: 0111110110110000001100111101110100010100110101101111100101110101
    // 7A6C731D22347676

    return 0;
}
