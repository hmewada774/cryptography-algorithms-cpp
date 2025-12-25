#include <iostream>
#include <ctime>
#include <unordered_map>
#include <bitset> // <-- Added for bitset
using namespace std;

// Function to simulate the Feistel round (substitution with XOR operation)
string feistelRound(string half, string key)
{
    string result = "";
    for (int i = 0; i < half.length(); ++i)
    {
        char temp = half[i] ^ key[i % key.length()];
        result += temp;
        // Convert char to unsigned int before using bitset
        cout << "Feistel Round S-box Output (binary): " << bitset<8>((unsigned char)temp) << endl;
    }
    return result;
}

// Feistel network encryption function
string feistelEncrypt(string text, string key)
{
    if (text.length() % 2 != 0)
        text += 'X'; // Pad with 'X' if odd length
    int halfLen = text.length() / 2;
    string L = text.substr(0, halfLen);
    string R = text.substr(halfLen);

    // Start measuring execution time
    clock_t start = clock();

    for (int round = 0; round < 2; ++round)
    {
        cout << "Round " << round + 1 << " - L: " << L << ", R: " << R << endl;
        string F = feistelRound(R, key); // Feistel function
        string newR = "";
        for (int i = 0; i < halfLen; ++i)
            newR += L[i] ^ F[i];
        L = R;
        R = newR;
    }

    // End execution time measurement
    clock_t end = clock();
    double execTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "Encryption Execution Time: " << execTime << " seconds" << endl;

    return L + R;
}

// Feistel network decryption function
string feistelDecrypt(string cipher, string key)
{
    int halfLen = cipher.length() / 2;
    string L = cipher.substr(0, halfLen);
    string R = cipher.substr(halfLen);

    // Start measuring execution time
    clock_t start = clock();

    for (int round = 0; round < 2; ++round)
    {
        cout << "Round " << round + 1 << " - L: " << L << ", R: " << R << endl;
        string F = feistelRound(L, key); // Feistel function (inverse round)
        string newL = "";
        for (int i = 0; i < halfLen; ++i)
            newL += R[i] ^ F[i];
        R = L;
        L = newL;
    }

    // End execution time measurement
    clock_t end = clock();
    double execTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "Decryption Execution Time: " << execTime << " seconds" << endl;

    return L + R;
}

// Basic frequency analysis to check randomness
void frequencyAnalysis(string text)
{
    unordered_map<char, int> freqMap;
    for (char c : text)
    {
        freqMap[c]++;
    }

    cout << "\n[Frequency Analysis of Text]\n";
    for (auto &pair : freqMap)
    {
        cout << pair.first << ": " << pair.second << endl;
    }
}

int main()
{
    string text = "HELLOX"; // Message to encrypt
    string key = "KEY";     // Key for Feistel cipher
    cout << "Original Text: " << text << endl;

    // Encrypting the text
    string encrypted = feistelEncrypt(text, key);
    cout << "\nEncrypted Text: ";
    for (char c : encrypted)
        cout << c << " ";
    cout << endl;

    // Perform Frequency Analysis on Encrypted Text
    frequencyAnalysis(encrypted);

    // Decrypting the text
    string decrypted = feistelDecrypt(encrypted, key);
    cout << "\nDecrypted Text: " << decrypted << endl;

    // Perform Frequency Analysis on Decrypted Text
    frequencyAnalysis(decrypted);

    return 0;
}
