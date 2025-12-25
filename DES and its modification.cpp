#include <iostream>
#include <ctime>
#include <unordered_map>
#include <bitset>
using namespace std;

int feistel(int half, int key)
{
    int result = (half ^ key) & 0xF;                                  // XOR and mask to get 4-bit output
    cout << "  S-box Output (Binary): " << bitset<4>(result) << endl; // S-box output in binary
    return result;
}

int desEncrypt(int plain, int k1, int k2, int k3, int k4)
{
    int L = (plain >> 4) & 0xF; // Left half of the plaintext
    int R = plain & 0xF;        // Right half of the plaintext

    // Round 1
    int temp = R;
    R = L ^ feistel(R, k1);
    L = temp;
    cout << "Round 1: L = " << bitset<4>(L) << ", R = " << bitset<4>(R) << endl;

    // Round 2
    temp = R;
    R = L ^ feistel(R, k2);
    L = temp;
    cout << "Round 2: L = " << bitset<4>(L) << ", R = " << bitset<4>(R) << endl;

    // Round 3
    temp = R;
    R = L ^ feistel(R, k3);
    L = temp;
    cout << "Round 3: L = " << bitset<4>(L) << ", R = " << bitset<4>(R) << endl;

    // Round 4
    temp = R;
    R = L ^ feistel(R, k4);
    L = temp;
    cout << "Round 4: L = " << bitset<4>(L) << ", R = " << bitset<4>(R) << endl;

    return (L << 4) | R; // Combine L and R back into one block
}

int desDecrypt(int cipher, int k1, int k2, int k3, int k4)
{
    int L = (cipher >> 4) & 0xF; // Left half of the ciphertext
    int R = cipher & 0xF;        // Right half of the ciphertext

    // Round 4
    int temp = L;
    L = R ^ feistel(L, k4);
    R = temp;
    cout << "Round 4: L = " << bitset<4>(L) << ", R = " << bitset<4>(R) << endl;

    // Round 3
    temp = L;
    L = R ^ feistel(L, k3);
    R = temp;
    cout << "Round 3: L = " << bitset<4>(L) << ", R = " << bitset<4>(R) << endl;

    // Round 2
    temp = L;
    L = R ^ feistel(L, k2);
    R = temp;
    cout << "Round 2: L = " << bitset<4>(L) << ", R = " << bitset<4>(R) << endl;

    // Round 1
    temp = L;
    L = R ^ feistel(L, k1);
    R = temp;
    cout << "Round 1: L = " << bitset<4>(L) << ", R = " << bitset<4>(R) << endl;

    return (L << 4) | R; // Combine L and R back into one block
}

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
    int msg;
    int k1 = 5, k2 = 9, k3 = 3, k4 = 12;

    cout << "Enter 8-bit message (0–255): ";
    cin >> msg;

    // Measure execution time
    clock_t start = clock();

    // Encryption
    int encrypted = desEncrypt(msg, k1, k2, k3, k4);

    // Decryption
    int decrypted = desDecrypt(encrypted, k1, k2, k3, k4);

    // Measure end time
    clock_t end = clock();
    double execTime = double(end - start) / CLOCKS_PER_SEC;

    // Output encrypted and decrypted results
    cout << "\nEncrypted: " << encrypted;
    cout << "\nDecrypted: " << decrypted << endl;

    // Execution time
    cout << "\nExecution Time: " << execTime << " seconds" << endl;

    // Memory Usage Estimation (basic)
    int memoryUsed = sizeof(msg) + sizeof(encrypted) + sizeof(decrypted) + sizeof(k1) + sizeof(k2) + sizeof(k3) + sizeof(k4);
    cout << "Estimated Memory Usage: " << memoryUsed << " bytes" << endl;

    // Cryptanalysis (Frequency analysis of ciphertext)
    string ciphertext = to_string(encrypted); // For simplicity, convert int to string
    analyzeFrequency(ciphertext);

    return 0;
}
