#include <iostream>
#include <ctime>
#include <iomanip> // For hex formatting
using namespace std;

const int BLOCK_SIZE = 64;

unsigned char simpleHash(const char *data, int len)
{
    unsigned char hash = 0;
    cout << "  simpleHash input (hex): ";
    for (int i = 0; i < len; ++i)
    {
        cout << hex << setw(2) << setfill('0') << (int)(unsigned char)data[i] << " ";
        hash = (hash + data[i]) ^ (hash << 1);
    }
    cout << "\n  simpleHash result: " << hex << (int)hash << "\n";
    return hash;
}

void prepareKey(char *key, int keyLen, char *outKey)
{
    for (int i = 0; i < BLOCK_SIZE; ++i)
        outKey[i] = (i < keyLen) ? key[i] : 0;
}

unsigned char hmac(const char *key, int keyLen, const char *message, int msgLen)
{
    char o_key_pad[BLOCK_SIZE], i_key_pad[BLOCK_SIZE], actualKey[BLOCK_SIZE];
    prepareKey((char *)key, keyLen, actualKey);

    for (int i = 0; i < BLOCK_SIZE; ++i)
    {
        o_key_pad[i] = actualKey[i] ^ 0x5c;
        i_key_pad[i] = actualKey[i] ^ 0x36;
    }

    char innerData[BLOCK_SIZE + msgLen];
    for (int i = 0; i < BLOCK_SIZE; ++i)
        innerData[i] = i_key_pad[i];
    for (int i = 0; i < msgLen; ++i)
        innerData[BLOCK_SIZE + i] = message[i];

    cout << "\n--- Inner Hashing ---\n";
    unsigned char innerHash = simpleHash(innerData, BLOCK_SIZE + msgLen);

    char outerData[BLOCK_SIZE + 1];
    for (int i = 0; i < BLOCK_SIZE; ++i)
        outerData[i] = o_key_pad[i];
    outerData[BLOCK_SIZE] = innerHash;

    cout << "\n--- Outer Hashing ---\n";
    unsigned char finalHMAC = simpleHash(outerData, BLOCK_SIZE + 1);
    return finalHMAC;
}

size_t estimateMemory()
{
    return BLOCK_SIZE * 4 + sizeof(unsigned char) * 2;
}

int main()
{
    const char *key = "mysecretkey";
    const char *message = "this is a test";

    clock_t start = clock();
    unsigned char tag = hmac(key, 11, message, 14);
    clock_t end = clock();

    double execTime = double(end - start) / CLOCKS_PER_SEC;

    cout << "\n--- Final HMAC Output ---\n";
    cout << "HMAC (simplified): 0x" << hex << (int)tag << endl;

    cout << "\n--- Performance Metrics ---\n";
    cout << "Execution Time: " << fixed << execTime << " seconds\n";
    cout << "Estimated Memory Used: " << estimateMemory() << " bytes\n";

    cout << "\n--- Cryptanalysis Summary ---\n";
    cout << "1. This simplified HMAC lacks cryptographic collision resistance.\n";
    cout << "2. Vulnerable to length extension and brute-force key guessing.\n";
    cout << "3. No cryptographic hash (e.g., SHA-256) used — not secure in practice.\n";
    cout << "4. Good for demonstration, not real-world security.\n";
    cout << "5. True HMAC ensures resistance to message tampering and forgeries.\n";

    return 0;
}
