#include <iostream>
#include <chrono>
#include <fstream>
#include <bitset>
using namespace std;

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

void printBinaryBlock(const unsigned char *block, const string &label)
{
    cout << label << " (Binary):" << endl;
    for (int i = 0; i < 16; ++i)
    {
        cout << bitset<8>(block[i]) << " ";
        if ((i + 1) % 4 == 0)
            cout << endl;
    }
    cout << endl;
}

void addRoundKey(unsigned char *state, const unsigned char *key)
{
    for (int i = 0; i < 16; ++i)
        state[i] ^= key[i];
}

void subBytes(unsigned char *state)
{
    for (int i = 0; i < 16; ++i)
    {
        unsigned char original = state[i];
        state[i] = (original << 1) | (original >> 7); // simple "S-box"
        cout << "S-box[" << i << "]: " << bitset<8>(original)
             << " -> " << bitset<8>(state[i]) << endl;
    }
    cout << endl;
}

void shiftRows(unsigned char *state)
{
    unsigned char temp[16];
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            temp[r * 4 + c] = state[r * 4 + (c + r) % 4];
    for (int i = 0; i < 16; ++i)
        state[i] = temp[i];
}

void miniAESEncrypt(unsigned char *data, const unsigned char *key)
{
    for (int round = 0; round < 3; ++round)
    {
        cout << "====== Round " << round + 1 << " ======" << endl;
        subBytes(data);
        shiftRows(data);
        addRoundKey(data, key);
        printBinaryBlock(data, "State After Round " + to_string(round + 1));
    }
}

void printBlockHex(const unsigned char *block)
{
    for (int i = 0; i < 16; ++i)
        printf("%02X ", block[i]);
    cout << endl;
}

int main()
{
    unsigned char data[16] = {
        'H', 'E', 'L', 'L',
        'O', ' ', 'W', 'O',
        'R', 'L', 'D', '!',
        'X', 'X', 'X', 'X'};

    unsigned char key[16] = {
        0x0F, 0x1E, 0x2D, 0x3C,
        0x4B, 0x5A, 0x69, 0x78,
        0x87, 0x96, 0xA5, 0xB4,
        0xC3, 0xD2, 0xE1, 0xF0};

    cout << "Original (Hex):\n";
    printBlockHex(data);
    printBinaryBlock(data, "Original");

    auto start = chrono::high_resolution_clock::now();

    miniAESEncrypt(data, key);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> exec_time = end - start;

    cout << "Encrypted (Hex):\n";
    printBlockHex(data);

    cout << "\nExecution Time: " << exec_time.count() << " microseconds" << endl;
    printMemoryUsage();

    // Basic Cryptanalysis Insight (simple pattern analysis)
    cout << "\n===== Cryptanalysis Insight =====" << endl;
    cout << "- This encryption uses a very basic left-shift 'S-box' which is linear and predictable.\n";
    cout << "- No MixColumns or key schedule diffusion, so pattern repetition is detectable.\n";
    cout << "- Not secure for real-world use; useful for educational purposes only.\n";
    cout << "- Try using a non-linear S-box and varying the key per round.\n";

    return 0;
}
