#include <iostream>
#include <string>
#include <ctime>
#include <unordered_map>
#include <bitset>
using namespace std;

// Feistel function (S-box)
int feistel(int half, int key)
{
    int result = (half ^ key) & 0xF;                                  // Using XOR and masking with 0xF to ensure 4-bit output
    cout << "  S-box Output (Binary): " << bitset<4>(result) << endl; // Output of S-box in binary
    return result;
}

// Encryption for a block using Feistel network
int encryptBlock(int block, int k1, int k2, int k3, int k4)
{
    int L = (block >> 4) & 0xF; // Left half of the block
    int R = block & 0xF;        // Right half of the block
    int keys[4] = {k1, k2, k3, k4};

    for (int i = 0; i < 4; ++i)
    {
        int temp = R;
        R = L ^ feistel(R, keys[i]);
        L = temp;

        // Output the round in binary
        cout << " Round " << (i + 1) << ": L = " << bitset<4>(L) << ", R = " << bitset<4>(R) << endl;
    }
    return (L << 4) | R;
}

// Decryption for a block using Feistel network
int decryptBlock(int block, int k1, int k2, int k3, int k4)
{
    int L = (block >> 4) & 0xF;     // Left half of the block
    int R = block & 0xF;            // Right half of the block
    int keys[4] = {k4, k3, k2, k1}; // Reverse key order for decryption

    for (int i = 0; i < 4; ++i)
    {
        int temp = L;
        L = R ^ feistel(L, keys[i]);
        R = temp;

        // Output the round in binary
        cout << " Round " << (i + 1) << ": L = " << bitset<4>(L) << ", R = " << bitset<4>(R) << endl;
    }
    return (L << 4) | R;
}

// CBC Encryption
string cbcEncrypt(string plaintext, int iv, int k1, int k2, int k3, int k4)
{
    string ciphertext = "";
    int prev = iv;

    for (char c : plaintext)
    {
        int block = (int)c;
        int xored = block ^ prev;
        cout << "Block (XOR with IV): " << bitset<8>(xored) << endl; // Block after XOR with IV
        int enc = encryptBlock(xored, k1, k2, k3, k4);
        ciphertext += (char)enc;
        prev = enc;
    }
    return ciphertext;
}

// CBC Decryption
string cbcDecrypt(string ciphertext, int iv, int k1, int k2, int k3, int k4)
{
    string plaintext = "";
    int prev = iv;

    for (char c : ciphertext)
    {
        int block = (int)c;
        int dec = decryptBlock(block, k1, k2, k3, k4);
        int orig = dec ^ prev;
        plaintext += (char)orig;
        prev = block;
    }
    return plaintext;
}

// Frequency Analysis for Cryptanalysis
void analyzeFrequency(string text)
{
    unordered_map<char, int> freqMap;
    for (char c : text)
    {
        freqMap[c]++;
    }

    cout << "\n[Character Frequency Analysis]\n";
    for (auto &pair : freqMap)
    {
        cout << "'" << pair.first << "': " << pair.second << endl;
    }
}

int main()
{
    string text;
    cout << "Enter plaintext: ";
    getline(cin, text);

    int iv = 99;
    int k1 = 5, k2 = 9, k3 = 3, k4 = 12;

    // Measure execution time
    clock_t start = clock();

    // Encryption
    string encrypted = cbcEncrypt(text, iv, k1, k2, k3, k4);

    // Measure end time
    clock_t end = clock();
    double execTime = double(end - start) / CLOCKS_PER_SEC;

    cout << "\nEncrypted (raw bytes): ";
    for (char c : encrypted)
    {
        cout << (int)(unsigned char)c << " ";
    }

    cout << "\nDecryption started..." << endl;

    // Decryption
    string decrypted = cbcDecrypt(encrypted, iv, k1, k2, k3, k4);
    cout << "Decrypted: " << decrypted << endl;

    // Execution time
    cout << "\nExecution Time: " << execTime << " seconds" << endl;

    // Memory Usage Estimation
    int memoryUsed = sizeof(text) + sizeof(encrypted) + sizeof(decrypted) + sizeof(iv) + sizeof(k1) + sizeof(k2) + sizeof(k3) + sizeof(k4);
    cout << "Estimated Memory Usage: " << memoryUsed << " bytes" << endl;

    // Perform basic cryptanalysis (frequency analysis)
    analyzeFrequency(encrypted);

    return 0;
}
