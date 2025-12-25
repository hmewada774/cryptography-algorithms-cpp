#include <iostream>
#include <string>
#include <ctime>
#include <unordered_map>
#include <bitset>
using namespace std;

int feistel(int half, int key)
{
    int result = (half ^ key) & 0xF;
    cout << "  S-box Output (Binary) after XOR with Key: " << bitset<8>(result) << endl; // XOR with key and output in binary
    return result;
}

int encryptBlock(int block, int k1, int k2, int k3, int k4)
{
    int L = (block >> 4) & 0xF;
    int R = block & 0xF;
    int keys[4] = {k1, k2, k3, k4};

    cout << "\n--- Encrypting Block ---\n";
    for (int i = 0; i < 4; ++i)
    {
        int temp = R;
        R = L ^ feistel(R, keys[i]);
        L = temp;

        // Output of each round in binary
        cout << "Round " << i + 1 << ": L = " << bitset<4>(L) << " R = " << bitset<4>(R) << endl;
    }
    return (L << 4) | R;
}

int decryptBlock(int block, int k1, int k2, int k3, int k4)
{
    int L = (block >> 4) & 0xF;
    int R = block & 0xF;
    int keys[4] = {k4, k3, k2, k1};

    cout << "\n--- Decrypting Block ---\n";
    for (int i = 0; i < 4; ++i)
    {
        int temp = L;
        L = R ^ feistel(L, keys[i]);
        R = temp;

        // Output of each round in binary
        cout << "Round " << i + 1 << ": L = " << bitset<4>(L) << " R = " << bitset<4>(R) << endl;
    }
    return (L << 4) | R;
}

string ecbEncrypt(string plaintext, int k1, int k2, int k3, int k4)
{
    string ciphertext = "";
    for (char c : plaintext)
    {
        int enc = encryptBlock((int)c, k1, k2, k3, k4);
        ciphertext += (char)enc;
    }
    return ciphertext;
}

string ecbDecrypt(string ciphertext, int k1, int k2, int k3, int k4)
{
    string plaintext = "";
    for (char c : ciphertext)
    {
        int dec = decryptBlock((int)c, k1, k2, k3, k4);
        plaintext += (char)dec;
    }
    return plaintext;
}

void analyzeFrequency(string text)
{
    unordered_map<char, int> freqMap;
    for (char c : text)
    {
        freqMap[c]++;
    }

    cout << "\n[Character Frequency Analysis of Ciphertext]\n";
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

    int k1 = 5, k2 = 9, k3 = 3, k4 = 12;

    // Measure execution time
    clock_t start = clock();

    // Encrypt the plaintext using ECB mode
    string encrypted = ecbEncrypt(text, k1, k2, k3, k4);

    // Decrypt the ciphertext using ECB mode
    string decrypted = ecbDecrypt(encrypted, k1, k2, k3, k4);

    // Measure end time
    clock_t end = clock();
    double execTime = double(end - start) / CLOCKS_PER_SEC;

    // Output encrypted and decrypted results
    cout << "\nEncrypted (raw): ";
    for (char c : encrypted)
        cout << (int)(unsigned char)c << " ";
    cout << "\nDecrypted: " << decrypted << endl;

    // Execution time
    cout << "\nExecution Time: " << execTime << " seconds" << endl;

    // Memory Usage Estimation (basic)
    int memoryUsed = sizeof(text) + sizeof(encrypted) + sizeof(decrypted) + sizeof(k1) + sizeof(k2) + sizeof(k3) + sizeof(k4);
    cout << "Estimated Memory Usage: " << memoryUsed << " bytes" << endl;

    // Cryptanalysis (Frequency analysis of ciphertext)
    analyzeFrequency(encrypted);

    return 0;
}
