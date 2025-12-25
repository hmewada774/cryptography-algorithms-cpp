#include <iostream>
#include <ctime>
using namespace std;

// Function F with detailed debug output (serves as our S-box substitute)
unsigned int F(unsigned int x, unsigned int key)
{
    cout << "    F() Input: 0x" << std::hex << x << ", Key: 0x" << key << "\n";
    unsigned int temp = x ^ key;
    cout << "    After XOR: 0x" << std::hex << temp << "\n";
    unsigned int temp2 = ((temp << 4) | (temp >> 4)) ^ key;
    cout << "    Shift and XOR: 0x" << std::hex << temp2 << "\n";
    return temp2;
}

// Blowfish-like encryption: 4 rounds with detailed round outputs.
void blowfishEncrypt(unsigned int &L, unsigned int &R, unsigned int key[4])
{
    for (int i = 0; i < 4; ++i)
    {
        cout << "\n-- Encryption Round " << i + 1 << " --\n";
        cout << "  Before Round: L = 0x" << std::hex << L << ", R = 0x" << R << "\n";
        unsigned int Fout = F(R, key[i]);
        cout << "  F(R, key[" << i << "]) = 0x" << std::hex << Fout << "\n";
        L = L ^ Fout;
        cout << "  After XOR: L = 0x" << std::hex << L << "\n";
        // Swap L and R
        unsigned int temp = L;
        L = R;
        R = temp;
        cout << "  After Swap: L = 0x" << std::hex << L << ", R = 0x" << R << "\n";
    }
    // Final swap to restore order
    unsigned int temp = L;
    L = R;
    R = temp;
    cout << "\nFinal state after encryption (post-swap): L = 0x" << std::hex << L << ", R = 0x" << R << "\n";
}

// Blowfish-like decryption: inverse order with round outputs.
void blowfishDecrypt(unsigned int &L, unsigned int &R, unsigned int key[4])
{
    for (int i = 3; i >= 0; --i)
    {
        cout << "\n-- Decryption Round " << (4 - i) << " --\n";
        cout << "  Before Round: L = 0x" << std::hex << L << ", R = 0x" << R << "\n";
        unsigned int Fout = F(R, key[i]);
        cout << "  F(R, key[" << i << "]) = 0x" << std::hex << Fout << "\n";
        L = L ^ Fout;
        cout << "  After XOR: L = 0x" << std::hex << L << "\n";
        // Swap L and R
        unsigned int temp = L;
        L = R;
        R = temp;
        cout << "  After Swap: L = 0x" << std::hex << L << ", R = 0x" << R << "\n";
    }
    // Final swap to restore original state
    unsigned int temp = L;
    L = R;
    R = temp;
    cout << "\nFinal state after decryption (post-swap): L = 0x" << std::hex << L << ", R = 0x" << R << "\n";
}

// Return the number of differing bits (Hamming distance) between two 32-bit words.
int hammingDistance(unsigned int a, unsigned int b)
{
    unsigned int diff = a ^ b;
    int count = 0;
    for (int i = 0; i < 32; i++)
    {
        count += (diff >> i) & 1;
    }
    return count;
}

// Simple differential analysis on the encryption function.
// Flips each bit of the combined plaintext (L, R) one at a time,
// re-encrypting and then printing the Hamming distance difference.
void differentialAnalysis(unsigned int origL, unsigned int origR, unsigned int key[4])
{
    cout << "\n[Differential Cryptanalysis]\n";
    // Encrypt original
    unsigned int L0 = origL, R0 = origR;
    blowfishEncrypt(L0, R0, key);

    // For every bit in 64-bit plaintext
    for (int bit = 0; bit < 64; bit++)
    {
        unsigned int modL = origL, modR = origR;
        if (bit < 32)
        {
            modL ^= (1u << bit);
        }
        else
        {
            modR ^= (1u << (bit - 32));
        }
        unsigned int L1 = modL, R1 = modR;
        blowfishEncrypt(L1, R1, key);
        int hd = hammingDistance(L0, L1) + hammingDistance(R0, R1);
        cout << " Bit flipped at position " << bit << ": Hamming distance = " << hd << "\n";
    }
}

// A simple linear analysis: for a small set of plaintexts, see how often a specific input bit matches a specific output bit.
void linearAnalysis(unsigned int key[4])
{
    cout << "\n[Linear Cryptanalysis]\n";
    int matchCount = 0;
    int total = 0;
    // In this example we examine the least significant bit of L input vs the most significant bit of L output.
    for (unsigned int i = 0; i < 256; i++)
    {
        // Vary only the lower 8 bits of L, keep R fixed
        unsigned int L = i, R = 0x9ABCDEF0; // fixed R from original
        unsigned int origBit = L & 1;       // LSB of L
        unsigned int outL = L, outR = R;
        blowfishEncrypt(outL, outR, key);
        // Get the MSB of L after encryption
        unsigned int outBit = (outL >> 31) & 1;
        if (origBit == outBit)
            matchCount++;
        total++;
    }
    double bias = (matchCount - (total / 2.0)) / (total / 2.0);
    cout << "Correlation (Input LSB vs Output MSB): Bias = " << bias << "\n";
}

int main()
{
    // Hardcoded plaintext (split into two 32-bit halves) and key
    unsigned int L = 0x12345678;
    unsigned int R = 0x9ABCDEF0;
    unsigned int key[4] = {0x11111111, 0x22222222, 0x33333333, 0x44444444};

    cout << "Plaintext: " << std::hex << L << " " << R << "\n";

    clock_t start = clock();

    // Encrypt with detailed round outputs.
    blowfishEncrypt(L, R, key);
    cout << "\nEncrypted: " << std::hex << L << " " << R << "\n";

    // Decrypt with detailed round outputs.
    blowfishDecrypt(L, R, key);
    cout << "\nDecrypted: " << std::hex << L << " " << R << "\n";

    clock_t end = clock();
    double execTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "\nExecution Time: " << execTime << " seconds\n";

    // Basic estimated memory usage for our main variables.
    int memoryUsed = sizeof(L) + sizeof(R) + sizeof(key);
    cout << "Estimated Memory Usage (variables): " << memoryUsed << " bytes\n";

    // Perform basic cryptanalysis.
    differentialAnalysis(0x12345678, 0x9ABCDEF0, key);
    linearAnalysis(key);

    return 0;
}
