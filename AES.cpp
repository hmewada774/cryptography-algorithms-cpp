#include <iostream>
#include <chrono>
#include <fstream>
#include <bitset>
using namespace std;

const int Nb = 4;

unsigned char sBox[256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,
    0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    // Only 16 values defined - for real AES you'd need full 256-byte S-box
    // Fill in default linear map for undefined values for this test
};

void fillMissingSBox()
{
    for (int i = 16; i < 256; ++i)
    {
        sBox[i] = i; // Identity mapping for simplicity
    }
}

void printMemoryUsage()
{
    ifstream status("/proc/self/status");
    string line;
    while (getline(status, line))
    {
        if (line.substr(0, 6) == "VmRSS:")
            cout << "Memory Usage (RSS):" << line.substr(6) << endl;
    }
}

void printStateBinary(unsigned char state[4][Nb], string label)
{
    cout << "\n"
         << label << " (Binary):" << endl;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < Nb; ++j)
            cout << bitset<8>(state[i][j]) << " ";
        cout << endl;
    }
}

void printState(unsigned char state[4][Nb])
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < Nb; ++j)
            printf("%02X ", state[i][j]);
        cout << endl;
    }
}

void addRoundKey(unsigned char state[4][Nb], unsigned char key[4][Nb])
{
    cout << "\n[AddRoundKey]" << endl;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < Nb; ++j)
        {
            cout << "State(" << i << "," << j << "): "
                 << bitset<8>(state[i][j]) << " ^ " << bitset<8>(key[i][j]) << " = ";
            state[i][j] ^= key[i][j];
            cout << bitset<8>(state[i][j]) << endl;
        }
}

void subBytes(unsigned char state[4][Nb])
{
    cout << "\n[SubBytes]" << endl;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < Nb; ++j)
        {
            unsigned char before = state[i][j];
            state[i][j] = sBox[state[i][j]];
            cout << "SBox(" << bitset<8>(before) << ") = " << bitset<8>(state[i][j]) << endl;
        }
}

void shiftRows(unsigned char state[4][Nb])
{
    cout << "\n[ShiftRows]" << endl;
    unsigned char temp;
    temp = state[1][0];
    for (int i = 0; i < Nb - 1; ++i)
        state[1][i] = state[1][i + 1];
    state[1][Nb - 1] = temp;

    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;

    temp = state[3][Nb - 1];
    for (int i = Nb - 1; i > 0; --i)
        state[3][i] = state[3][i - 1];
    state[3][0] = temp;

    printStateBinary(state, "After ShiftRows");
}

void aesEncrypt(unsigned char state[4][Nb], unsigned char key[4][Nb])
{
    cout << "\n=== Initial AddRoundKey ===";
    addRoundKey(state, key);
    printStateBinary(state, "State After Initial AddRoundKey");

    cout << "\n=== SubBytes ===";
    subBytes(state);
    printStateBinary(state, "State After SubBytes");

    cout << "\n=== ShiftRows ===";
    shiftRows(state);

    cout << "\n=== Final AddRoundKey ===";
    addRoundKey(state, key);
    printStateBinary(state, "Final State After AddRoundKey");
}

int main()
{
    fillMissingSBox();

    unsigned char state[4][Nb] = {
        {'T', 'H', 'I', 'S'},
        {'I', 'S', 'A', '_'},
        {'T', 'E', 'S', 'T'},
        {'!', '!', '!', '!'}};

    unsigned char key[4][Nb] = {
        {0x2b, 0x28, 0xab, 0x09},
        {0x7e, 0xae, 0xf7, 0xcf},
        {0x15, 0xd2, 0x15, 0x4f},
        {0x16, 0xa6, 0x88, 0x3c}};

    cout << "Plaintext Block (Hex):\n";
    printState(state);
    printStateBinary(state, "Plaintext");

    auto start = chrono::high_resolution_clock::now();

    aesEncrypt(state, key);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> exec_time = end - start;

    cout << "\nEncrypted Block (Hex):\n";
    printState(state);

    cout << "\nExecution Time: " << exec_time.count() << " microseconds" << endl;
    printMemoryUsage();

    cout << "\n=== Cryptanalysis Insight ===" << endl;
    cout << "- Limited S-box defined (only 16 entries), rest defaulted to identity mapping.\n";
    cout << "- No MixColumns -> linearity may preserve patterns.\n";
    cout << "- Same key used for both AddRoundKeys = weak key schedule.\n";
    cout << "- Secure AES uses nonlinear S-box, key expansion, and more rounds.\n";
    cout << "- This design is good for understanding structure, not secure in practice.\n";

    return 0;
}
